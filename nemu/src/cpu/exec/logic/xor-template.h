#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	// if(cpu.eip == 0x1000b1)
	// Log("bef xor-spj at 0x1000b1: eax %d esi %d edx %d ebx %d", cpu.eax, cpu.esi, cpu.edx, cpu.ebx);
	// if(cpu.eip == 0x1000b1)
	// Log("xor before dest %d reg %s src %d reg %s", op_dest->val, REG_NAME(op_dest->reg), op_src->val, REG_NAME(op_src->reg));

	DATA_TYPE result = op_dest->val ^ op_src->val;
	OPERAND_W(op_dest, result);
	
	cpu.psw->CF = cpu.psw->OF = 0;

	//! (Cf & OF)
	testfor_flags_s(result, 0xFFF - 0x800 - 0x1);

	// if(cpu.eip == 0x1000b1)
	// Log("aft xor-spj at 0x1000b1: eax %d esi %d edx %d ebx %d", cpu.eax, cpu.esi, cpu.edx, cpu.ebx);
	// if(cpu.eip == 0x1000b1)
	// Log("xor after res %d", result);

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
