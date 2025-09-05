#ifndef __BLOP_MEMORY_H__
#define __BLOP_MEMORY_H__

#include <stdint.h>
#include <stdlib.h>

#include "../Blop.h"

typedef struct _BlopPool_t* BlopPool;

BlopPool    BlopNewPool();
int         BlopFreePool(BlopPool pool);

void        BlopFree(void* ptr);
void        BlopEmpty(BlopPool pool);
void*       BlopAlloc(BlopPool pool, size_t size);
void*       BlopRealloc(BlopPool pool, void* ptr, size_t size);
void*       BlopDuplicate(BlopPool pool, void* ptr, size_t size);

void        BlopPoolPrint(BlopPool pool);

size_t      BlopGetPtrSize  (void* ptr);
const char* BlopGetPtrAlias (void* ptr);
void        BlopSetPtrAlias (void* ptr, const char* alias);

size_t      BlopGetPoolSize (BlopPool pool);
const char* BlopGetPoolAlias(BlopPool pool);
void        BlopSetPoolAlias(BlopPool pool, const char* alias);

void        BlopMemoryErrorCallback(PFN_BlopErrorCallback callback);

#endif // __BLOP_MEMORY_H__