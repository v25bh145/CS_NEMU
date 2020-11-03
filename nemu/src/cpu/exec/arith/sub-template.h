#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest, result);

	testfor_flags_s(result, 0xFFF);
	if(cpu.eip == 0x10015d) 
		Log("sub eax: %d", cpu.eax);
	
	print_asm_template2();
}

make_instr_helper(i2a);
make_instr_helper(i2rm);
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
