#include "../../include/vr.h"
#include "../lib/runner.h"

static void *setUp(const MunitParameter params[], MUNIT_UNUSED void *user_data)
{
    struct vr *v = munit_malloc(sizeof *v);
    int rv;

    (void)params;
    rv = vr_init(v, 1);
    munit_assert_int(rv, ==, 0);

    return v;
}

static void tearDown(void *data)
{
    struct vr *v = data;
    vr_close(v);
    free(v);
}

SUITE(step)

/* Passing an invalid event code results in an error. */
TEST(step, invalid, setUp, tearDown, 0, NULL)
{
    struct vr *v = data;
    struct vr_event event;
    struct vr_update update;
    int rv;
    event.type = 666;
    rv = vr_step(v, &event, &update);
    munit_assert_int(rv, ==, VR_INVALID);
    return MUNIT_OK;
}
