#include <stdio.h>
#include <stdlib.h>

int check_sort(int len, double A[len]) {
	for(size_t i = 0; i < len-1; ++i) {
		if(A[i] > A[i+1]) {
			return 1;
		}
	}
	return 0;
}

int main(void) {
	double A[5] = {
		[1] = 1,
		[2] = 3,
		[3] = 5,
		[4] = 2,
	};

	printf("A[] sort status: %d \n",
			check_sort(5, A));
	return EXIT_SUCCESS;
}
