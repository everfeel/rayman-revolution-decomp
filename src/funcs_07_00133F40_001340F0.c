#include "common.h"

INCLUDE_ASM(const s32, "core/main", func_00133F40);

INCLUDE_ASM(const s32, "core/main", func_001340F0);

s32 func_00108648(u8 *arg0);
s32 func_00135530();
/* Confirmed from the retail listing (see REFERENCE-BEHAVIORMATCH.md):
 * `func_00139708(actor, f32 *normal)` returns 1 when
 * `actor->+0x18 <= normal[2]`; `normal` points at a SurfaceContact's +0x04
 * field (a floor-like surface test).  Not called from this module, but
 * declared with its real signature so it is not reconstructed as (s32, s32). */
s32 func_00139708(s32 actor, f32 *normal);
s32 func_00131D78(s32, s32, f32);
s32 func_00135CF8(s32, s32, s32, s32, s32);
f32 func_00139828(s32, s32);
f32 func_00139858(s32, s32);
/* Typed definition in funcs_11 (f32 *, void *, s32, SurfaceContact *, s32);
 * declared with s32 spellings so the arity is checked at the call sites. */
s32 func_00136C08(s32 position, s32 actor, s32 count, s32 contacts, s32 indexLimit);
s32 func_0013A148(s32, f32);

/* Draft for func_001346E8 - standalone test version. */

extern s32 D_003931B0;          /* object entry array base (pointer value) */
extern s32 D_003931B8;
extern f32 D_003931B4;
extern f32 D_00392718;
extern f32 D_0039271C;
extern f32 D_00392720;
extern f32 D_00392724;
extern f32 D_00392728;
extern f32 D_0039272C;
extern f32 D_00392730;
extern s32 D_00330ED0[];
