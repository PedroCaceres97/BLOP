#ifndef __BLOP_MEMORY_H__
#define __BLOP_MEMORY_H__

#include <blop/blop.h>

typedef struct _BlopPool_t* BlopPool;

#ifdef __BLOP_SHOW_MEMORY_IMPLEMENTATION__

#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>

struct _BlopPool_t {
    size_t              total;
    struct _BlopList_t  list;
};

struct _BlopPtrHeader_t {
    long                magic;
    struct _BlopNode_t  node;
    struct _BlopPool_t* pool;
    size_t              size;
};

#endif // __BLOP_SHOW_MEMORY_IMPLEMENTATION__

BlopResult BlopNewPool          (BlopPool* buffer);
BlopResult BlopFreePool         (BlopPool pool);

BlopResult BlopPoolFree         (BlopPool pool, void* ptr);
BlopResult BlopPoolAlloc        (BlopPool pool, void** buffer, size_t size);
BlopResult BlopPoolRealloc      (BlopPool pool, void** buffer, void* ptr, size_t size);
BlopResult BlopPoolDuplicate    (BlopPool pool, void** buffer, void* ptr, size_t size);
BlopResult BlopPoolClean        (BlopPool pool);

BlopResult BlopPoolPtrGetSize   (void* ptr, size_t* buffer);
BlopResult BlopPoolGetTotal     (BlopPool pool, size_t* buffer);
BlopResult BlopPoolGetCount     (BlopPool pool, size_t* buffer);

BlopResult BlopPoolPrint        (BlopPool pool);

#endif // __BLOP_MEMORY_H__