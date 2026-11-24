import unittest

from tools.function_units import (
    AssemblyFunctionUnit,
    CFunctionUnit,
    iter_function_units,
)


class FunctionUnitTests(unittest.TestCase):
    def test_old_style_definition_is_rewritten_as_a_prototype(self):
        source = """
s32 func_00100000(p)
u8 *p;
{
    return p[0];
}
void func_00100008(void) {}
"""
        units = list(iter_function_units(source))

        self.assertEqual(len(units), 2)
        self.assertIsInstance(units[0], CFunctionUnit)
        self.assertEqual(units[0].name, "func_00100000")
        self.assertIn("func_00100000();", units[1].source())

    def test_old_style_definition_remains_a_no_prototype_declaration(self):
        source = """
extern s32 func_00100000();
s32 func_00100000(arg0, arg1)
u16 arg0;
u8 arg1;
{
    return arg0 + arg1;
}
void func_00100008(void) {}
"""
        units = list(iter_function_units(source))

        self.assertIn("func_00100000();", units[1].source())

    def test_arbitrary_assembler_alias_uses_the_linker_symbol(self):
        source = """
s32 w1_vararg_impl(s32 first, ...) __asm__("func_00226310");
s32 w1_vararg_impl(s32 first, ...)
{
    return first;
}
"""
        units = list(iter_function_units(source))

        self.assertEqual(len(units), 1)
        self.assertEqual(units[0].name, "func_00226310")

    def test_strings_and_comments_do_not_create_assembly_units(self):
        source = r'''
void func_00100000(void)
{
    const char *text = "__asm__(\".include \\\"asm/nonmatchings/fake.s\\\"\");";
    /* INCLUDE_ASM("asm/nonmatchings/fake/func_00100008.s"); */
}
'''
        units = list(iter_function_units(source))

        self.assertEqual([unit.name for unit in units], ["func_00100000"])

    def test_top_level_assembly_include_joins_adjacent_literals(self):
        source = r'''
void func_00100000(void) {}
__asm__(".include \"asm/nonmatchings/core/main/"
        "func_00100008.s\"");
'''
        units = list(iter_function_units(source))

        self.assertEqual(len(units), 2)
        self.assertIsInstance(units[1], AssemblyFunctionUnit)
        self.assertEqual(units[1].name, "func_00100008")

    def test_top_level_assembly_directive_is_emitted_once_after_its_function(self):
        source = """
void func_00100000(void) {}
__asm__(".space 12, 0");
void func_00100008(void) {}
"""
        units = list(iter_function_units(source))

        self.assertIn('__asm__(".space 12, 0");', units[0].source())
        self.assertNotIn(".space 12", units[1].source())

    def test_function_prefix_is_a_snapshot(self):
        source = """
void func_00100000(void) {}
void func_00100008(void) {}
"""
        units = list(iter_function_units(source))

        self.assertNotIn("func_00100008", units[0].source())
        self.assertIn("func_00100000", units[1].source())


if __name__ == "__main__":
    unittest.main()
