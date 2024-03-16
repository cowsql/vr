#include "../include/vr.h"

#include <stdio.h>
#include <string.h>

#include "assert.h"
#include "configuration.h"
#include "heap.h"
#include "recv.h"
#include "tracing.h"

#define infof(...) Infof(v->tracer, "> " __VA_ARGS__)

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

/* Emit a start message containing information about the current state. */
static void stepStartEmitMessage(const struct vr *v)
{
    char msg[512] = {0};
    char msg_view[64];

    sprintf(msg_view, "view %lu", v->view);
    strcat(msg, msg_view);

    infof("%s", msg);
}

/* Handle a VR_START event. */
static int stepStart(struct vr *v, unsigned long view)
{
    v->view = view;
    stepStartEmitMessage(v);
    return 0;
}

/* Handle new messages. */
static int stepReceive(struct vr *v, struct vr_message *message)
{
    const char *desc;

    switch (message->type) {
        case VR_REQUEST:
            desc = "request";
            break;
        default:
            desc = "unknown message";
            break;
    }

    infof("recv %s from server %u", desc, message->server_id);

    return recvMessage(v, message);
}

int vr_step(struct vr *v,
            const struct vr_event *event,
            struct vr_update *update)
{
    int rv;

    assert(event != NULL);
    assert(update != NULL);

    switch (event->type) {
        case VR_START:
            rv = stepStart(v, event->start.view);
            break;
        case VR_RECEIVE:
            rv = stepReceive(v, event->receive.message);
            break;
        default:
            rv = VR_INVALID;
            break;
    }

    if (rv != 0) {
        goto out;
    }

out:
    if (rv != 0) {
        return rv;
    }
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
