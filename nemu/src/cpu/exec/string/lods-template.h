#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute() {
    // swaddr_write(cpu.eax, DATA_BYTE, MEM_R(cpu.esi));
    cpu.eax = MEM_R(reg_l(R_ESI));

    int incDec = 0;
    incDec = cpu.psw->DF ? -DATA_BYTE : DATA_BYTE;
	cpu.esi += incDec;

    print_asm_template1();
}

make_instr_helper(single);

#include "cpu/exec/template-end.h"