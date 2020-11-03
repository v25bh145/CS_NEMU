#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
    int plus = 0;
    if(instr_fetch(cpu.eip, 1) == 0xc2)
        plus = op_src->imm;
    cpu.eip = instr_fetch(cpu.esp, 4);

    switch(DATA_BYTE) {
        case 2:
            cpu.esp += 2;
            break;
        case 4:
            cpu.esp += 4;
            break;
        default:
            panic("ERROR in pop");
    }
    cpu.esp += plus;
    cpu.eip = cpu.eip + ( plus ? -2 : 0);

    print_asm_template1();
}

#if DATA_BYTE == 4
make_instr_helper(single);
#endif

make_instr_helper(i);


#include "cpu/exec/template-end.h"