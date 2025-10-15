#ifndef __BLOP_H__
#define __BLOP_H__

// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// Custom Macros

#define BLOP_ERROR_MESSAGE(msg)                     printf("[BLOP -> %s{%s} ]: %s\n", __FILE__, __FUNCTION__, msg)
#define BLOP_ERROR_MESSAGE_BONDS(msg, idx, bond)    printf("[BLOP -> %s{%s} idx = %zu while bond = %zu]: %s\n", __FILE__, __FUNCTION__, idx, bond, msg)

#define BLOP_CONCAT2_IMPL(a, b)     a##b
#define BLOP_CONCAT3_IMPL(a, b, c)  a##b##c

#define BLOP_CONCAT2(a, b)          BLOP_CONCAT2_IMPL(a, b)
#define BLOP_CONCAT3(a, b, c)       BLOP_CONCAT3_IMPL(a, b, c)

// Memory Macros

#define BLOP_FREE(ptr)                         free((void*)ptr)
#define BLOP_MALLOC(type, count)        (type*)malloc(sizeof(type) * (count))
#define BLOP_CALLOC(type, count)        (type*)calloc(count, sizeof(type))
#define BLOP_REALLOC(type, ptr, size)   (type*)realloc((void*)ptr, size)

// Utility Macros

#define BLOP_CAST(type, tocast)   ((type)(tocast))
#define BLOP_TERNARY(cnd, x, y)   ((cnd) ? (x) : (y))

// Mathematical Macros

#define BLOP_MIN(x, y)         BLOP_TERNARY((x > y), y, x)
#define BLOP_MAX(x, y)         BLOP_TERNARY((x > y), x, y)

#define BLOP_DISTANCE(x, y)   (BLOP_TERNARY(x > y, x - y, y - x))
#define BLOP_DIFFERENCE(x, y) (BLOP_TERNARY(x > y, x - y, 0 - (y - x)))

// Ptr arithmetic

#define BLOP_PADD(ptr, value) (void*)((uint8_t*)ptr + value)
#define BLOP_PSUB(ptr, value) (void*)((uint8_t*)ptr - value)

#endif // __BLOP_H__