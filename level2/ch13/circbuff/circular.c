#include "circular.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/** @brief the hidden implementation of the circular buffer type */
struct circular {
	size_t start; /**< Position of element 0 */
	size_t len;   /**< Number of elements stored */
	size_t max_len; /**< Maximum capacity */
	double* tab; /**< Array holding the data */
};

circular* circular_init (circular* c, size_t max_len) {
	if (c) {
		if (max_len) {
			*c = (circular){
				.max_len = max_len,
				.tab = malloc(sizeof(double[max_len])),
			};

			if (!c->tab) c->max_len = 0;
		} else {
			*c = (circular){ 0 };
		}
	}
	return c;
}

void circular_destroy(circular* c) {
	if (c) {
		free(c->tab);
		circular_init(c, 0);
	}
}

static size_t circular_getpos(circular* c, size_t pos) {
	pos += c->start;
	pos %= c->max_len;
	return pos;
}

double* circular_element(circular* c, size_t pos) {
	double* ret = 0;
	if (c) {
		if (pos < c->max_len) {
			pos = circular_getpos(c, pos);
			ret = &c->tab[pos];
		}
	}
	return ret;
}

circular* circular_append(circular* c, double value) {
	if (c) {
		if (c->len < c->max_len) {
			size_t pos = circular_getpos(c, c->len);
			c->tab[pos] = value;
			c->len += 1;
			return c;
		}
	}
	return 0;
}

double circular_pop(circular* c) {
	if (c) {
		if (c->len > 0) {
			// first get the 0th value
			size_t pos = circular_getpos(c, 0);
			double val = c->tab[pos];

			// then set c->len = c->len - 1 (we've removed an element)
			// add 1 to c->start (i.e. advance the head of the buffer)
			// set start to start % max_len in case we've wrapped around
			c->len -= 1;
			c->start += 1;
			c->start %= c->max_len;

			return val;
		}
	}
	return 0.0;
}

circular* circular_new(size_t len) {
	circular* new_c = malloc(sizeof(circular));
	return circular_init(new_c, len);
}

void circular_delete(circular* c) {
	circular_destroy(c);
}

size_t circular_getlength(circular* c) {
	if (c) {
		return c->len;
	}	
	return 0;
}

circular* circular_resize(circular* c, size_t nlen) {
	if (c) {
		size_t len = c->len;
		if (len > nlen) return 0;
		size_t olen = c->max_len;
		if (nlen != olen) {
			size_t ostart = circular_getpos(c, 0);
			size_t nstart = ostart;
			double* otab = c->tab;
			double* ntab;
			if (nlen > olen) {
				ntab = realloc(c->tab, sizeof(double[nlen]));
				if (!ntab) return 0;
				// If we have two separate chunks
				if (ostart+len > olen) {
					size_t ulen = olen - ostart;
					size_t llen = len - ulen;
					if (llen <= (nlen - olen)) {
						memcpy(ntab + olen, ntab,
							llen*sizeof(double));
					} else {
						nstart = nlen - ulen;
						memmove(ntab + nstart, ntab + ostart,
							ulen*sizeof(double));
					}
				}
			} else {
				nstart = 0;
				if (ostart == 0) {
					// if ostart == 0, we can just realloc
					// since nlen > len and realloc will keep what it can
					// here, all of it
					ntab = realloc(c->tab, sizeof(double[nlen]));
				} else {
					size_t ulen = olen - ostart;
					size_t llen = len - ulen;

					// one chunk
					if (ulen >= len) {
						memmove(otab, otab + ostart, len*sizeof(double));
						ntab = realloc(otab, sizeof(double[nlen]));
					}  else {
						// two chunks
						ntab = malloc(sizeof(double[nlen]));
						memcpy(ntab, otab + ostart, ulen*sizeof(double));
						memcpy(ntab+ulen, otab, llen*sizeof(double));
						free(otab);
					}
				}
			}
			*c = (circular){
				.max_len = nlen,
				.start = nstart,
				.len = len,
				.tab = ntab,
			};
		}
	}
	return c;
}
