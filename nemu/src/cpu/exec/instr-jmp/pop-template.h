#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
    // if(cpu.eip == 0x100031)
    //     Log("spj pop 100031 %x", MEM_R(134216668));
    REG(op_dest->val) = MEM_R(cpu.esp);
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
    print_asm_template1();
}

make_instr_helper(r_pop);
make_instr_helper(rm);

#include "cpu/exec/template-end.h"