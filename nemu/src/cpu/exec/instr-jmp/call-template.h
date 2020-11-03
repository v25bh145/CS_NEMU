#include "cpu/exec/template-start.h"

#define instr call

// static void do_execute() {
//     cpu.esp -= DATA_BYTE;

//     MEM_W(cpu.esp, DATA_BYTE  + cpu.eip); // There is no "+1"

//     cpu.eip += op_src->val;

//     Log("after call eip: %x", cpu.eip + DATA_BYTE + 1);
//     print_asm_template1();
// }
make_helper(concat(call_i_, SUFFIX)) {
    int len = concat(decode_i_, SUFFIX) (eip + 1);

    cpu.esp -= DATA_BYTE;

    MEM_W(cpu.esp, cpu.eip  + len); // There is no "+1"

    cpu.eip += op_src->val;

    print_asm_template1();

    return len + 1;
}
make_helper(concat(call_rm_, SUFFIX)) {
    int len = concat(decode_rm_, SUFFIX) (eip + 1);

    cpu.esp -= DATA_BYTE;

    MEM_W(cpu.esp, cpu.eip  + len); // There is no "+1"

    cpu.eip = op_src->val - len - 1;

    print_asm_template1();

    return len + 1;
}

// make_instr_helper(i);
// make_instr_helper(rm);

#undef GET_TYPE

#include "cpu/exec/template-end.h"