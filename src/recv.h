/* Receive a message. */

#ifndef RECV_H_
#define RECV_H_

#include "../include/vr.h"

/* Function to be invoked upon receiving a message. */
int recvMessage(struct vr *r, struct vr_message *message);

#endif /* RECV_H_ */
