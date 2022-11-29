#include "tree.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

struct node {
	RB_ENTRY(node) entry;
	int key, val;
};
RB_HEAD(int_assoc, node);

int keycmp(struct node* e1, struct node* e2) {
	return (e1->key < e2->key ? -1 : e1->key > e2->key);
}

RB_PROTOTYPE(int_assoc, node, entry, keycmp)
RB_GENERATE(int_assoc, node, entry, keycmp)

int int_assoc_insert(struct int_assoc* head, int key, int value) {
	struct node* n = malloc(sizeof(struct node));
	if (n == NULL) return 1;
	n->key = key;
	n->val = value;

	if (RB_INSERT(int_assoc, head, n) == NULL) return 0;

	/* means we had a duplicate */
	return 1;
}

int int_assoc_update(struct int_assoc* head, int key, int value) {
	struct node find;
	struct node* result;
	find.key = key;

	result = RB_FIND(int_assoc, head, &find);

	/* no match found; insert */
	if (result == NULL) return int_assoc_insert(head, key, value);

	/* found a match; update its value */
	result->val = value;
	return 0;
}

int int_assoc_delete(struct int_assoc* head, int key) {
	struct node find;
	struct node* result;
	find.key = key;

	result = RB_FIND(int_assoc, head, &find);

	/* no such key found */
	if (result == NULL) return 1;

	result = RB_REMOVE(int_assoc, head, &find);

	/* remove failed */
	if (result == NULL) return 1;

	free(result);
	return 0;
}

struct node* int_assoc_get(struct int_assoc* head, int key) {
	struct node find;
	struct node* result;
	find.key = key;

	result = RB_FIND(int_assoc, head, &find);

	return result;
}

int main(int argc, char** argv) {
	struct int_assoc arr = RB_INITIALIZER(&arr);

	int keys[] = { 5, 10, 15, 20, 25 };
	int values[] = { 25, 100, 225, 400, 625 };

	for (size_t i = 0; i < 5; i++) {
		if(int_assoc_insert(&arr, keys[i], values[i]) != 0) {
			exit(1);
		}
	}

	printf("Sorted key-value pairs\n");
	struct node* iter;
	RB_FOREACH(iter, int_assoc, &arr) {
		printf("(%d, %d)\n", iter->key, iter->val);
	}

	return 0;
}
