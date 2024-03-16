#ifndef VR_H
#define VR_H

#define VR_API __attribute__((visibility("default")))

VR_API int vr_step(void);

/**
 * User-definable dynamic memory allocation functions.
 *
 * The @data field will be passed as first argument to all functions.
 */
struct vr_heap
{
    void *data; /* User data */
    void *(*malloc)(void *data, size_t size);
    void (*free)(void *data, void *ptr);
    void *(*calloc)(void *data, size_t nmemb, size_t size);
    void *(*realloc)(void *data, void *ptr, size_t size);
};

VR_API void *vr_malloc(size_t size);
VR_API void vr_free(void *ptr);
VR_API void *vr_calloc(size_t nmemb, size_t size);
VR_API void *vr_realloc(void *ptr, size_t size);

VR_API void vr_heap_set(struct vr_heap *heap);

#endif /* VR_H */
