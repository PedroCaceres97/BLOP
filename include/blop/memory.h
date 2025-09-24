#ifndef __BLOP_MEMORY_H__
#define __BLOP_MEMORY_H__

#include <blop/blop.h>

typedef struct _BlopPool_t* BlopPool;

#ifdef __BLOP_SHOW_MEMORY_IMPLEMENTATION__

#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>

struct _BmmPool_t {
    size_t              total;
    struct _BlopList_t  list;
};

struct _BmmPtrHeader_t {
    long                magic;
    struct _BlopNode_t  node;
    struct _BmmPool_t*  pool;
    size_t              size;
};

struct _BdmmPool_t {
    size_t              total;
    struct _BlopList_t  list;
    const char*         alias;
    const char*         file;
    uint32_t            line;
    const char*         function;
};

struct _BdmmPtrHeader_t {
    long                        magic;
    struct _BlopNode_t          node;
    struct _BdmmPool_t*         pool;
    size_t                      size;
    const char*                 alias;
    const char*                 file;
    uint32_t                    line;
    const char*                 function;
};

#endif // __BLOP_SHOW_MEMORY_IMPLEMENTATION__

BlopPool        BlopCreatePool(BlopApplication app);
void            BlopDestroyPool(BlopPool pool);
size_t          BlopPoolCount(BlopPool pool);
size_t          BlopPoolTotal(BlopPool pool);
void            BlopPoolPrint(BlopPool pool);
void            BlopPoolClean(BlopPool pool);

void*           BlopAlloc(BlopPool pool, size_t size);
void*           BlopRealloc(BlopPool pool, void* ptr, size_t size);
void*           BlopDuplicate(BlopPool pool, void* ptr, size_t size);
void            BlopFree(void* ptr);

#define BlopCreateDebugPool(app, alias)    __BlopCreateDebugPool(app, alias, __FILE__, __LINE__, __FUNCTION__)
#define BlopDebugAlloc(pool, size, alias)    __BlopDebugAlloc(pool, size, alias, __FILE__, __LINE__, __FUNCTION__)
#define BlopDebugRealloc(pool, ptr, size, alias) __BlopDebugRealloc(pool, ptr, size, alias, __FILE__, __LINE__, __FUNCTION__)
#define BlopDebugDuplicate(pool, ptr, size, alias) __BlopDebugDuplicate(pool, ptr, size, alias, __FILE__, __LINE__, __FUNCTION__)

BlopPool      __BlopCreateDebugPool(BlopApplication app, const char* alias, const char* file, uint32_t line, const char* function);
void            BlopDestroyDebugPool(BlopPool pool);
size_t          BlopDebugPoolCount(BlopPool pool);
size_t          BlopDebugPoolTotal(BlopPool pool);
void            BlopDebugPoolPrint(BlopPool pool);
void            BlopDebugPoolClean(BlopPool pool);

void*         __BlopDebugAlloc(BlopPool pool, size_t size, const char* alias, const char* file, uint32_t line, const char* function);
void*         __BlopDebugRealloc(BlopPool pool, void* ptr, size_t size, const char* alias, const char* file, uint32_t line, const char* function);
void*         __BlopDebugDuplicate(BlopPool pool, void* ptr, size_t size, const char* alias, const char* file, uint32_t line, const char* function);
void            BlopDebugFree(void* ptr);

#endif // __BLOP_MEMORY_H__