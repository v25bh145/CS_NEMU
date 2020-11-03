#include "cpu/exec/template-start.h"

#define instr shl

static void do_execute () {
	uint32_t src = op_src->val;
	uint32_t dest = op_dest->val;
	uint8_t count = src & 0x1f;
	dest <<= count;

	OPERAND_W(op_dest, dest);

	DATA_TYPE tmp = dest;
	cpu.psw->OF = 0;
	cpu.psw->CF = 0;
	cpu.psw->ZF = !tmp;
    cpu.psw->SF = tmp >> ((DATA_BYTE << 3) - 1);
	tmp ^= tmp >> 4;
    tmp ^= tmp >> 2;
    tmp ^= tmp >> 1;
    tmp &= 1;
    cpu.psw->PF = !tmp;
	//ZF PF SF
	// testfor_flags_s(dest, 0x40 + 0x80 + 0x4);

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
