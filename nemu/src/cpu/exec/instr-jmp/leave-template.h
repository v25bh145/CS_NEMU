#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute() {
    //ebp & target :
    //target targetaddr
    //ebp eax
    //now want esp turn to targetaddr
    //so I should first plus ebp then use esp.
    cpu.esp = cpu.ebp;
    cpu.ebp = MEM_R(cpu.esp);
    cpu.esp += 4;
    
    print_asm_template1();
}

make_instr_helper(single);

#include "cpu/exec/template-end.h"