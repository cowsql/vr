#include "../lib/cluster.h"
#include "../lib/runner.h"

struct fixture
{
    FIXTURE_CLUSTER;
};

static void *setUp(const MunitParameter params[], MUNIT_UNUSED void *user_data)
{
    struct fixture *f = munit_malloc(sizeof *f);
    SETUP_CLUSTER();
    return f;
}

static void tearDown(void *data)
{
    struct fixture *f = data;
    TEAR_DOWN_CLUSTER();
    free(f);
}

SUITE(normal)

TEST(normal, request, setUp, tearDown, 0, NULL)
{
    struct fixture *f = data;
    CLUSTER_START(1);
    CLUSTER_TRACE("[   0] 1 > view 0\n");
    return MUNIT_OK;
}
