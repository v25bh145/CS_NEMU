#ifndef __OPERAND_H__
#define __OPERAND_H__

enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };

#define OP_STR_SIZE 40

typedef struct {
	uint32_t type;
	size_t size;
	union {
		uint32_t reg;
		swaddr_t addr;
		uint32_t imm;
		int32_t simm;
	};
	uint32_t val;
	char str[OP_STR_SIZE];
	//use in some instr, such as jcc
	uint32_t abstract_instr_enum;
} Operand;

typedef struct {
	uint32_t opcode;
	bool is_operand_size_16;
	Operand src, dest, src2;
} Operands;

#endif