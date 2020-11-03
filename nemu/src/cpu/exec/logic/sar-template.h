#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	if(cpu.eip == 0x100167) 
		Log("tmp: eax(dt) %d", cpu.eax);
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	OPERAND_W(op_dest, dest);
	if(cpu.eip == 0x100167) 
		Log("sar mid: eax(dt) %d", dest);
	DATA_TYPE tmp = dest;
	cpu.psw->CF = 0;
	cpu.psw->OF = 0;
	cpu.psw->ZF = !tmp;
    cpu.psw->SF = tmp >> ((DATA_BYTE << 3) - 1);
	tmp ^= tmp >> 4;
    tmp ^= tmp >> 2;
    tmp ^= tmp >> 1;
    tmp &= 1;
    cpu.psw->PF = !tmp;
	if(cpu.eip == 0x100167) 
		Log("tmp2: eax(dt) %d", cpu.eax);
	
	//ZF PF SF
	// testfor_flags_s(dest, 0x40 + 0x80 + 0x4);

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
