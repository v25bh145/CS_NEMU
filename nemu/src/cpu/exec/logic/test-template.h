#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
    DATA_TYPE result = op_dest->val & op_src->val;
    // Log("cmp res %d dest %d pos %d src %d pos %d with eip: %x", result, op_dest->val, op_dest->reg, op_src->val, op_src->reg, cpu.eip);
    cpu.psw->ZF = !result;
    cpu.psw->CF = 0;
    cpu.psw->SF = result >> ((DATA_BYTE << 3) - 1);
    result ^= result >> 4;
    result ^= result >> 2;
    result ^= result >> 1;
    result &= 1;
    cpu.psw->PF = !result;
    print_asm_template2();
}

make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"