#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute() {
    // swaddr_write(cpu.eax, DATA_BYTE, MEM_R(cpu.edi));
    op_dest->val = cpu.eax;
    op_src->val = MEM_R(cpu.edi);
    int res = op_dest->val - op_src->val;

    int incDec = 0;
    incDec = cpu.psw->DF ? -DATA_BYTE : DATA_BYTE;
	cpu.edi += incDec;

    cpu.psw->CF = op_dest->val < op_src->val;

    testfor_flags_s(res, 0xFFF - 0x1);

    print_asm_template1();
}

make_instr_helper(single);

#include "cpu/exec/template-end.h"