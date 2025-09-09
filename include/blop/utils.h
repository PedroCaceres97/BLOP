#ifndef __BLOP_UTILS_H__
#define __BLOP_UTILS_H__

#include <blop/blop.h>

// Conditional Macros
#define ternary(cnd, x, y) ((cnd) ? (x) : (y))

#define asign_if(cnd, var, value) if (cnd) {var = value;}

// Mathematical Macros
#define min(x, y) ((x > y) ? y : x)
#define max(x, y) ((x > y) ? x : y)

#define distance(x, y) (ternary(x > y, x - y, y - x))
#define add_difference(var, x, y) if (x > y) {var += x - y;} else {var -= y - x;}

// Pointer arithmetic
#define ptr_add(ptr, value) (void*)((uint8_t*)ptr + value)
#define ptr_sub(ptr, value) (void*)((uint8_t*)ptr - value)

const char* BlopUtilsPathLast(const char* path);

#endif // __BLOP_UTILS_H__