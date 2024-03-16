#include "../include/vr.h"

#include "configuration.h"
#include "heap.h"

int vr_init(struct vr *v, unsigned id)
{
    ConfigurationInit(&v->configuration);
    v->id = id;
    v->view = 0;
    v->status = VR_NORMAL;
    v->op_number = 0;

    v->tracer = NULL;

    return 0;
}

int vr_close(struct vr *v)
{
    (void)v;
    return 0;
}

int vr_step(void)
{
    return 0;
}

void *vr_malloc(size_t size)
{
    return HeapMalloc(size);
}

void vr_free(void *ptr)
{
    HeapFree(ptr);
}

void *vr_calloc(size_t nmemb, size_t size)
{
    return HeapCalloc(nmemb, size);
}

void *vr_realloc(void *ptr, size_t size)
{
    return HeapRealloc(ptr, size);
}

void vr_heap_set(struct vr_heap *heap)
{
    HeapSet(heap);
}
