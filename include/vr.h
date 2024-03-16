#ifndef VR_H
#define VR_H

#include <stddef.h>

#define VR_API __attribute__((visibility("default")))

enum {
    VR_NOMEM = 1,
    VR_DUPLICATEID,
    VR_DUPLICATEADDRESS,
    VR_BADROLE,
    VR_INVALID /* Invalid parameter */
};

/* Customizable tracer for debugging, logging and metrics purposes. */
struct vr_tracer
{
    void *impl; /* Implementation-defined state object. */

    /* Version of the vr_tracer structure. Must be 1. */
    unsigned char version;

    /* Emit an event of the given @type code. The @info object contains
     * details about the event and its format depends on the event @type.
     *
     * Type codes from #1 to #255 are reserved for traces defined by the
     * core library.
     *
     * Type codes from #256 to #65535 are reserved for events defined by
     * specific I/O backends.
     *
     * Type codes from #65535 onwards can be used by user applications.
     */
    void (*emit)(struct vr_tracer *t, unsigned type, const void *info);
};

#define VR_TRACER_DIAGNOSTIC 1

/* Information passed as @info argument to #vr_tracer->emit() for
 * traces generated by #vr objects.
 */
struct vr_tracer_info
{
    /* Version of the #vr_tracer_info structure. Must be least 1. */
    unsigned char version;

    union {
        struct
        {
            unsigned level; /* 1 Error, 2 Warning, 3 Info, 4 Debug, 5 Trace */
            const char *message;
            const char *file;
            int line;
        } diagnostic; /* @type VR_TRACER_DIAGNOSTIC */
    };
};

struct vr_buffer
{
    void *base; /* Pointer to the buffer data. */
    size_t len; /* Length of the buffer. */
};

struct vr_op
{
    struct vr_buffer buf; /* Operation data. */
};

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

/* From Section 3:
 *
 *  VR uses three sub-protocols that work together to ensure correctness:
 *
 *  - Normal case processing of user requests.
 *  - View changes to select a new primary.
 *  - Recovery of a failed replica so that it can rejoin the group.
 */
enum vr_status { VR_NORMAL = 1, VR_VIEW_CHANGE, VR_RECOVERING };

/* From Figure 2:
 *
 *  This is an array containing op-number entries. The entries contain the
 *  requests that have been received so far in their assigned order.
 */
struct vr_log
{
    struct vr_op *ops;
};

/* From Figure 2:
 *
 *  This records for each client the number of its most recent request, plus, if
 *  the request has been executed, the result sent for that request.
 */
struct vr_client_table
{
    unsigned n; /* Number of registered clients */
};

struct vr
{
    /* Figure 2: VR state at a replica */
    struct vr_configuration configuration; /* Current configuration */
    unsigned id;                           /* Replica ID (replica number) */
    unsigned long view;                    /* Current view number */
    enum vr_status status;                 /* Current sub-protocol */
    unsigned long op_number;               /* Current operation number */
    struct vr_log log;                     /* Requests received so far */
    unsigned long commit_number;           /* Most recently committed op */
    struct vr_client_table client_table;   /* Registered clients */

    struct vr_tracer *tracer; /* Custom tracer */
};

VR_API int vr_init(struct vr *v, unsigned id);
VR_API int vr_close(struct vr *v);

enum vr_event_type {
    VR_START = 1, /* Initial event starting the server. */
    VR_RECEIVE    /* A message has been received from another server. */
};

/* Represents an external event that drives the VR engine forward (for example
 * receiving a message). */
struct vr_event
{
    unsigned long time;
    enum vr_event_type type;
    unsigned char unused;
    unsigned short capacity;
    unsigned char reserved[4];
    union {
        struct
        {
            unsigned long view;
        } start;
    };
};

/* Hold information about changes that user code must perform after a call to
 * vr_step() returns (e.g. new messages that must be sent, etc.). */
struct vr_update
{
    unsigned flags;
};

VR_API int vr_step(struct vr *v,
                   const struct vr_event *event,
                   struct vr_update *update);

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
