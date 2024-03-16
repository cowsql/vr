#include <stdlib.h>

#include "heap.h"

static void *defaultMalloc(void *data, size_t size)
{
    (void)data;
    return malloc(size);
}

static void defaultFree(void *data, void *ptr)
{
    (void)data;
    free(ptr);
}

static void *defaultCalloc(void *data, size_t nmemb, size_t size)
{
    (void)data;
    return calloc(nmemb, size);
}

static void *defaultRealloc(void *data, void *ptr, size_t size)
{
    (void)data;
    return realloc(ptr, size);
}

static struct vr_heap defaultHeap = {
    NULL,           /* data */
    defaultMalloc,  /* malloc */
    defaultFree,    /* free */
    defaultCalloc,  /* calloc */
    defaultRealloc, /* realloc */
};

static struct vr_heap *currentHeap = &defaultHeap;

void *HeapMalloc(size_t size)
{
    return currentHeap->malloc(currentHeap->data, size);
}

void HeapFree(void *ptr)
{
    if (ptr == NULL) {
        return;
    }
    currentHeap->free(currentHeap->data, ptr);
}

void *HeapCalloc(size_t nmemb, size_t size)
{
    return currentHeap->calloc(currentHeap->data, nmemb, size);
}

void *HeapRealloc(void *ptr, size_t size)
{
    return currentHeap->realloc(currentHeap->data, ptr, size);
}

void HeapSet(struct vr_heap *heap)
{
    currentHeap = heap;
}
