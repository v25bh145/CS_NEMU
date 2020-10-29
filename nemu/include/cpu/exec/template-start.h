#include "cpu/exec/helper.h"

#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]

#define MEM_R(addr) swaddr_read(addr, DATA_BYTE)
#define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data)

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))

#define test_for_flags concat(test_for_flags_, DATA_BYTE)(DATA_TYPE result) {\
    if(op_src->val > 0 && op_dest->val < 0 && result < 0) {\
        cpu.psw->SF = 1;\
        cpu.psw->OF = 1;\
    } else if (op_src->val < 0 && op_dest->val > 0 && result > 0) {\
        cpu.psw->SF = 0;\
        cpu.psw->OF = 1;\
    } else if (!result) {\
        cpu.psw->ZF = 1;\
    } else {\
        cpu.psw->SF = result < 0;\
    }\
	DATA_TYPE cnt = 0, tmp = result, tmp2 = op_dest->val;\
	while( ( tmp & 1 ) > 0 && tmp > 0) {\
		cnt += ( tmp & 1 );\
		tmp = tmp >> 1;\
	}\
	cpu.psw->PF = cnt % 2 ? 0 : 1;\
	cpu.psw->CF = ( ( result | op_dest->val ) - op_dest->val ) > 0;\
	tmp = result & 0xF;\
	tmp2 = op_dest->val & 0xF;\
	cpu.psw->AF = ( ( tmp | tmp2 ) - tmp2) > 0;\
}
