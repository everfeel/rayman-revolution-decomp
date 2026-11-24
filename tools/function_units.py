"""Split preprocessed C modules into link-ordered, function-sized inputs."""

import ast
import hashlib
import re
from dataclasses import dataclass


FUNCTION_NAME = re.compile(r"func_[0-9A-Fa-f]{8}\Z")
FUNCTION_TOKEN = re.compile(r"\bfunc_[0-9A-Fa-f]{8}(?:_impl)?\b")
ASM_ALIAS = re.compile(
    r"\b(?P<c_name>[A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}]*?\)\s*"
    r"__asm__\s*\(\s*\"(?P<symbol>func_[0-9A-Fa-f]{8})\"\s*\)\s*;",
    re.DOTALL,
)
STRING_LITERAL = re.compile(r'"(?:\\.|[^"\\])*"')
LINE_MARKER = re.compile(r'(?m)^[ \t]*#[ \t]*(?:line[ \t]+)?\d+[ \t]+"[^"\n]*"[^\n]*\n?')
ASM_CALL = re.compile(r"\b__asm__\s*\(")
ASM_INCLUDE = re.compile(r'(?m)\.include\s+"([^"]+\.s)"')


@dataclass(frozen=True)
class CFunction:
    name: str
    start: int
    body_start: int
    end: int
    prototype: str


@dataclass(frozen=True)
class AssemblyFunction:
    name: str
    start: int
    end: int
    include_path: str


@dataclass(frozen=True)
class AssemblyDirective:
    start: int
    end: int
    statement: str


@dataclass
class CFunctionUnit:
    name: str
    prefix_parts: tuple
    body: str
    source_digest: str
    postlude_parts: tuple = ()

    def source(self):
        return "".join(self.prefix_parts) + self.body + "".join(self.postlude_parts)


@dataclass(frozen=True)
class AssemblyFunctionUnit:
    name: str
    include_path: str


def _mask_non_code(text):
    chars = list(text)
    state = "code"
    index = 0
    while index < len(text):
        char = text[index]
        pair = text[index:index + 2]
        if state == "line_comment":
            if char == "\n":
                state = "code"
            else:
                chars[index] = " "
        elif state == "block_comment":
            if pair == "*/":
                chars[index] = chars[index + 1] = " "
                state = "code"
                index += 1
            elif char != "\n":
                chars[index] = " "
        elif state in ("string", "character"):
            if char == "\\":
                if char != "\n":
                    chars[index] = " "
                if index + 1 < len(text):
                    if text[index + 1] != "\n":
                        chars[index + 1] = " "
                    index += 1
            elif (state == "string" and char == '"') or (
                state == "character" and char == "'"
            ):
                chars[index] = " "
                state = "code"
            elif char != "\n":
                chars[index] = " "
        elif pair == "//":
            chars[index] = chars[index + 1] = " "
            state = "line_comment"
            index += 1
        elif pair == "/*":
            chars[index] = chars[index + 1] = " "
            state = "block_comment"
            index += 1
        elif char == '"':
            chars[index] = " "
            state = "string"
        elif char == "'":
            chars[index] = " "
            state = "character"
        index += 1
    return "".join(chars)


def _matching_delimiter(masked, start, opening, closing):
    depth = 0
    for index in range(start, len(masked)):
        if masked[index] == opening:
            depth += 1
        elif masked[index] == closing:
            depth -= 1
            if depth == 0:
                return index
    return None


def _skip_space(masked, index):
    while index < len(masked) and masked[index].isspace():
        index += 1
    return index


def _old_style_body(masked, name, close_paren):
    start = _skip_space(masked, close_paren + 1)
    if start >= len(masked) or masked[start] in "{;":
        return None

    brace = masked.find("{", start, start + 4096)
    if brace < 0:
        return None
    semicolon = masked.find(";", start, brace)
    if semicolon < 0:
        return None
    if FUNCTION_TOKEN.search(masked, start, brace):
        return None

    declarations = masked[start:brace]
    if not re.fullmatch(r"[A-Za-z_0-9\s*(),\[\];]+", declarations):
        return None
    if "=" in declarations or "__attribute__" in declarations:
        return None
    return brace


def _old_style_prototype(text, start, open_paren):
    return text[start:open_paren].rstrip() + "();\n"


def _function_definitions(text, masked):
    functions = []
    aliases = {
        match.group("c_name"): match.group("symbol")
        for match in ASM_ALIAS.finditer(text)
    }
    if aliases:
        candidate = re.compile(
            r"\b(?:func_[0-9A-Fa-f]{8}(?:_impl)?|"
            + "|".join(re.escape(name) for name in sorted(aliases, key=len, reverse=True))
            + r")\b"
        )
    else:
        candidate = FUNCTION_TOKEN
    cursor = 0
    while True:
        match = candidate.search(masked, cursor)
        if match is None:
            break
        c_name = match.group()
        name = aliases.get(c_name, c_name)
        open_paren = _skip_space(masked, match.end())
        if open_paren >= len(masked) or masked[open_paren] != "(":
            cursor = match.end()
            continue
        close_paren = _matching_delimiter(masked, open_paren, "(", ")")
        if close_paren is None:
            raise ValueError(f"{name}: unterminated function declaration")

        body_start = _skip_space(masked, close_paren + 1)
        old_style = False
        if body_start < len(masked) and masked[body_start] != "{":
            if masked[body_start] == ";":
                cursor = match.end()
                continue
            body_start = _old_style_body(masked, name, close_paren)
            if body_start is None:
                cursor = match.end()
                continue
            old_style = True

        end = _matching_delimiter(masked, body_start, "{", "}")
        if end is None:
            raise ValueError(f"{name}: unterminated function body")
        start = text.rfind("\n", 0, match.start()) + 1
        if old_style:
            prototype = _old_style_prototype(text, start, open_paren)
        else:
            prototype = text[start:body_start].rstrip() + ";\n"
        functions.append(CFunction(name, start, body_start, end + 1, prototype))
        cursor = end + 1
    return functions


def _assembly_functions(text, masked, functions):
    assembly_functions = []
    for match in ASM_CALL.finditer(masked):
        if any(function.start <= match.start() < function.end for function in functions):
            continue
        open_paren = masked.find("(", match.start(), match.end())
        close_paren = _matching_delimiter(masked, open_paren, "(", ")")
        if close_paren is None:
            raise ValueError("unterminated top-level __asm__ statement")
        semicolon = _skip_space(masked, close_paren + 1)
        if semicolon >= len(masked) or masked[semicolon] != ";":
            continue

        statement = text[match.start():semicolon + 1]
        try:
            contents = "".join(
                ast.literal_eval(literal.group())
                for literal in STRING_LITERAL.finditer(statement)
            )
        except (SyntaxError, ValueError):
            continue
        include = ASM_INCLUDE.search(contents)
        if include is None:
            start = text.rfind("\n", 0, match.start()) + 1
            if masked[start:match.start()].strip():
                continue
            if 'include/macro.inc' in contents:
                continue
            assembly_functions.append(
                AssemblyDirective(start, semicolon + 1, statement)
            )
            continue
        include_path = include.group(1)
        if not (
            include_path.startswith("asm/nonmatchings/")
            or include_path.startswith("src/asm/")
        ):
            continue
        name = include_path.rsplit("/", 1)[-1][:-2]
        if not FUNCTION_NAME.fullmatch(name):
            raise ValueError(
                f"top-level assembly includes an unsupported function path: "
                f"{include_path}"
            )
        start = text.rfind("\n", 0, match.start()) + 1
        assembly_functions.append(
            AssemblyFunction(name, start, semicolon + 1, include_path)
        )
    return assembly_functions


def iter_function_units(preprocessed_text):
    """Yield link-ordered function inputs, replacing prior bodies with prototypes."""
    text = LINE_MARKER.sub("", preprocessed_text)
    masked = _mask_non_code(text)
    functions = _function_definitions(text, masked)
    assembly_functions = _assembly_functions(text, masked, functions)
    items = sorted(
        [*functions, *assembly_functions],
        key=lambda item: item.start,
    )
    directive_tails = {}
    previous_function = None
    for item in items:
        if isinstance(item, AssemblyDirective):
            if not isinstance(previous_function, CFunction):
                raise ValueError(
                    "top-level assembly directive is not preceded by a C function"
                )
            directive_tails.setdefault(id(previous_function), []).append(
                item.statement
            )
        else:
            previous_function = item
    names = set()
    for item in items:
        if isinstance(item, AssemblyDirective):
            continue
        if item.name in names:
            raise ValueError(f"duplicate function input for {item.name}")
        names.add(item.name)

    prefix_parts = []
    prefix_digest = hashlib.sha256()
    cursor = 0
    for item in items:
        gap = text[cursor:item.start]
        prefix_parts.append(gap)
        prefix_digest.update(gap.encode("utf-8"))
        if isinstance(item, AssemblyDirective):
            cursor = item.end
            continue
        if isinstance(item, CFunction):
            body = text[item.start:item.end]
            postlude = tuple(directive_tails.get(id(item), ()))
            digest = hashlib.sha256()
            digest.update(prefix_digest.copy().digest())
            digest.update(body.encode("utf-8"))
            for statement in postlude:
                digest.update(statement.encode("utf-8"))
            yield CFunctionUnit(
                item.name,
                tuple(prefix_parts),
                body,
                digest.hexdigest(),
                postlude,
            )
            prefix_parts.append(item.prototype)
            prefix_digest.update(item.prototype.encode("utf-8"))
        else:
            yield AssemblyFunctionUnit(item.name, item.include_path)
        cursor = item.end
