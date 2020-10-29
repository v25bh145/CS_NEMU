#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sub.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/test.h"
#include "logic/cmp.h"

#include "instr-jmp/call.h"
#include "instr-jmp/push.h"
#include "instr-jmp/jcc.h"
#include "instr-jmp/pop.h"
#include "instr-jmp/ret.h"
#include "instr-jmp/leave.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"
