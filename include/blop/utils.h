#ifndef __BLOP_UTILS_H__
#define __BLOP_UTILS_H__

#include <blop/blop.h>

// Standart Memory Macros
#define bm_free(ptr)                    free((void*)ptr)
#define bm_malloc(type, size)    (type*)malloc(size)
#define bm_calloc(type, count)   (type*)calloc(count, sizeof(type))

// Conditional Macros
#define bm_ternary(cnd, x, y) ((cnd) ? (x) : (y))
#define bm_asign_if(cnd, var, value) if (cnd) {var = value;}

// Mathematical Macros
#define bm_min(x, y) ((x > y) ? y : x)
#define bm_max(x, y) ((x > y) ? x : y)

#define bm_distance(x, y) (blop_ternary(x > y, x - y, y - x))
#define bm_add_difference(var, x, y) if (x > y) {var += x - y;} else {var -= y - x;}

// Pointer arithmetic
#define bm_ptr_add(ptr, value) (void*)((uint8_t*)ptr + value)
#define bm_ptr_sub(ptr, value) (void*)((uint8_t*)ptr - value)

const char* BlopUtilsPathLast(const char* path);

#endif // __BLOP_UTILS_H__