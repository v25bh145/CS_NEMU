#ifndef __POP_H__
#define __POP_H__

// for r32 and r16
make_helper(pop_r_pop_w);
make_helper(pop_r_pop_l);
make_helper(pop_m_w);
make_helper(pop_m_l);

make_helper(pop_r_pop_v);
make_helper(pop_rm_v);

#endif