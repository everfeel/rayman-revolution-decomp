#include "common.h"

/* func_001022E8 through func_001022E8; retail address range 0x001022E8-0x00102548. */


INCLUDE_ASM(const s32, "core/main", func_001022E8);
extern u8 D_00252230[9];
extern void func_00144C80(void);

/* func_00102548 through func_00102548; retail address range 0x00102548-0x00102570. */
void func_00102548(void)
{
    func_00112E30();
    D_00252230[0] = 1;
    func_00144C80();
}
