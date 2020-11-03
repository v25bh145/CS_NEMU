#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute() {
    // swaddr_write(cpu.eax, DATA_BYTE, MEM_R(cpu.edi));

    MEM_W(cpu.edi, cpu.eax);
    
    int incDec = 0;
    incDec = cpu.psw->DF ? -DATA_BYTE : DATA_BYTE;
	cpu.edi += incDec;

    print_asm_template1();
}

make_instr_helper(single);

#include "cpu/exec/template-end.h"