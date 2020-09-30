#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	swaddr_t addr;

} WP;

#endif
