#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
    //TODO: exam if MEM correct
    // Log("brfore push: %d %d", cpu.esp, MEM_R(cpu.esp));
    if(op_src->type == OP_TYPE_REG) {
        cpu.esp -= DATA_BYTE;
        // MEM_W(cpu.esp - DATA_BYTE, op_src->val);
        MEM_W(cpu.esp, op_src->val);
    } else if(op_src->type == OP_TYPE_MEM) {
        cpu.esp -= DATA_BYTE;
        // MEM_W(cpu.esp - DATA_BYTE, MEM_R(op_src->val));
        MEM_W(cpu.esp, op_src->val);
    } else if(op_src->type == OP_TYPE_IMM) {
        cpu.esp -= DATA_BYTE;
        // MEM_W(cpu.esp - DATA_BYTE, op_src->val);
        MEM_W(cpu.esp, op_src->val);
    }
    // Log("after push: %d %d ebp %d %d", cpu.esp, MEM_R(cpu.esp), cpu.ebp, MEM_R(cpu.ebp));
    // panic("rel32"str(SUFFIX)": %d", REG(op_src->val));
    print_asm_template1();
}

make_instr_helper(r);
make_instr_helper(i);

#include "cpu/exec/template-end.h"