#include <string.h>

#include "assert.h"
#include "configuration.h"
#include "heap.h"

/* Current encoding format version. */
#define ENCODING_FORMAT 1

void ConfigurationInit(struct vr_configuration *c)
{
    c->servers = NULL;
    c->n = 0;
}

void ConfigurationClose(struct vr_configuration *c)
{
    size_t i;
    assert(c != NULL);
    assert(c->n == 0 || c->servers != NULL);
    for (i = 0; i < c->n; i++) {
        HeapFree(c->servers[i].address);
    }
    if (c->servers != NULL) {
        HeapFree(c->servers);
    }
}

unsigned ConfigurationIndexOf(const struct vr_configuration *c,
                              const unsigned id)
{
    unsigned i;
    assert(c != NULL);
    for (i = 0; i < c->n; i++) {
        if (c->servers[i].id == id) {
            return i;
        }
    }
    return c->n;
}

int ConfigurationAdd(struct vr_configuration *c,
                     const unsigned id,
                     const char *address,
                     const int role)
{
    struct vr_server *servers;
    struct vr_server *server;
    char *address_copy;
    size_t i;
    int rv;
    assert(c != NULL);
    assert(id != 0);

    if (role != VR_STANDBY && role != VR_VOTER && role != VR_SPARE) {
        rv = VR_BADROLE;
        goto err;
    }

    /* Check that neither the given id or address is already in use */
    for (i = 0; i < c->n; i++) {
        server = &c->servers[i];
        if (server->id == id) {
            rv = VR_DUPLICATEID;
            goto err;
        }
        if (strcmp(server->address, address) == 0) {
            rv = VR_DUPLICATEADDRESS;
            goto err;
        }
    }

    /* Make a copy of the given address */
    address_copy = HeapMalloc(strlen(address) + 1);
    if (address_copy == NULL) {
        rv = VR_NOMEM;
        goto err;
    }
    strcpy(address_copy, address);

    /* Grow the servers array.. */
    servers = HeapRealloc(c->servers, (c->n + 1) * sizeof *server);
    if (servers == NULL) {
        rv = VR_NOMEM;
        goto err_after_address_copy;
    }
    c->servers = servers;

    /* Fill the newly allocated slot (the last one) with the given details. */
    server = &servers[c->n];
    server->id = id;
    server->address = address_copy;
    server->role = role;

    c->n++;

    return 0;

err_after_address_copy:
    HeapFree(address_copy);
err:
    assert(rv == VR_BADROLE || rv == VR_DUPLICATEID ||
           rv == VR_DUPLICATEADDRESS || rv == VR_NOMEM);
    return rv;
}
