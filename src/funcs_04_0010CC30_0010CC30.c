#include "common.h"

/* NOTE: deliberately no `const` here. Old gcc (2.x / ee-gcc) can treat `const`
 * on a function declaration as the "const function" attribute (no side effects,
 * result depends only on args), which lets -O2 delete any call whose result is
 * unused and CSE/hoist the rest. Several of these exist purely for side effects. */
extern void func_00112FF8(u8 *arg0, u8 arg1);
extern s32 func_0010DD18();
extern s32 func_0010E920(u8 *arg0);
extern s32 func_001104A0();
extern void func_00127330(u32 arg0, u32 arg1);
extern void func_00158E60(u8 *arg0, u8 arg1);
extern void func_00159308(u8 *arg0);
extern u32 func_00159300(u8 *arg0);
extern void func_00159308(u8 *);
extern u8 *func_0010CB00();
extern u8 *func_00158EC8();
extern void func_00120D48();
extern void func_0010DE30();
extern void func_001106D0();

s32 func_0010CC30(u8 *arg0, u16 arg1, u8 *arg2)
{
    u8 *q;
    u8 *tbl;
    u8 *cursor;
    u8 *end;
    u8 *arr;
    u8 *slot;
    u8 *holder;
    u8 *node;
    u8 *rec;
    u8 *t;
    u32 idx;
    u16 word;
    u8 t3;
    f32 f20;
    u32 _fl, _v;
    u8 *_e, *_e2;

    f20 = *(f32 *)(arg2 + 0x3C);
    q = *(u8 **)(*(u32 *)(arg2 + 4));
    tbl = *(u8 **)(*(u32 *)(q + 0x10));

    func_001104A0(arg2, q + 0x94, arg0, arg1);

    cursor = *(u8 **)(q + 0x94);
    end = cursor + arg0[0x1B] * 6;
    if (cursor != end) {
        do {
            if (cursor[2] == 0xFF) {
                cursor += 6;
                continue;
            }
            arr = *(u8 **)(q + 0x88);
            slot = arr + cursor[2] * 0x20;
            holder = *(u8 **)slot;
            t3 = cursor[3];
            if (t3 == 0 && ((word = *(u16 *)cursor) & 0x8000) != 0) {
                idx = word & 0x7FFF;
                rec = tbl + idx * 0x14;
                if (*(u16 *)(rec + 0xC) == 0xFFFF) {
                    *(u32 *)(holder + 4) = 0;
                    *(u32 *)(holder + 0) = 1;
                } else if (*(u32 *)rec == 0) {
                    if ((*(u8 **)(slot + 4)) != 0) {
                        if (*(u8 **)((*(u8 **)(slot + 4)) + 0x14) != 0)
                            *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x14) + 0x18) = *(u8 **)((*(u8 **)(slot + 4)) + 0x18);
                        if (*(u8 **)((*(u8 **)(slot + 4)) + 0x18) != 0)
                            *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x18) + 0x14) = *(u8 **)((*(u8 **)(slot + 4)) + 0x14);
                        if (*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) != 0) {
                            (*(u32 *)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x10))--;
                            if (*(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x08) == (u8 *)((*(u8 **)(slot + 4))))
                                *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x08) = *(u8 **)((*(u8 **)(slot + 4)) + 0x14);
                            if (*(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x0C) == (u8 *)((*(u8 **)(slot + 4))))
                                *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x0C) = *(u8 **)((*(u8 **)(slot + 4)) + 0x18);
                            *(u8 **)((*(u8 **)(slot + 4)) + 0x14) = 0;
                        } else {
                            *(u8 **)((*(u8 **)(slot + 4)) + 0x14) = 0;
                        }
                        *(u8 **)((*(u8 **)(slot + 4)) + 0x18) = 0;
                        *(u8 **)((*(u8 **)(slot + 4)) + 0x1C) = 0;
                        func_0010DE30((s32)((*(u8 **)(slot + 4))));
                        *(u8 **)(slot + 4) = 0;
                    }
                    _fl = *(u16 *)(rec + 0xC);
                    if (*(u32 *)(arg0 + 4) == 0) {
                        if (_fl == 0) {
                            _v = *(u32 *)(rec + 4);
                            *(u32 *)(holder + 4) = _v;
                            *(u32 *)(holder + 0) = (_v != 0) ? 8 : 1;
                        }
                    } else if (_fl == 0) {
                        _e = (u8 *)func_0010CB00(arg0, cursor[2], arg1);
                        if (_e == 0) {
                            func_00158E60(arg2, cursor[2]);
                            _v = *(u32 *)(rec + 4);
                            *(u32 *)(holder + 0) = (_v != 0) ? 8 : 1;
                            *(u32 *)(holder + 4) = *(u32 *)(rec + 4);
                        } else if (_e[1] < 0x64) {
                            _e2 = (u8 *)func_00158EC8(arg2, idx, _e[0], _e[1], cursor[2]);
                            if (_e2 != 0) {
                                func_00159308(_e2);
                                _v = func_00159300(_e2);
                                *(u32 *)(holder + 4) = _v;
                                *(u32 *)(holder + 0) = (_v != 0) ? 8 : 1;
                            } else {
                                _v = *(u32 *)(tbl + idx * 0x14 + 4);
                                *(u32 *)(holder + 0) = (_v != 0) ? 8 : 1;
                                *(u32 *)(holder + 4) = *(u32 *)(tbl + idx * 0x14 + 4);
                            }
                        } else {
                            func_00158E60(arg2, cursor[2]);
                            _v = *(u32 *)(tbl + _e[0] * 0x14 + 4);
                            *(u32 *)(holder + 4) = _v;
                            *(u32 *)(holder + 0) = (_v != 0) ? 8 : 1;
                        }
                    }
                    *(u32 *)(holder + 0x34) = 0;
                    *(u32 *)(holder + 0x38) = 0;
                    t3 = cursor[3];
                } else {
                    node = *(u8 **)(slot + 4);
                    if (node == 0) {
                        *(u32 *)(holder + 4) = 0;
                        *(u32 *)(holder + 0) = 1;
                        node = (u8 *)func_0010DD18();
                        if (*(u32 *)(node + 0x14) != 0)
                            *(u32 *)(*(u32 *)(node + 0x14) + 0x18) = *(u32 *)(node + 0x18);
                        if (*(u32 *)(node + 0x18) != 0)
                            *(u32 *)(*(u32 *)(node + 0x18) + 0x14) = *(u32 *)(node + 0x14);
                        if (*(u32 *)(node + 0x1C) != 0) {
                            (*(u32 *)(*(u32 *)(node + 0x1C) + 0x10))--;
                            if (*(u32 *)(*(u32 *)(node + 0x1C) + 0x08) == (u32)(node))
                                *(u32 *)(*(u32 *)(node + 0x1C) + 0x08) = *(u32 *)(node + 0x14);
                            if (*(u32 *)(*(u32 *)(node + 0x1C) + 0x0C) == (u32)(node))
                                *(u32 *)(*(u32 *)(node + 0x1C) + 0x0C) = *(u32 *)(node + 0x18);
                            *(u32 *)(node + 0x18) = 0;
                        } else {
                            *(u32 *)(node + 0x18) = 0;
                        }
                        *(u32 *)(node + 0x1C) = 0;
                        *(u32 *)(node + 0x14) = 0;
                        *(u32 *)(node + 0x1C) = (u32)holder;
                        *(u32 *)(node + 0x18) = *(u32 *)(holder + 0x0C);
                        if (*(u32 *)(holder + 0x08) == 0)
                            *(u32 *)(holder + 0x08) = (u32)node;
                        else
                            *(u32 *)(*(u32 *)(holder + 0x0C) + 0x14) = (u32)node;
                        *(u32 *)(holder + 0x0C) = (u32)node;
                        (*(u32 *)(holder + 0x10))++;
                        *(u32 *)(slot + 4) = (u32)node;
                    }
                    _fl = *(u16 *)(rec + 0xC);
                    if (*(u32 *)(arg0 + 4) == 0) {
                        if (_fl == 0) {
                            _v = *(u32 *)(rec + 4);
                            *(u32 *)(node + 4) = _v;
                            *(u32 *)(node + 0) = (_v != 0) ? 8 : 1;
                        }
                    } else if (_fl == 0) {
                        _e = (u8 *)func_0010CB00(arg0, cursor[2], arg1);
                        if (_e == 0) {
                            func_00158E60(arg2, cursor[2]);
                            _v = *(u32 *)(rec + 4);
                            *(u32 *)(node + 0) = (_v != 0) ? 8 : 1;
                            *(u32 *)(node + 4) = *(u32 *)(rec + 4);
                        } else if (_e[1] < 0x64) {
                            _e2 = (u8 *)func_00158EC8(arg2, idx, _e[0], _e[1], cursor[2]);
                            if (_e2 != 0) {
                                func_00159308(_e2);
                                _v = func_00159300(_e2);
                                *(u32 *)(node + 4) = _v;
                                *(u32 *)(node + 0) = (_v != 0) ? 8 : 1;
                            } else {
                                _v = *(u32 *)(tbl + idx * 0x14 + 4);
                                *(u32 *)(node + 0) = (_v != 0) ? 8 : 1;
                                *(u32 *)(node + 4) = *(u32 *)(tbl + idx * 0x14 + 4);
                            }
                        } else {
                            func_00158E60(arg2, cursor[2]);
                            _v = *(u32 *)(tbl + _e[0] * 0x14 + 4);
                            *(u32 *)(node + 4) = _v;
                            *(u32 *)(node + 0) = (_v != 0) ? 8 : 1;
                        }
                    }
                    *(u32 *)(node + 0x34) = 0;
                    *(u32 *)(node + 0x38) = 0;
                    func_00120D48(*(u32 *)(node + 0x20));
                    func_00127330(*(u32 *)(node + 0x20), *(u32 *)rec);
                    {
                        u8 *o = *(u8 **)rec;

                        if (*(f32 *)o * (*(f32 *)(o + 4) * *(f32 *)(o + 8)) < 0.0f)
                            *(u32 *)(node + 0x2C) &= 0xFFF7FFFF;
                    }
                    if ((*(u32 *)(node + 0x30) & 0x10000) == 0) {
                        if (*(u32 *)(arg2 + 0x2C) & 8) {
                            if (cursor[4] == 0xFF) {
                                *(f32 *)(node + 0x3C) = f20;
                                *(u32 *)(node + 0x2C) |= 8;
                            } else {
                                *(u32 *)(node + 0x2C) &= ~8;
                                *(f32 *)(node + 0x3C) = (f32)cursor[4];
                            }
                        } else {
                            *(f32 *)(node + 0x3C) = f20;
                        }
                    }
                    t3 = cursor[3];
                }
            }

            /* Types 2, 3, 5 and 6 always tear the node down. Any other type
             * keeps it only if bit 15 of the entry word is set. */
            if (t3 != 2 && t3 != 3 && t3 != 5 && t3 != 6 &&
                (*(u16 *)cursor & 0x8000) != 0)
                goto state_update;

            if ((*(u8 **)(slot + 4)) != 0) {
                if (*(u8 **)((*(u8 **)(slot + 4)) + 0x14) != 0)
                    *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x14) + 0x18) = *(u8 **)((*(u8 **)(slot + 4)) + 0x18);
                if (*(u8 **)((*(u8 **)(slot + 4)) + 0x18) != 0)
                    *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x18) + 0x14) = *(u8 **)((*(u8 **)(slot + 4)) + 0x14);
                if (*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) != 0) {
                    (*(u32 *)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x10))--;
                    if (*(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x08) == (u8 *)((*(u8 **)(slot + 4))))
                        *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x08) = *(u8 **)((*(u8 **)(slot + 4)) + 0x14);
                    if (*(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x0C) == (u8 *)((*(u8 **)(slot + 4))))
                        *(u8 **)(*(u8 **)((*(u8 **)(slot + 4)) + 0x1C) + 0x0C) = *(u8 **)((*(u8 **)(slot + 4)) + 0x18);
                    *(u8 **)((*(u8 **)(slot + 4)) + 0x14) = 0;
                } else {
                    *(u8 **)((*(u8 **)(slot + 4)) + 0x14) = 0;
                }
                *(u8 **)((*(u8 **)(slot + 4)) + 0x18) = 0;
                *(u8 **)((*(u8 **)(slot + 4)) + 0x1C) = 0;
                func_0010DE30((s32)((*(u8 **)(slot + 4))));
                *(u8 **)(slot + 4) = 0;
            }
            *(u32 *)(holder + 4) = 0;
            *(u32 *)(holder + 0) = 1;
        state_update:
            if ((*(u32 *)(holder + 0x30) & 0x10000) == 0) {
                if (*(u32 *)(arg2 + 0x2C) & 8) {
                    if (cursor[4] == 0xFF) {
                        *(f32 *)(holder + 0x3C) = f20;
                        *(u32 *)(holder + 0x2C) |= 8;
                    } else {
                        *(u32 *)(holder + 0x2C) &= ~8;
                        *(f32 *)(holder + 0x3C) = (f32)cursor[4];
                    }
                } else {
                    *(f32 *)(holder + 0x3C) = f20;
                }
            }
            t = *(u8 **)(slot + 4);
            if (t == 0)
                t = *(u8 **)slot;
            func_00112FF8(t, slot[0x11]);
            cursor += 6;
        } while (cursor != end);
    }
    func_001106D0(arg2, q + 0x94, arg0, arg1, *(u8 *)(q + 0xF0), *(s32 *)(q + 0x88));
    *(u8 *)(q + 0xF0) = 0;
    return func_0010E920(arg2);
}
