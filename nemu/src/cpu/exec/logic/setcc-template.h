#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute() {
    enum {SETO, SETNO, SETB, SETNB, SETZ, SETNZ, SETBE, SETNBE, 
                SETS, SETNS, SETP, SETNP, SETL, SETNL, SETLE, SETNLE };
    REG(op_src->reg) = 0;
    switch (op_src->abstract_instr_enum - 0x90) {
        case SETO:
            if(cpu.psw->OF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETO");
        case SETNO:
            if(!cpu.psw->OF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNO");
        case SETB:
            if(cpu.psw->CF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETB");
        case SETNB:
            if(!cpu.psw->CF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNB");
        case SETZ:
            if(cpu.psw->ZF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETZ");
        case SETNZ:
            if(!cpu.psw->ZF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNZ");
        case SETBE:
            if(cpu.psw->ZF || cpu.psw->CF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETBE");
        case SETNBE:
            if(!cpu.psw->ZF && !cpu.psw->CF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNBE");
        case SETS:
            if(cpu.psw->SF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETS");
        case SETNS:
            if(!cpu.psw->SF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNS");
        case SETNP:
            if(!cpu.psw->PF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNP");
        case SETL:
            if(cpu.psw->OF != cpu.psw->SF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETL");
        case SETNL:
            if(cpu.psw->OF == cpu.psw->SF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNL");
        case SETP:
            if(cpu.psw->PF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETP");
        case SETLE:
            if(cpu.psw->ZF || cpu.psw->SF != cpu.psw->OF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETLE");
        case SETNLE:
            if(!cpu.psw->ZF && cpu.psw->SF == cpu.psw->OF) 
                REG(op_src->reg) = 1;
            break;
            // panic("SETNLE");
        default:
            panic("UNKNOWN ERROR %d", op_src->abstract_instr_enum);
    }
    print_asm_template1();
}

make_instr_helper(rm_condition);



#include "cpu/exec/template-end.h"