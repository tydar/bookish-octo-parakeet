#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void initTable(size_t len, size_t Table[len]) {
	for(size_t i = 0; i < len; i++) {
		Table[i] = SIZE_MAX;
	}
}

size_t Find(size_t len, size_t elem, size_t Table[len]) {
	size_t root = elem;
	
	while(Table[root] != SIZE_MAX) {
		root = Table[root];
	}

	return root;
}

void FindReplace(size_t len, size_t elem, size_t new, size_t Table[len]) {
	// if we're passed an element not in the table, return
	if (elem > len || (new > len && len != SIZE_MAX)) return;

	size_t next = Table[elem];
	Table[elem] = new;
	while(next != SIZE_MAX) {
		size_t temp = Table[next];
		Table[next] = new;
		next = temp;
	}
}

void FindCompress(size_t len, size_t elem, size_t Table[len]) {
	// I am going to walk the tree twice for this function
	// There is a more efficient way but it would be a space
	// complexity tradeoff.
	//
	// Also assuming this means "all parents *with the same root* as IDed"
	//
	// TODO: I think I *may* have been incorrect. Instead of setting *all* nodes
	// 	 on the path to the root to be children of the root. Set *only* the indicated
	// 	 node's parent to be its root.
	
	const size_t root = Find(len, elem, Table);

	for (size_t i = 0; i < len; ++i) {
		if(i != root && Find(len, i, Table) == root) {
			Table[i] = root;
		}
	}
}

void Union(size_t len, size_t left, size_t right, size_t Table[len]) {
	FindCompress(len, left, Table);
	const size_t newRoot = Find(len, left, Table);
	FindReplace(len, right, newRoot, Table);
}

void PrintPartition(size_t len, size_t elem, size_t Table[len]) {
	const size_t root = Find(len, elem, Table);
	printf("{ %zu ", root);
	for(size_t i = 0; i < len; ++i) {
		if (i != root && Find(len, i, Table) == root) {
			printf("%zu ", i);
		}
	}
	puts("}\n");
}

int main(void) {
	const size_t len = 10;
	size_t Table[len];
	initTable(len, Table);
	Table[1] = 0;
	Table[2] = 1;
	Table[3] = 2;

	Table[4] = 5;
	Table[6] = 5;

	puts("Partition 0: ");
	PrintPartition(len, 0, Table);
	puts("Partition 5: ");
	PrintPartition(len, 5, Table);

	Union(len, 5, 0, Table);
	PrintPartition(len, 0, Table);

	return EXIT_SUCCESS;
}
