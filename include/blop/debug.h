#ifndef __BLOP_DEBUG_H__
#define __BLOP_DEBUG_H__

#include <blop/blop.h>

typedef struct _BlopDebugPool_t* BlopDebugPool;

#ifdef __BLOP_SHOW_DEBUG_IMPLEMENTATION__

#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>

struct _BlopDebugPool_t {
    size_t              total;
    struct _BlopList_t  list;
    const char*         alias;
    const char*         file;
    uint32_t            line;
    const char*         function;
};

struct _BlopDebugPtrHeader_t {
    long                        magic;
    struct _BlopNode_t          node;
    struct _BlopDebugPool_t*    pool;
    size_t                      size;
    const char*                 alias;
    const char*                 file;
    uint32_t                    line;
    const char*                 function;
};

#endif // __BLOP_SHOW_DEBUG_IMPLEMENTATION__

BlopResult    __BlopNewDebugPool     (BlopDebugPool* buffer, int mute, const char* alias, const char* file, uint32_t line, const char* function);
BlopResult    __BlopFreeDebugPool    (BlopDebugPool pool, int mute);

BlopResult    __BlopDebugFree        (BlopDebugPool pool, void* ptr, int mute);
BlopResult    __BlopDebugAlloc       (BlopDebugPool pool, void** buffer, size_t size, int mute, const char* alias, const char* file, uint32_t line, const char* function);
BlopResult    __BlopDebugRealloc     (BlopDebugPool pool, void** buffer, void* ptr, size_t size, int mute, const char* alias, const char* file, uint32_t line, const char* function);
BlopResult    __BlopDebugDuplicate   (BlopDebugPool pool, void** buffer, void* ptr, size_t size, int mute, const char* alias, const char* file, uint32_t line, const char* function);
BlopResult    __BlopDebugPoolClean   (BlopDebugPool pool, int mute, int verbose);

BlopResult    __BlopDebugPtrGetSize  (BlopDebugPool pool, void* ptr, size_t* buffer);
BlopResult    __BlopDebugPoolGetTotal(BlopDebugPool pool, size_t* buffer);
BlopResult    __BlopDebugPoolGetCount(BlopDebugPool pool, size_t* buffer);

BlopResult    __BlopDebugPoolPrint   (BlopDebugPool pool);

#endif // __BLOP_DEBUG_H__