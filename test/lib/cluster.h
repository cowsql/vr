/* Setup and drive a test VR cluster. */

#ifndef TEST_CLUSTER_H
#define TEST_CLUSTER_H

#include <stdbool.h>

#include "../../include/vr.h"

#include "munit.h"

#define FIXTURE_CLUSTER struct test_cluster cluster

#define SETUP_CLUSTER() test_cluster_setup(params, &f->cluster)

#define TEAR_DOWN_CLUSTER() test_cluster_tear_down(&f->cluster)

/* Start the server with the given ID. */
#define CLUSTER_START(ID) test_cluster_start(&f->cluster, ID)

/* Step the cluster until the all expected output is consumed. Fail the test if
 * a mismatch is found. */
#define CLUSTER_TRACE(EXPECTED)                       \
    if (!test_cluster_trace(&f->cluster, EXPECTED)) { \
        munit_error("trace does not match");          \
    }

/* Wrap a @vr instance and maintain network state. */
struct test_cluster;
struct test_server
{
    struct vr_tracer tracer;      /* Custom tracer */
    struct vr vr;                 /* VR instance */
    struct test_cluster *cluster; /* Parent cluster */
    unsigned network_latency;     /* Network latency */
    char address[8];              /* Server address */
    bool running;                 /* Whether the server is running */
};

#define TEST_CLUSTER_N_SERVERS 8

/* Cluster of test raft servers instances with fake disk and network I/O. */
struct test_cluster
{
    struct test_server servers[TEST_CLUSTER_N_SERVERS]; /* Cluster servers */
    unsigned long time;                                 /* Global time */
    bool in_tear_down;                                  /* Tearing down */
    char trace[8192];                                   /* Captured messages */
};

void test_cluster_setup(const MunitParameter params[], struct test_cluster *c);
void test_cluster_tear_down(struct test_cluster *c);

/* Start the server with the given @id. */
void test_cluster_start(struct test_cluster *c, unsigned id);

/* Advance the cluster by completing a single asynchronous operation or firing a
 * timeout. */
void test_cluster_step(struct test_cluster *c);

/* Compare the trace of all messages emitted by all servers with the given
 * expected trace. If they don't match, print the last line which differs and
 * return #false. */
bool test_cluster_trace(struct test_cluster *c, const char *expected);

#endif /* TEST_CLUSTER_H */
