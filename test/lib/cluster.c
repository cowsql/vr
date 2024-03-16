#include <stdio.h>

#include "cluster.h"

/* Custom emit tracer function which includes the server ID. */
static void serverEmit(struct vr_tracer *t, unsigned type, const void *data)
{
    struct test_server *server;
    struct test_cluster *cluster;
    const struct vr_tracer_info *info = data;
    char trace[1024];

    server = t->impl;
    cluster = server->cluster;

    if (cluster->in_tear_down) {
        return;
    }

    if (type != VR_TRACER_DIAGNOSTIC) {
        return;
    }

    if (info->diagnostic.level > 3) {
        fprintf(stderr, "TRACE: %u > %s\n", server->vr.id,
                info->diagnostic.message);
        return;
    }

    if (info->diagnostic.message[0] == '>') {
        snprintf(trace, sizeof trace, "[%4lu] %u %s", cluster->time,
                 server->vr.id, info->diagnostic.message);
    } else {
        snprintf(trace, sizeof trace, "         %s", info->diagnostic.message);
    }
    strcat(cluster->trace, trace);
    strcat(cluster->trace, "\n");
    fprintf(stderr, "%s\n", trace);
}

/* Initialize a new server object. */
static void serverInit(struct test_server *s,
                       unsigned id,
                       struct test_cluster *cluster)
{
    int rv;

    s->tracer.impl = s;
    s->tracer.version = 1;
    s->tracer.emit = serverEmit;

    sprintf(s->address, "%u", id);

    rv = vr_init(&s->vr, id);
    munit_assert_int(rv, ==, 0);

    s->vr.tracer = &s->tracer;

    s->cluster = cluster;
    s->running = false;
}

static void serverStop(struct test_server *s)
{
    s->running = false;
}

/* Release all resources used by a server object. */
static void serverClose(struct test_server *s)
{
    if (s->running) {
        serverStop(s);
    }
    vr_close(&s->vr);
}

void test_cluster_setup(const MunitParameter params[], struct test_cluster *c)
{
    unsigned i;

    (void)params;

    for (i = 0; i < TEST_CLUSTER_N_SERVERS; i++) {
        serverInit(&c->servers[i], i + 1, c);
    }

    c->time = 0;
    c->in_tear_down = false;
}

void test_cluster_tear_down(struct test_cluster *c)
{
    unsigned i;

    c->in_tear_down = true;

    for (i = 0; i < TEST_CLUSTER_N_SERVERS; i++) {
        serverClose(&c->servers[i]);
    }
}
