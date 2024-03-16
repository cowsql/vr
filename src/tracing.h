/* Tracing functions and helpers. */

#ifndef TRACING_H_
#define TRACING_H_

#include "../include/vr.h"

#define LIKELY(x) __builtin_expect(!!(x), 1)

/* Use TRACER to trace an event of type TYPE with the given INFO. */
#define Trace(TRACER, TYPE, INFO)             \
    do {                                      \
        if (LIKELY(TRACER == NULL)) {         \
            break;                            \
        }                                     \
        if (LIKELY(TRACER->version == 2)) {   \
            TRACER->emit(TRACER, TYPE, INFO); \
        }                                     \
    } while (0)

/* Emit a diagnostic message with the given tracer at level 3. */
#define Infof(TRACER, ...) Logf(TRACER, 3, __VA_ARGS__)

/* Emit diagnostic message with the given tracer at level 5. */
#define Tracef(TRACER, ...) Logf(TRACER, 5, __VA_ARGS__)

/* Use the tracer to log an event at the given level.
 *
 * The LEVEL parameter should be one of:
 *
 * - 1: error
 * - 2: warning
 * - 3: info
 * - 4: debug
 * - 5: trace
 */
#define Logf(TRACER, LEVEL, ...)                  \
    do {                                          \
        unsigned _type;                           \
        struct vr_tracer_info _info;              \
        static char _msg[1024];                   \
                                                  \
        if (LIKELY(TRACER == NULL)) {             \
            break;                                \
        }                                         \
                                                  \
        snprintf(_msg, sizeof _msg, __VA_ARGS__); \
                                                  \
        _type = VR_TRACER_DIAGNOSTIC;             \
        _info.version = 1;                        \
        _info.diagnostic.level = LEVEL;           \
        _info.diagnostic.message = _msg;          \
        _info.diagnostic.file = __FILE__;         \
        _info.diagnostic.line = __LINE__;         \
        TRACER->emit(TRACER, _type, &_info);      \
    } while (0)

#endif /* TRACING_H_ */
