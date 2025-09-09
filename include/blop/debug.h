#ifndef __BLOP_DEBUG_H__
#define __BLOP_DEBUG_H__

#include <blop/blop.h>

typedef struct _BlopDebugPool_t* BlopDebugPool;

#ifdef __BLOP_SHOW_DEBUG_IMPLEMENTATION__

#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>

struct _BlopDebugPool_t {
    BlopList            list;
    size_t              total;
    const char*         alias;
    const char*         file;
    const char*         line;
    const char*         function;
};

struct _BlopDebugPtrHeader_t {
    long                        magic;
    struct _BlopNode_t          node;
    struct _BlopDebugPool_t*    pool;
    size_t                      size;
    const char*                 alias;
    const char*                 file;
    const char*                 line;
    const char*                 function;
};

#endif // __BLOP_SHOW_DEBUG_IMPLEMENTATION__

BlopPool    __BlopNewDebugPool     (const char* alias, const char* file, const char* line, const char* funtion);
int         __BlopFreeDebugPool    (BlopPool pool);

int         __BlopDebugFree        (BlopDebugPool pool, void* ptr);
int         __BlopDebugEmpty       (BlopDebugPool pool);
void*       __BlopDebugAlloc       (BlopDebugPool pool, size_t size);
void*       __BlopDebugRealloc     (BlopDebugPool pool, void* ptr, size_t size);
void*       __BlopDebugDuplicate   (BlopDebugPool pool, void* ptr, size_t size);

int         __BlopDebugPoolPrint   (BlopDebugPool pool);

size_t      __BlopGetDebugPtrSize  (void* ptr);
size_t      __BlopGetDebugPoolSize (BlopDebugPool pool);

#endif // __BLOP_DEBUG_H__