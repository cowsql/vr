/* Setup and drive a test VR cluster. */

#ifndef TEST_CLUSTER_H
#define TEST_CLUSTER_H

#include <stdbool.h>

#include "../../include/vr.h"

#include "munit.h"

#define FIXTURE_CLUSTER struct test_cluster cluster

#define SETUP_CLUSTER() test_cluster_setup(params, &f->cluster)

#define TEAR_DOWN_CLUSTER() test_cluster_tear_down(&f->cluster)

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

#endif /* TEST_CLUSTER_H */
