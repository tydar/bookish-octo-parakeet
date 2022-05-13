
segm:
	c99 -Werror -o segm segm.c -I$(CURDIR)/simd/src/ -L$(CURDIR)/simd/build/ -lm -lstdc++ -pthread -lSimd

segm-debug:
	c99 -g -Werror -o segm segm.c -I$(CURDIR)/simd/src/ -L$(CURDIR)/simd/build/ -lm -lstdc++ -pthread -lSimd

clean: 
	rm segm
