#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Free callback must return zero on faile and non-zero on success
typedef int  (*PFN_BlopFreeCallback)(void* ptr);
typedef void*(*PFN_BlopAllocCallback)(size_t size);
typedef void*(*PFN_BlopReallocCallback)(void* ptr, size_t size);

typedef void (*PFN_BlopErrorCallback)(const char* file, uint32_t line, const char* function, const char* message);

#ifdef __BLOP_DEFAULT_CALLBACKS__

#define blop_error(message) __blop_error(__FILE__, __LINE__, __FUNCTION__, message)

extern PFN_BlopFreeCallback     __blop_free; 
extern PFN_BlopAllocCallback    __blop_alloc;
extern PFN_BlopReallocCallback  __blop_realloc;
extern PFN_BlopErrorCallback    __blop_error;

#endif // __BLOP_DEFAULT_CALLBACKS__

#endif // __BLOP_H__