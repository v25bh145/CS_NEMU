#include "trap.h"

int f[15];
int ans[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

int fact(int n) {
	if(n == 0 || n == 1) return 1;
	else return fact(n - 1) * n;
}

int main() {
	// int i;

	// for(i = 0; i < 13; i ++) {
		// f[i] = fact(i);
		// nemu_assert(f[i] == ans[i]);
	// }

	// nemu_assert(i == 13);

	f[10] = fact(10);
	nemu_assert(f[10] == ans[10]);

	return 0;
}

