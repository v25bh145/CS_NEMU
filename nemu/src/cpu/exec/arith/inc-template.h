#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);
	panic("%x %x ", instr_fetch(0x000010a8, 1), instr_fetch(0x000010a7, 1));
	testfor_flags_s(result, 0x01 + 0x80 + 0x40 + 0x10 + 0x4);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
