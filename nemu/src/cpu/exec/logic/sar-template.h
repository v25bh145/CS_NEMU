#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	panic("awa");
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	OPERAND_W(op_dest, dest);

	int tmp = dest;
	cpu.psw->OF = 0;
	cpu.psw->CF = 0;
	while((tmp ^ 1) != 0 && dest != 0) {
		tmp >>= 1;
		cpu.psw->CF++;
	}
	
	//ZF PF SF
	testfor_flags_s(dest, 0x40 + 0x80 + 0x4);

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
