#ifndef __BLOP_MEMORY_H__
#define __BLOP_MEMORY_H__

#include <blop/blop.h>

typedef struct _BlopPool_t* BlopPool;

#ifdef __BLOP_SHOW_MEMORY_IMPLEMENTATION__

#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>

struct _BlopPool_t {
    size_t              total;
    const char*         alias;
    BlopList            list;
};

struct _BlopPtrHeader_t {
    long                magic;
    struct _BlopNode_t  node;
    struct _BlopPool_t* pool;
    size_t              size;
    const char*         alias;
};

#endif // __BLOP_SHOW_MEMORY_IMPLEMENTATION__

BlopPool    BlopNewPool     ();
int         BlopFreePool    (BlopPool pool);

int         BlopFree        (void* ptr);
int         BlopEmpty       (BlopPool pool);
void*       BlopAlloc       (BlopPool pool, size_t size);
void*       BlopRealloc     (BlopPool pool, void* ptr, size_t size);
void*       BlopDuplicate   (BlopPool pool, void* ptr, size_t size);

int         BlopPoolPrint   (BlopPool pool);

size_t      BlopGetPtrSize  (void* ptr);
const char* BlopGetPtrAlias (void* ptr);
int         BlopSetPtrAlias (void* ptr, const char* alias);

size_t      BlopGetPoolSize (BlopPool pool);
const char* BlopGetPoolAlias(BlopPool pool);
int         BlopSetPoolAlias(BlopPool pool, const char* alias);

#endif // __BLOP_MEMORY_H__