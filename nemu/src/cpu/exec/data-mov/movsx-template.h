#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {

    #if DATA_BYTE == 4
    DATA_TYPE ret = (int32_t)(op_src->val & 0xffff);
    #endif
    #if DATA_BYTE == 2
    DATA_TYPE ret = (int16_t)(op_src->val & 0xffff);
    #endif
    OPERAND_W(op_dest, ret);
    
	print_asm_template2();
}

make_instr_helper(rmb2r);
make_instr_helper(rmw2r);

#include "cpu/exec/template-end.h"