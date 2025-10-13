#ifndef __BLOP_MACROS_H__
#define __BLOP_MACROS_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "blop_basics.h"

#ifndef BLOP_MACROS_PREFIX
    #define BLOP_MACROS_PREFIX blop
#endif

#ifndef BLOP_MACROS_CAMEL

#define __BLOP_CAST         BLOP_XCONCAT(BLOP_MACROS_PREFIX, _cast)
#define __BLOP_TERNARY      BLOP_XCONCAT(BLOP_MACROS_PREFIX, _ternary)
#define __BLOP_MIN          BLOP_XCONCAT(BLOP_MACROS_PREFIX, _min)
#define __BLOP_MAX          BLOP_XCONCAT(BLOP_MACROS_PREFIX, _max)
#define __BLOP_DISTANCE     BLOP_XCONCAT(BLOP_MACROS_PREFIX, _distance)
#define __BLOP_DIFFERENCE   BLOP_XCONCAT(BLOP_MACROS_PREFIX, _difference)
#define __BLOP_PADD         BLOP_XCONCAT(BLOP_MACROS_PREFIX, _padd)
#define __BLOP_PSUB         BLOP_XCONCAT(BLOP_MACROS_PREFIX, _psub)
#define __BLOP_FREE         BLOP_XCONCAT(BLOP_MACROS_PREFIX, _free)
#define __BLOP_MALLOC       BLOP_XCONCAT(BLOP_MACROS_PREFIX, _malloc)
#define __BLOP_CALLOC       BLOP_XCONCAT(BLOP_MACROS_PREFIX, _calloc)
#define __BLOP_REALLOC      BLOP_XCONCAT(BLOP_MACROS_PREFIX, _realloc)

#else // BLOP_MACROS_CAMEL

#define __BLOP_CAST         BLOP_XCONCAT(BLOP_MACROS_PREFIX, Cast)
#define __BLOP_TERNARY      BLOP_XCONCAT(BLOP_MACROS_PREFIX, Ternary)
#define __BLOP_MIN          BLOP_XCONCAT(BLOP_MACROS_PREFIX, Min)
#define __BLOP_MAX          BLOP_XCONCAT(BLOP_MACROS_PREFIX, Max)
#define __BLOP_DISTANCE     BLOP_XCONCAT(BLOP_MACROS_PREFIX, Distance)
#define __BLOP_DIFFERENCE   BLOP_XCONCAT(BLOP_MACROS_PREFIX, Difference)
#define __BLOP_PADD         BLOP_XCONCAT(BLOP_MACROS_PREFIX, Padd)
#define __BLOP_PSUB         BLOP_XCONCAT(BLOP_MACROS_PREFIX, Psub)
#define __BLOP_FREE         BLOP_XCONCAT(BLOP_MACROS_PREFIX, Free)
#define __BLOP_MALLOC       BLOP_XCONCAT(BLOP_MACROS_PREFIX, Malloc)
#define __BLOP_CALLOC       BLOP_XCONCAT(BLOP_MACROS_PREFIX, Calloc)
#define __BLOP_REALLOC      BLOP_XCONCAT(BLOP_MACROS_PREFIX, Realloc)

#endif

// C Language Macros

#define __BLOP_CAST(type, tocast)   ((type)(tocast))
#define __BLOP_TERNARY(cnd, x, y)   ((cnd) ? (x) : (y))

// Mathematical Macros

#define __BLOP_MIN(x, y)         __BLOP_TERNARY((x > y), y, x)
#define __BLOP_MAX(x, y)         __BLOP_TERNARY((x > y), x, y)

#define __BLOP_DISTANCE(x, y)   (__BLOP_TERNARY(x > y, x - y, y - x))
#define __BLOP_DIFFERENCE(x, y) (__BLOP_TERNARY(x > y, x - y, 0 - (y - x)))

// Ptr arithmetic

#define __BLOP_PADD(ptr, value) (void*)((uint8_t*)ptr + value)
#define __BLOP_PSUB(ptr, value) (void*)((uint8_t*)ptr - value)

// stdlib Macros

#define __BLOP_FREE(ptr)                         free((void*)ptr)
#define __BLOP_MALLOC(type, size)         (type*)malloc(size)
#define __BLOP_CALLOC(type, count)        (type*)calloc(count, sizeof(type))
#define __BLOP_REALLOC(type, ptr, size)   (type*)realloc((void*)ptr, size)

#endif // __BLOP_MACROS_H__