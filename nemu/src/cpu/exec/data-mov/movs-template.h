#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
    // swaddr_write(reg_l(R_EDI),DATA_BYTE,MEM_R(reg_l(R_ESI)));
    swaddr_write(cpu.edi, DATA_BYTE, MEM_R(cpu.esi));
    int incDec = 0;
    incDec = cpu.psw->DF ? -DATA_BYTE : DATA_BYTE;
	cpu.esi += incDec;
    cpu.edi += incDec;
	print_asm_template1();
}

make_instr_helper(single);

#include "cpu/exec/template-end.h"
