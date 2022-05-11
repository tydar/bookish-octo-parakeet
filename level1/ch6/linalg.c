#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

double dotprod(size_t len, double A[len], double B[len]){
	double collector = 0.0;
	for(size_t i = 0; i < len; ++i) {
		collector += A[i]*B[i];
	}
	return collector;
}

void transform(size_t n, size_t m, double matrix[n][m], double vec[n], double result[m]) {
	for(size_t i = 0; i < m; ++i) {
		result[i] = 0.0;
		for(size_t j = 0; j < n; ++j) {
			result[i]  += matrix[j][i]*vec[j];
		}
	}
}

int main(void) {
	double A[3] = {1.0, 2.0, 1.0};
	double B[3] = {2.0, 3.0, 3.0};

	printf("A dot B = %g\n", dotprod(3, A, B));

	double V[3] = {14.3, 12.78, 6.2};
	double M[3][3] = {
		{1, 0, 0},
		{0, 0, -1},
		{0, 1, 0},
	};

	puts("V X M = \n");

	double rot[3];
	transform(3, 3, M, V, rot);

	printf("{ %g %g %g }\n", rot[0], rot[1], rot[2]);

	return EXIT_SUCCESS;
}
