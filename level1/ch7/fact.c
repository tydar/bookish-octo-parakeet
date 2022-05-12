#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool divCheck(size_t n, size_t k) {
	return (n % k) == 0;
}

/**wheel_fact_fst consumes a positive integer and returns its smallest prime factor
 *
 * @param n a positive integer
 * @invariant 0 < n <= SIZE_MAX
 * @return the smallest prime factor of n
 */
size_t wheel_fact_fst(size_t n) {
	if(n == 1 || n == 0) return n;
	if(divCheck(n, 2)) return 2;
	if(divCheck(n, 3)) return 3;
	if(divCheck(n, 5)) return 5;

	size_t inc[8] = { 4, 2, 4, 2, 4, 6, 2, 6 };

	size_t k = 7;
	size_t i = 0;

	while(k*k <= n) {
		bool kDividesN = divCheck(n, k);
		size_t fstFactK = wheel_fact_fst(k);
		if(kDividesN) {
			return fstFactK;
		} 

		k += inc[i];
		i = i < 8 ? i + 1 : 1;
	}

	return n;
}

int main(int argc, char* argv[argc]) {
	puts("Number : First prime factor\n");
	
	for(size_t i = 1; i < argc; i++) {
		unsigned long n = strtoul(argv[i], NULL, 10);

		// not sure if this cast is exactly legit
		printf("%zu : %zu\n", n, wheel_fact_fst(n));
	}

	return EXIT_SUCCESS;
}
