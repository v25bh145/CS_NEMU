#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;
static int current = 0;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].next = NULL;
		wp_pool[i].addr = 0;
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* get_wp_head () {
	return head;
}
void printAllPool () {
	int count = 0;
	WP* h;
	for(h = head; h != NULL; h = h->next) {
		count++;
		Log("count%d: in addr 0x%x ", count, h->addr);
	}
}

int setWatchpoint(uint32_t step) {
	current++;
	wp_pool[current - 1].addr = step;
	if(head == NULL) {
		head = &wp_pool[current - 1];
	} else if (head->addr > step) {
		wp_pool[current - 1].next = head;
		head = &wp_pool[current - 1];
	} else {
		//sort
		WP* h;
		for(h = head; h != NULL; h = h->next) {
			if(h == NULL) break;
			WP* nex = h->next;
			if(nex == NULL) {
				//insert into the last one
				h->next = &wp_pool[current - 1]; 
				break;
			} else {
					if(nex->addr > wp_pool[current - 1].addr) {
						//insert into the pre of h2
						h->next = &wp_pool[current - 1];
						wp_pool[current - 1].next = nex;
						break;
					} else {
						continue;
					}
			}
		}
	}
	return 0;
}
int delWatchpoint(int count) {
	Log("%d + %d", current, count);
	if(current < count || count <= 0) {
		Log("error: no such wp to del.");
		Log("help: d N");
		return 1;
	} else if(count == 1) {
		//delete head(soft delete for it is a static Array)
		head->addr = 0;
		current--;

		WP* tmpWp = head;
		head = head->next;
		tmpWp->next = NULL;
	} else {
		int tmpCount = 0;
		WP* h;
		for(h = head; h != NULL; h = h->next) {
			tmpCount++;
			if(tmpCount + 1 == count) {
				//delete h->next(soft delete for it is a static Array)
				h->next->addr = 0;
				current--;

				WP* tmpWp = h->next;
				h->next = h->next->next;
				tmpWp->next = NULL;
				return 0;
			}
		}
		Log("error: no such wp to del.");
		Log("help: d N");
		return 1;
	}
	return 0;
}

int wpSearch(uint32_t eip, int len) {
		WP* h;
		int count;
		for(h = head; h != NULL; h = h->next) {
			count++;
			if(h->addr < eip && h->addr >= eip - len) {
				return count;
			}
			if(h->addr > eip) {
				return 0;
			}
		}
		return 0;
}