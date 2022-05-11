#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

double F(double x) {
	return x*x*x+1;
}

double f(double x) {
	double h = sqrt(2.E-16)*x;
	return (F(x+h) - F(x-h))/(2*h);
}

int main(void) {
	printf("f'(0.5)=%g\tf'(1)=%g\tf'(5)=%g\n",
			f(0.5), f(1.0), f(5.0));
	return EXIT_SUCCESS;
}
