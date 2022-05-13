#include "Simd/SimdLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define PC_MAX 800*600
#define UNSET USHRT_MAX
#define MERGE_DIST 30

		
typedef unsigned uf_elem;

/**init_uf_array sets up the initial unionfind array
 *
 * Assumes that the pixels of the image are 1-byte grayscale
 *
 * @param size (in) size total pixel count for image
 * @param uf   (out) the ith element of this array will be i; meaning the pixel is its own parent
 * @param stats (out) the ith element of this array will hold the number of elements in the set and the sum of their values
 */
void init_uf_arrays(size_t size, uf_elem uf[PC_MAX], unsigned stats[PC_MAX][2], uint8_t* img) {
	for(size_t i = 0; i < size; ++i) {
		uf[i] = i;
		stats[i][0] = 1;
		stats[i][1] = img[i];
	}
}

uf_elem find(size_t pc, uf_elem e, uf_elem uf[PC_MAX]) {
	if(uf[e] == e) return e;


	uf_elem root = e;
	while(uf[root] != root) {
		root = uf[root];
	}

	// compress tree
	while(uf[e] != root) {
		uint8_t parent = uf[e];
		uf[e] = root;
		e = parent;
	}

	return root;
}

void uf_union(size_t pc, uf_elem e1, uf_elem e2, uf_elem uf[PC_MAX], unsigned stats[PC_MAX][2]) {
	uf_elem r1 = find(pc, e1, uf);
	uf_elem r2 = find(pc, e2, uf);

	if(r1 == r2) return;

	if(stats[r1][0] >= stats[r2][0]) {
		uf[r2] = r1;
		stats[r1][0] += stats[r2][0];
		stats[r1][1] += stats[r2][1];
	} else {
		uf[r1] = r2;
		stats[r2][0] += stats[r1][0];
		stats[r2][1] += stats[r1][1];
	}
}

bool should_merge(size_t pc, uf_elem e1, uf_elem e2, uf_elem uf[PC_MAX], unsigned stats[PC_MAX][2]) {
	uf_elem r1 = find(pc, e1, uf);
	uf_elem r2 = find(pc, e2, uf);

	if(r1 == r2) return false;

	uf_elem avg1 = stats[r1][1] / stats[r1][0];
	uf_elem avg2 = stats[r2][1] / stats[r2][0];

	if(avg1 > avg2 && avg1 - avg2 <= MERGE_DIST) {
		return true;
	} else if(avg2 - avg1 <= MERGE_DIST) {
		return true;
	}

	return false;
}

void merge_all(size_t pc, size_t stride, uf_elem uf[PC_MAX], unsigned stats[PC_MAX][2], uint8_t* img) {
	unsigned merged = 1;
	while(merged > 0) {
		merged = 0;
		for(size_t i = 0; i < pc; i++) {
			size_t left = i % stride == 0 ? SIZE_MAX : i-1; 
			size_t top = i > stride ? i-stride : SIZE_MAX;
			if(left != SIZE_MAX && should_merge(pc, i, left, uf, stats)) {
				uf_union(pc, i, left, uf, stats);
				merged += 1;
			}

			if(top != SIZE_MAX && should_merge(pc, i, top, uf, stats)) {
				uf_union(pc, i, top, uf, stats);
				merged += 1;
			}
		}
	}
}

void uf_to_image(size_t pc, uf_elem uf[PC_MAX], unsigned stats[PC_MAX][2], uint8_t* newImg) {
	for(size_t i = 0; i < pc; i++) {
		uf_elem root = find(pc, i, uf);
		newImg[i] = stats[root][1];
	}
}

int main(void) {
	// set up parameters for image load routine
	size_t* stride = malloc(sizeof(size_t));
	size_t* width = malloc(sizeof(size_t));
	size_t* height = malloc(sizeof(size_t));
	SimdPixelFormatType fmt = SimdPixelFormatGray8;

	uint8_t* img = SimdImageLoadFromFile("sample-boat-400x300.png", stride, width, height, &fmt);
	if(img) {
		size_t pc = (*width)*(*height);
		if(pc > PC_MAX) {
			printf("image total pixels exceed maximum: %zu\n", pc);
			return EXIT_FAILURE;
		}
		uf_elem uf[PC_MAX] = { 0 };
		unsigned stats[PC_MAX][2] = { 0 };
		if(!uf) { perror("malloc uf"); return EXIT_FAILURE; }
		init_uf_arrays(pc, uf, stats, img);
		merge_all(pc, *stride, uf, stats, img);

		uint8_t *newImg = malloc(pc);
		uf_to_image(pc, uf, stats, newImg);
		SimdBool result = SimdImageSaveToFile(newImg, *stride, *width, *height, fmt, SimdImageFilePng, 0, "outImage.png");
		if(!result) { perror("save image"); return EXIT_FAILURE; }
	} else {
		perror("image failed to load");
		return EXIT_FAILURE;
	}

	SimdFree(img);
	free(stride);
	free(width);
	free(height);
	return EXIT_SUCCESS;
}
