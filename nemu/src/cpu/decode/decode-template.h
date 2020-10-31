#include "cpu/exec/template-start.h"

#include "cpu/decode/modrm.h"

#define decode_r_internal concat3(decode_r_, SUFFIX, _internal)
#define decode_rm_internal concat3(decode_rm_, SUFFIX, _internal)
#define decode_i concat(decode_i_, SUFFIX)
#define decode_a concat(decode_a_, SUFFIX)
#define decode_r2rm concat(decode_r2rm_, SUFFIX)

/* Ib, Iv */
make_helper(concat(decode_i_, SUFFIX)) {
	/* eip here is pointing to the immediate */
	op_src->type = OP_TYPE_IMM;
	op_src->imm = instr_fetch(eip, DATA_BYTE);
	op_src->val = op_src->imm;

#ifdef DEBUG
	snprintf(op_src->str, OP_STR_SIZE, "$0x%x", op_src->imm);
#endif
	return DATA_BYTE;
}

#if DATA_BYTE == 1 || DATA_BYTE == 4
/* sign immediate */
make_helper(concat(decode_si_, SUFFIX)) {
	op_src->type = OP_TYPE_IMM;

	/* TODO: Use instr_fetch() to read `DATA_BYTE' bytes of memory pointed
	 * by `eip'. Interpret the result as an signed immediate, and assign
	 * it to op_src->simm.
	 *
	op_src->simm = ???
	 */
	// panic("please impleme me %d", instr_fetch(eip, DATA_BYTE));
	op_src->simm = instr_fetch(eip, DATA_BYTE);
	op_src->val = op_src->simm;

#ifdef DEBUG
	snprintf(op_src->str, OP_STR_SIZE, "$0x%x", op_src->val);
#endif
	return DATA_BYTE;
}
#endif

/* eAX */
static int concat(decode_a_, SUFFIX) (swaddr_t eip, Operand *op) {
	op->type = OP_TYPE_REG;
	op->reg = R_EAX;
	op->val = REG(R_EAX);

#ifdef DEBUG
	snprintf(op->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
#endif
	return 0;
}

/* eXX: eAX, eCX, eDX, eBX, eSP, eBP, eSI, eDI */
static int concat3(decode_r_, SUFFIX, _internal) (swaddr_t eip, Operand *op) {
	op->type = OP_TYPE_REG;
	op->reg = ops_decoded.opcode & 0x7;
	op->val = REG(op->reg);

#ifdef DEBUG
	snprintf(op->str, OP_STR_SIZE, "%%%s", REG_NAME(op->reg));
#endif
	return 0;
}

static int concat3(decode_rm_, SUFFIX, _internal) (swaddr_t eip, Operand *rm, Operand *reg) {
	rm->size = DATA_BYTE;
	int len = read_ModR_M(eip, rm, reg);
	reg->val = REG(reg->reg);

#ifdef DEBUG
	snprintf(reg->str, OP_STR_SIZE, "%%%s", REG_NAME(reg->reg));
#endif
	return len;
}

/* Eb <- Gb
 * Ev <- Gv
 */
make_helper(concat(decode_r2rm_, SUFFIX)) {
	return decode_rm_internal(eip, op_dest, op_src);
}

/* Gb <- Eb
 * Gv <- Ev
 */
make_helper(concat(decode_rm2r_, SUFFIX)) {
	return decode_rm_internal(eip, op_src, op_dest);
}


/* AL <- Ib
 * eAX <- Iv
 */
make_helper(concat(decode_i2a_, SUFFIX)) {
	decode_a(eip, op_dest);
	return decode_i(eip);
}

/* Gv <- EvIb
 * Gv <- EvIv
 * use for imul */
make_helper(concat(decode_i_rm2r_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_src2, op_dest);
	len += decode_i(eip + len);
	return len;
}

/* Eb <- Ib
 * Ev <- Iv
 */
make_helper(concat(decode_i2rm_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_dest, op_src2);		/* op_src2 not use here */
	len += decode_i(eip + len);
	return len;
}

/* XX <- Ib 
 * eXX <- Iv 
 */
make_helper(concat(decode_i2r_, SUFFIX)) {
	decode_r_internal(eip, op_dest);
	return decode_i(eip);
}

/* used by unary operations */
make_helper(concat(decode_rm_, SUFFIX)) {
	return decode_rm_internal(eip, op_src, op_src2);		/* op_src2 not use here */
}

make_helper(concat(decode_r_, SUFFIX)) {
	return decode_r_internal(eip, op_src);
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(decode_si2rm_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_dest, op_src2);	/* op_src2 not use here */
	len += decode_si_b(eip + len);
	return len;
}

make_helper(concat(decode_si_rm2r_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_src2, op_dest);
	len += decode_si_b(eip + len);
	return len;
}
#endif

/* used by shift instructions */
make_helper(concat(decode_rm_1_, SUFFIX)) {
	int len = decode_r2rm(eip);
	op_src->type = OP_TYPE_IMM;
	op_src->imm = 1;
	op_src->val = 1;
#ifdef DEBUG
	sprintf(op_src->str, "$1");
#endif
	return len;
}

make_helper(concat(decode_rm_cl_, SUFFIX)) {
	int len = decode_r2rm(eip);
	op_src->type = OP_TYPE_REG;
	op_src->reg = R_CL;
	op_src->val = reg_b(R_CL);
#ifdef DEBUG
	sprintf(op_src->str, "%%cl");
#endif
	return len;
}

make_helper(concat(decode_rm_imm_, SUFFIX)) {
	int len = decode_r2rm(eip);
	len += decode_i_b(eip + len);
	return len;
}

//for myself
make_helper(concat(decode_i_condition_, SUFFIX)) {
	//TODO for16/32
    if(!strcmp(str(SUFFIX), "b")) {
		op_src->abstract_instr_enum = MEM_R(eip - 1);
		int len = concat(decode_i_, SUFFIX)(eip);
		return len;
    } else {
		panic("please implement 16/32 reg for jcc!");
	}
}
make_helper(concat(decode_rm_condition_, SUFFIX)) {
	//TODO for16/32
    if(!strcmp(str(SUFFIX), "b")) {
		op_src->abstract_instr_enum = MEM_R(eip - 1);
		int len = concat(decode_rm_, SUFFIX)(eip);
		return len;
    } else {
		panic("please implement 16/32 reg for setcc!");
	}
}
make_helper(concat(decode_r_pop_, SUFFIX)) { 
	op_dest->type = OP_TYPE_REG;
	op_dest->val = instr_fetch(eip - 1, 1) - 0x58;
	int len = concat(decode_r_, SUFFIX)(eip);
	return len;
}
make_helper(concat(decode_single_, SUFFIX)) {
	return 0;
}
make_helper(concat(decode_addr2r_, SUFFIX)) {
	// panic("%x", eip);
	ModR_M m;
	m.val = instr_fetch(eip, 1);
	op_src->reg = OP_TYPE_REG;
	op_src->reg = m.reg;
	int instr_len = load_addr(eip, &m, op_dest);
	return instr_len;
}
make_helper(concat(decode_rmb2r_,SUFFIX)) {
	op_src->size = 1;
	int len_addr = read_ModR_M(eip, op_src, op_dest);
	return len_addr;
}
make_helper(concat(decode_rmw2r_,SUFFIX)) {
	// panic("len: %x", eip - 1);
	op_src->size = 2;
	int len_addr = read_ModR_M(eip - 1, op_src2, op_dest);
	panic("len: %d", len_addr);
	//op_src
	len_addr += concat(decode_r_, SUFFIX) (eip + len_addr);
	return len_addr;
}

void concat(write_operand_, SUFFIX) (Operand *op, DATA_TYPE src) {
	if(op->type == OP_TYPE_REG) { REG(op->reg) = src; }
	else if(op->type == OP_TYPE_MEM) { swaddr_write(op->addr, op->size, src); }
	else { assert(0); }
}

//myself
void concat(testfor_flags_s_, SUFFIX) (DATA_TYPE_S result, uint32_t type) {
	//type for:
	//bit      11       10         09	      08	    07	   06      04     02    00
	//reg     OF       DF	     IF	        TF	      SF     ZF      AF      PF  CF
	//code 0x800 0x400 0x200 0x100 0x80 0x40 0x10 0x4 0x1
    if(op_dest->val > 0 && op_src->val < 0 && result < 0) {
		if((type & 0x1) != 0)
        	cpu.psw->CF = 1;
		if((type & 0x80) != 0)
			cpu.psw->SF = 1;
		if((type & 0x800) != 0)
        	cpu.psw->OF = 1;
    } else if (op_dest->val < 0 && op_src->val > 0 && result > 0) {
		if((type & 0x1) != 0)
        	cpu.psw->CF = 0;
		if((type & 0x80) != 0)
			cpu.psw->SF = 0;
		if((type & 0x800) != 0)
        	cpu.psw->OF = 1;
    } else if (!result) {
		if((type & 0x40) != 0)
        	cpu.psw->ZF = 1;
    } else {
		if((type & 0x40) != 0)
        	cpu.psw->ZF = 0;
		if((type & 0x1) != 0)
        	cpu.psw->CF = result < 0;
		if((type & 0x80) != 0)
			cpu.psw->SF = result < 0;
    }
	DATA_TYPE cnt = 0, tmp = result, tmp2 = op_dest->val;
	if((type & 0x4) != 0) {
		while( ( tmp & 1 ) > 0 && tmp > 0) {
			cnt += ( tmp & 1 );
			tmp = tmp >> 1;
		}
		cpu.psw->PF = cnt % 2 ? 0 : 1;
	}
	// cpu.psw->CF = ( ( result | op_dest->val ) - op_dest->val ) > 0;
	if((type & 0x10) != 0) {
		tmp = result & 0xF;
		tmp2 = op_dest->val & 0xF;
		cpu.psw->AF = ( ( tmp | tmp2 ) - tmp2) > 0;
	}
}

// void concat(testfor_flags_, SUFFIX) (DATA_TYPE result) {
//     if(op_src->val > 0 && op_dest->val < 0 && result < 0) {
//         cpu.psw->SF = 1;
//         cpu.psw->OF = 1;
//     } else if (op_src->val < 0 && op_dest->val > 0 && result > 0) {
//         cpu.psw->SF = 0;
//         cpu.psw->OF = 1;
//     } else if (!result) {
//         cpu.psw->ZF = 1;
//     } else {
//         cpu.psw->SF = result < 0;
//     }
// 	DATA_TYPE cnt = 0, tmp = result, tmp2 = op_dest->val;
// 	while( ( tmp & 1 ) > 0 && tmp > 0) {
// 		cnt += ( tmp & 1 );
// 		tmp = tmp >> 1;
// 	}
// 	cpu.psw->PF = cnt % 2 ? 0 : 1;
// 	// cpu.psw->CF = ( ( result | op_dest->val ) - op_dest->val ) > 0;
// 	tmp = result & 0xF;
// 	tmp2 = op_dest->val & 0xF;
// 	cpu.psw->AF = ( ( tmp | tmp2 ) - tmp2) > 0;
// }

#include "cpu/exec/template-end.h"
