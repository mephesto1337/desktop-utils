#ifndef DESKTOP_UTILS_MACROS_H
#define DESKTOP_UTILS_MACROS_H

#include <stdbool.h>
#include <stdio.h>

#include "log.h"

#define CHK(expr, cond)         \
    do { \
        debug("%s", #expr); \
        if ( (expr) cond ) { \
            error("%s failed", #expr); \
            goto fail; \
        } \
    } while ( 0 )

#define CHK_NEG(expr)           CHK((long)(expr), < 0L)
#define CHK_FALSE(expr)         CHK(!!(expr), == false)
#define CHK_NULL(expr)          CHK(expr, == NULL)
#define SAFE_FREE(func, ptr)    \
    do { \
        if ( ptr != NULL ) { \
            func(ptr); \
        } \
        ptr = NULL; \
    } while ( 0 )
#define STRINGIFY(x)            __STRINGIFY(x)
#define __STRINGIFY(x)          #x
#define UNUSED(x)				(void)x
#define ARRAY_SIZE(x)           (sizeof(x) / sizeof(x[0]))


#endif // DESKTOP_UTILS_MACROS_H
