#ifndef _CIRCULAR_H
#define _CIRCULAR_H

#include <stddef.h>

/**an opaque type for a circular buffer for **double** values
 *
 * This data structure allows to add double values in rear and
 * to take them out in front. Each structure has a maximal number
 * of elements that can be stored in it.
 */
typedef struct circular circular;

/**append a new element with the *value*  to the buffer *c*.
 *
 * @return *c* if the new element could be appended, 0 otherwise.
 */
circular* circular_append(circular* c, double value);

/**remove the oldest element from *c* and return its value.
 *
 * @return the removed element if it exists, 0.0 otherwise.
 */
double circular_pop(circular* c);

/**return a pointer to position *pos* in buffer *c*.
 *
 * @return a pointer to the *pos* element of the buffer, 0 other.
 */
double* circular_element(circular* c, size_t pos);

/**initialize a circular buffer *c* with maximally *max_len* elements.
 *
 * Only use this function on an unintialized buffer.
 *
 * Each buffer that is initialized with this function must be destroyed
 * with a call to circular_destroy.
 *
 * @return *c* with a max_len set to *max_len* iff malloc was successful
 */
circular* circular_init(circular* c, size_t max_len);

/**destroy circular buffer *c*.
 *
 * *c* must have been initialized with a call to circular_init.
 */
void circular_destroy(circular* c);

/**allocate and initialize a circular buffer with maximally *len* elements.
 *
 * Each buffer that is allocated with this function must be deleted with
 * a call to circular_delete.
 */
circular* circular_new(size_t len);

/**delete circular buffer *c*.
 *
 * *c* must have been allocated with a call to circular_new.
 */
void circular_delete(circular* c);

/**resize *c* to capacity *max_len*.
 */
circular* circular_resize(circular* c, size_t max_len);

/**return the number of elements stored
 */
size_t circular_getlength(circular* c);

#endif
