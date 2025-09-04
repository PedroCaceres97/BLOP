#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define _balloc(type, count) ((type*)calloc(count, sizeof(type)))
#define _bfree(ptr) (free((void*)ptr))

typedef void(*PFN_BlopErrorCallback)(const char* file, uint32_t line, const char* function, const char* message, const char* solution);

#endif // __BLOP_H__