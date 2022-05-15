#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// can't do pointer arithmetic on void* so have to use memcpy
// see https://stackoverflow.com/q/29596151
void swap(void* a, void* b, size_t size){
	void *tmp = malloc(size);

	memcpy(tmp, b, size);
	memcpy(b, a, size);
	memcpy(a, tmp, size);

	free(tmp);
}

void my_sort(void* base, size_t nmemb, size_t size, int(*compar)(const void*, const void*)) {
	for(size_t i = 1; i < nmemb; i++) {
		for(size_t j = i; j > 0 && compar(base+((j-1) * size), base+(j * size)) > 0; j--){
			swap(base+((j-1) * size), base+(j*size), size);
		}
	}
}

int charcomp(void const* a, void const* b) {
	char const* A = a;
	char const* B = b;

	if (*A > *B) return 1;
	if (*A == *B) return 0;
	return -1;
}

int intcomp(void const* a, void const* b) {
	int const* A = a;
	int const* B = b;

	if (*A > *B) return 1;
	if (*A == *B) return 0;
	return -1;
}

int main(void) {
	char A[5] = { 3, 0, 2, 4, 7 };

	my_sort(A, 5, sizeof(A[0]), charcomp);

	printf("\n[ ");
	for(size_t i = 0; i < 5; i++) {
		printf("%u ", A[i]);
	}
	printf("]\n");

	int B[5] = { 100, -10, 23, 67, 91 };
	my_sort(B, 5, sizeof(B[0]), intcomp);

	printf("\n[ ");
	for(size_t i = 0; i < 5; i++) {
		printf("%d ", B[i]);
	}
	printf("]\n");

	return EXIT_SUCCESS;
}
