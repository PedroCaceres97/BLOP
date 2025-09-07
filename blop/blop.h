#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    BlopNullException       = -1
    BlopSuccess             =  0
    BlopLogicalException    =  1
} BlopErrorCode;

typedef int  (*PFN_BlopFreeCallback)(void* ptr);
typedef void*(*PFN_BlopAllocCallback)(size_t size);
typedef void*(*PFN_BlopReallocCallback)(void* ptr, size_t size);

typedef void (*PFN_BlopErrorCallback)(const char* file, uint32_t line, const char* function, const char* message);

#ifdef __BLOP_DEFAULT_CALLBACKS__

#define blop_free(ptr)                   __blop_free((void*)ptr)
#define blop_free_if(ptr, cnd) if (cnd) {__blop_free((void*)ptr)}
#define blop_alloc(type, count)   (type*)__blop_alloc(sizeof(type) * count)
#define blop_realloc(type, size)  (type*)__blop_realloc(size)
#define blop_error(message)              __blop_error(__FILE__, __LINE__, __FUNCTION__, message)

extern PFN_BlopFreeCallback     __blop_free; 
extern PFN_BlopAllocCallback    __blop_alloc;
extern PFN_BlopReallocCallback  __blop_realloc;
extern PFN_BlopErrorCallback    __blop_error;

#endif // __BLOP_DEFAULT_CALLBACKS__

#endif // __BLOP_H__