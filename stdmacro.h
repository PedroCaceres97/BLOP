#ifndef __STDMACRO_H__
#define __STDMACRO_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// C Language Macros

#define stdm_cast(type, tocast) ((type)(tocast))
#define stdm_ternary(cnd, x, y) ((cnd) ? (x) : (y))

// Mathematical Macros

#define stdm_min(x, y) stdm_ternary((x > y), y, x)
#define stdm_max(x, y) stdm_ternary((x > y), x, y)

#define stdm_distance(x, y) (stdm_ternary(x > y, x - y, y - x))
#define stdm_difference(x, y) (stdm_ternary(x > y, x - y, 0 - (y - x)))

// Ptr arithmetic

#define stdm_padd(ptr, value) (void*)((uint8_t*)ptr + value)
#define stdm_psub(ptr, value) (void*)((uint8_t*)ptr - value)

// stdlib Macros

#define stdm_free(ptr)                         free((void*)ptr)
#define stdm_malloc(type, size)         (type*)malloc(size)
#define stdm_calloc(type, count)        (type*)calloc(count, sizeof(type))
#define stdm_realloc(type, ptr, size)   (type*)realloc((void*)ptr, size)

#endif // __STDMACRO_H__