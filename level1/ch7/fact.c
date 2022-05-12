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

int main(void) {
	size_t nums[5] = { 5, 7, 36, 42643801, 2345726 };
	puts("Number : First prime factor\n");
	
	for(size_t i = 0; i < 5; i++) {
		printf("%zu : %zu\n", nums[i], wheel_fact_fst(nums[i]));
	}

	return EXIT_SUCCESS;
}
