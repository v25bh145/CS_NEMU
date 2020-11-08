#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
    //op_dest val has been swap to val...
    //val2: op_src->val val1: op_dest->val
    DATA_TYPE result = op_dest->val - op_src->val;
    // Log("cmp res %d dest %d pos %d src %d pos %d with eip: %x", result, op_dest->val, op_dest->reg, op_src->val, op_src->reg, cpu.eip);
    // Log("spj: %d", MEM_R(cpu.ebp - 0x1008));
    cpu.psw->ZF = !result;
    cpu.psw->CF = op_dest -> val < op_src -> val;
    cpu.psw->SF = result >> ((DATA_BYTE << 3) - 1);
    int tmp1 = (op_dest -> val) >> ((DATA_BYTE << 3) - 1);
    int tmp2 = (op_src -> val) >> ((DATA_BYTE << 3) - 1);
    cpu.psw->OF = (tmp1 != tmp2 && tmp2 == cpu.psw->SF);
    // testfor_flags_s(result, 0xFFF - 0x01 - 0x800);

    // Log("after cmp: Z %d S %d O %d", cpu.psw->ZF, cpu.psw->SF, cpu.psw->OF);
    // Log("after eax: %d ebs: %d databyte %d", cpu.eax, cpu.ebx, DATA_BYTE);

    print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
