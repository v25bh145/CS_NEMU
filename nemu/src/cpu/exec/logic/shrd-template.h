#include "cpu/exec/template-start.h"

#define instr shrd

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {

	DATA_TYPE src2 = op_src2->val;
	DATA_TYPE dest = op_dest->val;

	uint8_t tmp = op_src->val;
	tmp &= 0x1f;
	while(tmp != 0) {
		src2 >>= 1;
		src2 |= (dest & 1) << ((DATA_BYTE << 3) - 1);
		dest >>= 1;
		tmp --;
	}

	OPERAND_W(op_src2, src2);

	print_asm_template2();
}

make_helper(concat(shrdi_, SUFFIX)) {
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
