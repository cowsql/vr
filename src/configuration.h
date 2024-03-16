/* Modify and inspect @vr_configuration objects. */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "../include/vr.h"

/* Initialize an empty configuration. */
void ConfigurationInit(struct vr_configuration *c);

/* Release all memory used by the given configuration. */
void ConfigurationClose(struct vr_configuration *c);

/* Return the index of the server with the given ID (relative to the c->servers
 * array). If there's no server with the given ID, return the number of
 * servers. */
unsigned ConfigurationIndexOf(const struct vr_configuration *c, unsigned id);

/* Add a server to the given configuration.
 *
 * The given @address is copied and no reference to it is kept. In case of
 * error, @c is left unchanged.
 *
 * Errors:
 *
 * VR_DUPLICATEID
 *     @c already has a server with the given id.
 *
 * VR_DUPLICATEADDRESS
 *     @c already has a server with the given @address.
 *
 * VR_BADROLE
 *     @role is not one of VR_VOTER, VR_STANDBY or VR_SPARE.
 *
 * VR_NOMEM
 *     A copy of @address could not be made or the @c->servers could not
 *     be extended
 */
int ConfigurationAdd(struct vr_configuration *c,
                     unsigned id,
                     const char *address,
                     int role);

#endif /* CONFIGURATION_H_ */
