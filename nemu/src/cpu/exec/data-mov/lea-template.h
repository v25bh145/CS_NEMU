#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute() {
    // Log("addr %d reg %s spj %d eax %d", op_dest->addr, REG_NAME(op_src->reg), cpu.ebp, cpu.eax);
    REG(op_src->reg) = op_dest->addr;
	print_asm_template2();
}

make_instr_helper(addr2r)

#include "cpu/exec/template-end.h"
