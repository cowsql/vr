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

/* Fire the given event using raft_step() and process the resulting struct
 * raft_update object. */
static int serverStep(struct test_server *s, struct vr_event *event)
{
    struct vr *v = &s->vr;
    struct vr_update update;
    int rv;

    munit_assert_true(s->running);

    rv = vr_step(v, event, &update);
    if (rv != 0) {
        return rv;
    }

    return 0;
}

/* Start the server by passing to raft_step() a RAFT_START event with the
 * current disk state. */
static void serverStart(struct test_server *s)
{
    struct vr_event event;
    int rv;

    s->running = true;

    event.time = s->cluster->time;
    event.type = VR_START;
    event.start.view = 0;

    rv = serverStep(s, &event);
    munit_assert_int(rv, ==, 0);
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

/* Return the server with the given @id. */
static struct test_server *clusterGetServer(struct test_cluster *c, unsigned id)
{
    munit_assert_ulong(id, <=, TEST_CLUSTER_N_SERVERS);
    return &c->servers[id - 1];
}

void test_cluster_tear_down(struct test_cluster *c)
{
    unsigned i;

    c->in_tear_down = true;

    for (i = 0; i < TEST_CLUSTER_N_SERVERS; i++) {
        serverClose(&c->servers[i]);
    }
}

void test_cluster_start(struct test_cluster *c, unsigned id)
{
    struct test_server *server = clusterGetServer(c, id);
    serverStart(server);
}

void test_cluster_step(struct test_cluster *c)
{
    (void)c;
}

bool test_cluster_trace(struct test_cluster *c, const char *expected)
{
    size_t n1;
    size_t n2;
    size_t i;
    unsigned max_steps = 100;

consume:
    if (max_steps == 0) {
        goto mismatch;
    }
    max_steps -= 1;

    n1 = strlen(c->trace);
    n2 = strlen(expected);

    for (i = 0; i < n1 && i < n2; i++) {
        if (c->trace[i] != expected[i]) {
            break;
        }
    }

    /* Check if we produced more output than the expected one. */
    if (n1 > n2) {
        goto mismatch;
    }

    /* If there's more expected output, check that so far we're good, then
     * step and repeat. */
    if (n1 < n2) {
        if (i != n1) {
            goto mismatch;
        }
        c->trace[0] = 0;
        expected += i;
        test_cluster_step(c);
        goto consume;
    }

    munit_assert_ulong(n1, ==, n2);
    if (i != n1) {
        goto mismatch;
    }

    c->trace[0] = 0;

    return true;

mismatch:
    fprintf(stderr, "==> Expected:\n");
    fprintf(stderr, "%s\n", expected);

    fprintf(stderr, "==> Actual:\n");
    fprintf(stderr, "%s\n", c->trace);

    return false;
}
