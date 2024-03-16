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
    (void)f;
    return MUNIT_OK;
}
