#include <stdio.h>
#include <stdlib.h>

int main(void) {
	FILE* logfile = fopen("mylog.txt", "wx");
	if(!logfile) {
		perror("fopen failed");
		return EXIT_FAILURE;
	}

	fputs("*nodding sagely* file output\n", logfile);
	return EXIT_SUCCESS;
}
