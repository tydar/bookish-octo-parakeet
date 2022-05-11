#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define GSIZE 5
#define QSIZE GSIZE+1

/** @file */


/** enqueue adds an element to the front of a fixed-size queue.
 *
 * @param queue[QSIZE] is the queue
 * @param val is the value to enqueue
 * @invariant queue must be an array of size QSIZE
 * @return true if the value was enqueued
 */
bool enqueue(size_t queue[QSIZE], size_t val) {
	size_t index = SIZE_MAX;
	for(size_t i = 0; i < QSIZE-1; ++i) {
		if(queue[i] == SIZE_MAX) {
			index = i;
			break;
		}
	}

	if(index == SIZE_MAX) return false;

	queue[index] = val;
	queue[index+1] = SIZE_MAX;
	return true;
}

void printqueue(size_t queue[QSIZE]) {
	printf("Queue: [ ");
	for(size_t i = 0; i < QSIZE; ++i) {
		printf("%zu: %zu ", i, queue[i]);
	}
	printf("]\n");
}

/** dequeue removes an element from the front of the queue
 *
 * @param queue[QSIZE] is the queue
 * @invariant queue must be an array of size QSIZE
 * @return the value of the head element. if SIZE_MAX, means empty queue
 */
size_t dequeue(size_t queue[QSIZE]) {
	size_t retval = queue[0];
	for(size_t i = 0; i < QSIZE-1; ++i) {
		// if we hit the sentry value we are done moving elements up
		if(queue[i] == SIZE_MAX) break;
		queue[i] = queue[i+1];
	}

	return retval;
}

/** bfs is a breadth-first search on a graph using an adjacency matrix
 *
 * @param adj is an adjacency matrix such that adj[i][j] is true iff there is an arc from node i to node j
 * @param start is the id of the node at which to start the search
 * @param val is the value we are searching for
 * @invariant 0 <= start < GSIZE
 * @invariant graph must be an array of size GSIZE
 * @invariant adj must be a 2-dimensional array with dimensions GSIZE and GSIZE
 * @return the id of the connected node containing the val; SIZE_MAX if not found.
 */
size_t bfs(size_t graph[GSIZE], bool adj[GSIZE][GSIZE], size_t start, size_t val) {
	if(GSIZE == SIZE_MAX || start >= GSIZE || GSIZE == 0) return SIZE_MAX;

	if(graph[start] == val) return start;

	size_t queue[QSIZE] = { start, SIZE_MAX };

	size_t explored[GSIZE] = { 0 };

	while(queue[0] != SIZE_MAX) {
		size_t curr = dequeue(queue);
		if(graph[curr] == val) return curr;

		for(size_t i = 0; i < GSIZE; ++i) {
			if(adj[curr][i] && i != curr && !explored[i]) {
				// return if we are out of queue space somehow
				if(!enqueue(queue, i)) return SIZE_MAX;
				explored[i] = 1;
			}
		}
	}

	return SIZE_MAX;
}

/**setcomp sets a GSIZE x GSIZE size_t array to SIZE_MAX
 *
 * @param comp the GSIZE x GSIZE array to set
 * @invariant comp must be dimension GSIZE x GSIZE
 */
void setcomp(size_t comp[GSIZE][GSIZE]) {
	for(size_t i = 0; i < GSIZE; ++i) {
		for(size_t j = 0; j < GSIZE; ++j) {
			comp[i][j] = SIZE_MAX;
		}
	}
}

/**conncomp computes the list of connected components of the undirected graph represented by the adjacency matrix adj.
 *
 * Since we are working with static arrays, the connected component array must be size GSIZE x GSIZE to allow
 * for the cases of a fully connected or fully disconnected graph.
 *
 * When complete, for a given i, comp[i][0] == SIZE_MAX indicates i-1 was the last connected component
 *
 * @param adj the adjacency matrix for the graph
 * @param comp the array in which to store the connected components
 * @invariant adj[i][j] == true IFF adj[j][i] == true 
 * @return false if an issue was encountered; true if the comp array is populated
 */
bool conncomp(bool adj[GSIZE][GSIZE], size_t comp[GSIZE][GSIZE]) {
	size_t component[GSIZE];
	size_t next = 0;

	// initialize comp to all SIZE_MAX
	setcomp(comp);

	for(size_t i = 0; i < GSIZE; ++i) {
		component[i] = SIZE_MAX;
	}

	for(size_t i = 0; i < GSIZE; ++i) {
		size_t currcomp = next;
		
		// if we've already visited this node
		// its component will have been BFSed and completely IDd
		// so go skip the rst of the loop
		if(component[i] != SIZE_MAX) continue;

		component[i] = currcomp;
		comp[currcomp][0] = i;
		size_t compidx = 1;
		size_t queue[QSIZE] = {i, SIZE_MAX};
		while(queue[0] != SIZE_MAX) {
			size_t currnode = dequeue(queue);

			for(size_t j = 0; j < GSIZE; ++j) {
				if(adj[currnode][j] && j != currnode && component[j] == SIZE_MAX) {
					if(!enqueue(queue, j)) return false;
					component[j] = currcomp;
					comp[currcomp][compidx] = j;
					compidx++;
				}
			}
		}
		next += 1;
	}
	return true;
}

/**printcomp pretty prints the connected components represented by the array comp
 *
 * 
 *
 * @param comp 	The connected components array to print.
 * 		Each inner array is a SIZE_MAX-terminated list
 * 		of connected components. A subarray with SIZE_MAX
 * 		at index zero means there is not a connected component
 * 		at that superindex.
 * @invariant comp should be dimension GSIZE x GSIZE
 */
void printcomp(size_t comp[GSIZE][GSIZE]) {
	for(size_t i = 0; i < GSIZE; ++i) {
		if(comp[i][0] == SIZE_MAX) return;
		
		printf("Connected Set %zu: { ", i+1);
		for(size_t j = 0; j < GSIZE; ++j) {
			if(comp[i][j] == SIZE_MAX) break;
			printf("%zu ", comp[i][j]);
		}
		puts("}\n");
	}
}

/**bfs_span constructs a spanning tree for the given graph defined by adjacency matrix
 *
 * @params adj the adjacency matrix for the graph
 * @params st  the 2-dimensional array in which to store the spanning tree adjacency matrix
 * @invariant  the graph represented by adj must be connected
 * @invariant  the arrays adj and st must both be of dimension GSIZExGSIZE
 */
void bfs_span(bool adj[GSIZE][GSIZE], bool st[GSIZE][GSIZE]) {
	if(GSIZE == SIZE_MAX || GSIZE == 0) return;

	size_t queue[QSIZE] = { 0, SIZE_MAX };

	bool explored[GSIZE] = { 0 };
	explored[0] = 1;

	while(queue[0] != SIZE_MAX) {
		size_t curr = dequeue(queue);

		for(size_t i = 0; i < GSIZE; ++i) {
			if(adj[curr][i] && i != curr && !explored[i]) {
				// return if we are out of queue space somehow
				if(!enqueue(queue, i)) return;
				explored[i] = 1;
				st[curr][i] = 1;
				st[i][curr] = 1;
			}
		}
	}
}

/**print_adj prints an adjacency array to the terminal
 *
 * @param adj an adjacency array of size GSIZExGSIZE
 * @invariant adj must be of size GSIZExGSIZE
 */
void print_adj(bool adj[GSIZE][GSIZE]) {
	for(size_t i = 0; i < GSIZE; ++i) {
		printf("[ ");
		for(size_t j = 0; j < GSIZE; ++j) {
			printf("%u ", adj[i][j]);
		}
		printf("]\n");
	}
	printf("\n\n");
}

int main(void) {
	size_t graph[GSIZE] = { 1, 2, 4, 4, 5 };
	bool a[GSIZE][GSIZE] = {
		{ false, true, true, false, false },
		{ true, false, true, false, false },
		{ true, true, false, true, false },
		{ false, true, false, false, true },
		{ false, false, false, true, false },
	};

	printf("The node that stores 5 is %zu\n", bfs(graph, a, 0, 5));

	size_t conns[GSIZE][GSIZE];
	if(conncomp(a, conns)) {
		printcomp(conns);
	} else {
		puts("conncomp() failed\n");
	}


	bool st[GSIZE][GSIZE] = { 0  };
	bfs_span(a, st);
	print_adj(a);
	print_adj(st);

	return EXIT_SUCCESS;
}
