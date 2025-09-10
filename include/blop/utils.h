#ifndef __BLOP_UTILS_H__
#define __BLOP_UTILS_H__

#include <blop/blop.h>

// Conditional Macros
#define blop_ternary(cnd, x, y) ((cnd) ? (x) : (y))
#define blop_asign_if(cnd, var, value) if (cnd) {var = value;}

// Mathematical Macros
#define blop_min(x, y) ((x > y) ? y : x)
#define blop_max(x, y) ((x > y) ? x : y)

#define blop_distance(x, y) (blop_ternary(x > y, x - y, y - x))
#define blop_add_difference(var, x, y) if (x > y) {var += x - y;} else {var -= y - x;}

// Pointer arithmetic
#define blop_ptr_add(ptr, value) (void*)((uint8_t*)ptr + value)
#define blop_ptr_sub(ptr, value) (void*)((uint8_t*)ptr - value)

const char* BlopUtilsPathLast(const char* path);

#endif // __BLOP_UTILS_H__