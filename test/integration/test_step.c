#include "../../include/vr.h"
#include "../lib/runner.h"

SUITE(step)

/* Generation of the ID of the bootstrap dqlite node. */
TEST(step, noop, NULL, NULL, 0, NULL)
{
    int rv;
    rv = vr_step();
    munit_assert_int(0, ==, rv);
    return MUNIT_OK;
}
