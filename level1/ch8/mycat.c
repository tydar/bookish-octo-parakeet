#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

enum { buf_max = 32, };

int main(int argc, char* argv[argc+1]) {
	int ret = EXIT_FAILURE;
	char buffer[buf_max] = { 0 };
	for(int i = 1; i < argc; ++i) {
		FILE* instream = fopen(argv[i], "r");
		if(instream) {
			while(fgets(buffer, buf_max, instream)) {
				fputs(buffer, stdout);
			}
			fclose(instream);
			ret = EXIT_SUCCESS;
		} else {
			fprintf(stderr, "Could not open %s: ", argv[i]);
			perror(0);
			errno = 0;
		}
	}

	return ret;
}
