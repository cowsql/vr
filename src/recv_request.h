/* Receive a Request message. */

#ifndef RECV_REQUEST_H_
#define RECV_REQUEST_H_

#include "../include/vr.h"

/* Process a Request message from the given server. */
int recvRequest(struct vr *v,
                unsigned id,
                const char *address,
                const struct vr_request *request);

#endif /* RECV_REQUEST_H_ */
