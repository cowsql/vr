#ifndef VR_H
#define VR_H

#include <stddef.h>

#define VR_API __attribute__((visibility("default")))

enum { VR_NOMEM = 1, VR_DUPLICATEID, VR_DUPLICATEADDRESS, VR_BADROLE };

enum vr_role { VR_VOTER = 1, VR_STANDBY, VR_SPARE };

struct vr_server
{
    unsigned id;       /* Server ID, must be greater than zero. */
    char *address;     /* Server address. User defined. */
    enum vr_role role; /* Server role. */
};

/* From Figure 2:
 *
 *  The configuration. This is a sorted array containing the IP addresses of
 *  each of the 2f + 1 replicas.
 */
struct vr_configuration
{
    struct vr_server *servers; /* Array of servers member of the cluster. */
    unsigned n;                /* Number of servers in the array. */
};

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
