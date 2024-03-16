/* Define the assert() macro, either as the standard one or the test one. */

#ifndef ASSERT_H_
#define ASSERT_H_

#if defined(NDEBUG)
#define assert(x)        \
    do {                 \
        (void)sizeof(x); \
    } while (0)
#elif defined(VR_ASSERT_WITH_BACKTRACE)
#include <assert.h> /* for __assert_fail */
#include <backtrace.h>
#include <stdio.h>
#undef assert
#define assert(x)                                                 \
    do {                                                          \
        struct backtrace_state *state_;                           \
        if (!(x)) {                                               \
            state_ = backtrace_create_state(NULL, 0, NULL, NULL); \
            backtrace_print(state_, 0, stderr);                   \
            __assert_fail(#x, __FILE__, __LINE__, __func__);      \
        }                                                         \
    } while (0)
#else
#include <assert.h>
#endif

#endif /* ASSERT_H_ */
