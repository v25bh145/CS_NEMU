#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute () {
    if (op_src -> size == 1 && op_dest -> size != 1) op_src -> val = (int8_t)op_src -> val;
	    op_src -> val += cpu.psw->CF;
   
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest, result);


	testfor_flags_s(result, 0xFFF);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
