#include "cpu/exec/template-start.h"

#define instr nop

static void do_execute() {
    print_asm_template1();
}

make_instr_helper(single);

#include "cpu/exec/template-end.h"