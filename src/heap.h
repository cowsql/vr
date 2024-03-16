/* Internal heap APIs. */

#ifndef HEAP_H_
#define HEAP_H_

#include <stddef.h>

#include "../include/vr.h"

void *HeapMalloc(size_t size);

void *HeapCalloc(size_t nmemb, size_t size);

void *HeapRealloc(void *ptr, size_t size);

void HeapFree(void *ptr);

void HeapSet(struct vr_heap *heap);

#endif /* HEAP_H_ */
