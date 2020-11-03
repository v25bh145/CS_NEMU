#include "cpu/exec/template-start.h"

#define instr jcc

static void do_execute() {
    enum {JO, JNO, JB, JNB, JZ, JNZ, JBE, JNBE, JS, JNS, JP, JNP, JL, JNL, JLE, JNLE};
    DATA_TYPE_S rel = op_src->val;
    if(!strcmp(str(SUFFIX), "b")) {
        switch (op_src->abstract_instr_enum - 0x70) {
            case JO:
                if(cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JO");
            case JNO:
                if(!cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JNO");
            case JB:
                if(cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JB");
            case JNB:
                if(!cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JNB");
            case JZ:
                if(cpu.psw->ZF) 
                    cpu.eip += rel;
                break;
                // panic("JZ");
            case JNZ:
                if(!cpu.psw->ZF) 
                    cpu.eip += rel;
                break;
                // panic("JNZ");
            case JBE:
                if(cpu.psw->ZF || cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JBE");
            case JNBE:
                if(!cpu.psw->ZF && !cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JNBE");
            case JS:
                if(cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JS");
            case JNS:
                if(!cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JNS");
            case JNP:
                if(!cpu.psw->PF) 
                    cpu.eip += rel;
                break;
                // panic("JNP");
            case JL:
                if(cpu.psw->OF != cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JL");
            case JNL:
                if(cpu.psw->OF == cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JNL");
            case JP:
                if(cpu.psw->PF) 
                    cpu.eip += rel;
                break;
                // panic("JP");
            case JLE:
                if(cpu.psw->ZF || cpu.psw->SF != cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JLE");
            case JNLE:
                if(!cpu.psw->ZF && cpu.psw->SF == cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JNLE");
            case 0x79:
            case 0x7B:
                cpu.eip += rel;
                break;
                // panic("JMP");
            default:
                panic("UNKNOWN ERROR %d", op_src->abstract_instr_enum);
        }
    } else {
        switch (op_src->abstract_instr_enum - 0x80) {
            case JO:
                if(cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JO");
            case JNO:
                if(!cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JNO");
            case JB:
                if(cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JB");
            case JNB:
                if(!cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JNB");
            case JZ:
                if(cpu.psw->ZF) 
                    cpu.eip += rel;
                break;
                // panic("JZ");
            case JNZ:
                if(!cpu.psw->ZF) 
                    cpu.eip += rel;
                break;
                // panic("JNZ");
            case JBE:
                if(cpu.psw->ZF || cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JBE");
            case JNBE:
                if(!cpu.psw->ZF && !cpu.psw->CF) 
                    cpu.eip += rel;
                break;
                // panic("JNBE");
            case JS:
                if(cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JS");
            case JNS:
                if(!cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JNS");
            case JNP:
                if(!cpu.psw->PF) 
                    cpu.eip += rel;
                break;
                // panic("JNP");
            case JL:
                if(cpu.psw->OF != cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JL");
            case JNL:
                if(cpu.psw->OF == cpu.psw->SF) 
                    cpu.eip += rel;
                break;
                // panic("JNL");
            case JP:
                if(cpu.psw->PF) 
                    cpu.eip += rel;
                break;
                // panic("JP");
            case JLE:
                if(cpu.psw->ZF || cpu.psw->SF != cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JLE");
            case JNLE:
                if(!cpu.psw->ZF && cpu.psw->SF == cpu.psw->OF) 
                    cpu.eip += rel;
                break;
                // panic("JNLE");
            case 0x69:
                //jmp
                cpu.eip += rel;
                break;
            default:
                panic("UNKNOWN ERROR %d", op_src->abstract_instr_enum);
        }   
    }
    print_asm_template1();
}

make_instr_helper(i_condition);



#include "cpu/exec/template-end.h"