#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    BlopSuccess             ,      
    BlopNullException       , 
    BlopIndexException      , 
    BlopAllocationFailed    , 
    BlopDeallocationlFailed , 
    BlopLoopedCallback      ,
    BlopWrongSignature      ,
    BlopNonEmptyStructure   ,
    BlopNoAllocation        ,
} BlopResult;

typedef BlopResult  (*PFN_BlopFreeCallback)(void* ptr);
typedef void*       (*PFN_BlopAllocCallback)(size_t size);
typedef void*       (*PFN_BlopReallocCallback)(void* ptr, size_t size);

typedef void        (*PFN_BlopLogCallback)(const char* file, uint32_t line, const char* function, const char* message);
typedef void        (*PFN_BlopExitCallback)(int code);

BlopResult BlopSetFreeCallback   (PFN_BlopFreeCallback callback);
BlopResult BlopSetAllocCallback  (PFN_BlopAllocCallback callback);
BlopResult BlopSetReallocCallback(PFN_BlopReallocCallback callback);
BlopResult BlopSetErrorCallback  (PFN_BlopLogCallback callback);
BlopResult BlopSetDebugCallback  (PFN_BlopLogCallback callback);

const char* BlopResultToString(BlopResult result);

#ifdef __BLOP_DEFAULT_CALLBACKS__

#define blop_free(ptr)                       __blop_free((void*)ptr)
#define blop_free_if(cnd, ptr)     if (cnd) {__blop_free((void*)ptr);}
#define blop_alloc(type, size)        (type*)__blop_alloc(size)
#define blop_calloc(type, count)      (type*)__blop_alloc(sizeof(type) * count)
#define blop_realloc(type, ptr, size) (type*)__blop_realloc(ptr, size)
#define blop_error(message)                  __blop_error(__FILE__, __LINE__, __FUNCTION__, message)
#define blop_debug(message)                  __blop_debug(__FILE__, __LINE__, __FUNCTION__, message)
#define blop_debug_if(cnd, message) if (cnd) { blop_debug(message); }

#define blop_assert_null(ptr)       if (ptr == NULL) {blop_error(BlopResultToString(BlopNullException));  return BlopNullException; }
#define blop_assert_index(idx, max) if (idx >= max)  {blop_error(BlopResultToString(BlopIndexException)); return BlopIndexException; }

extern PFN_BlopFreeCallback     __blop_free; 
extern PFN_BlopAllocCallback    __blop_alloc;
extern PFN_BlopReallocCallback  __blop_realloc;
extern PFN_BlopLogCallback      __blop_error;
extern PFN_BlopLogCallback      __blop_debug;

#endif // __BLOP_DEFAULT_CALLBACKS__

#endif // __BLOP_H__