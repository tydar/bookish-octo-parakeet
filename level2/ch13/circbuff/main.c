#include "circular.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

void print_circular(circular* c, int verbose) {
	size_t len = circular_getlength(c);
	if(verbose) printf("Len: %zu\n", len);
	printf("[ ");
	for (size_t i = 0; i < len; i++) {
		double* elem = circular_element(c, i);
		printf("%g ", *elem);
	}

	printf("]\n");
}

int main(void) {
	circular* c = circular_new(5);

	for (size_t i = 0; i < 5; i++) {
		circular_append(c, i*1.0);
	}

	print_circular(c, 1);

	c = circular_resize(c, 6);

	if (c) {
		circular_append(c, 5.0);
	}

	print_circular(c, 1);

	double val = circular_pop(c);
	printf("Popped: %g\n", val);

	print_circular(c, 1);

	c = circular_resize(c, 5);
	if (c) {
		print_circular(c, 1);
	} else {
		puts("Downsize failed!");
	}

	return EXIT_SUCCESS;
}
