#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	OPERAND_W(op_dest, dest);

	int tmp = dest;
	int d = 0;
	while((tmp ^ 1) != 0) {
		tmp >>= 1;
		d++;
		if(tmp == 1)panic("awa");
		Log("%x", tmp);
	}
	Log("sar: d: %d dest: %d", d, dest);

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
