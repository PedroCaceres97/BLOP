#ifndef __BLOP_UTILS_H__
#define __BLOP_UTILS_H__

#include <blop/blop.h>

// Conditional Macros
#define ternary(cnd, x, y) ((cnd) ? (x) : (y))

#define asign_if(cnd, var, value) if (cnd) {var = value;}

#define return_if(cnd, value) if (cnd) { return value; } 
#define return_verbose(value, message) blop_error(message); return value;
#define return_verbose_if(cnd, value, message) if (cnd) {blop_error(message); return value;}


// Mathematical Macros
#define distance(x, y) (ternary(x > y, x - y, y - x))

const char* BlopUtilsPathLast(const char* path);

#endif // __BLOP_UTILS_H__