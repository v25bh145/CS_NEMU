#include "cpu/exec/template-start.h"

#define instr cwdcdq

static void do_execute() {
    REG(R_EDX) = -(MSB(REG(R_EAX)));
	print_asm_template1();
}

make_instr_helper(single)

#include "cpu/exec/template-end.h"
