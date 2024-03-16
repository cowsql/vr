#include "recv.h"
#include "recv_request.h"
#include "tracing.h"

#define infof(...) Infof(r->tracer, "  " __VA_ARGS__)

/* Dispatch a single message to the appropriate handler. */
int recvMessage(struct vr *v, struct vr_message *message)
{
    unsigned id = message->server_id;
    const char *address = message->server_address;
    int rv = 0;

    switch (message->type) {
        case VR_REQUEST:
            rv = recvRequest(v, id, address, &message->request);
            break;
        default:
            rv = VR_INVALID;
            break;
    };

    if (rv != 0) {
        return rv;
    }

    return 0;
}

#undef infof
