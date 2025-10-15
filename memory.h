#ifndef __BLOP_MEMORY_H__
#define __BLOP_MEMORY_H__

#include <blop/blop.h>

typedef struct _BMPOOL_t* BMPOOL;
typedef struct _BDPOOL_t* BDPOOL;

#ifdef __BLOP_SHOW_MEMORY_IMPLEMENTATION__

#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>

struct _BMPOOL_t {
    size_t              total;
    struct _BLIST_t     list;
};

struct _PtrHeader_t {
    long                magic;
    struct _BNODE_t     node;
    struct _BMPOOL_t*   pool;
    size_t              size;
};

struct _BDInfo_t {
    const char*         alias;
    const char*         file;
    uint32_t            line;
    const char*         function;
}

struct _BDPOOL_t {
    size_t              total;
    struct _BLIST_t     list;
    struct _BDInfo_t    info;
};

struct _DPtrHeader_t {
    long                magic;
    struct _BNODE_t     node;
    struct _BMPOOL_t*   pool;
    size_t              size;
    struct _BDInfo_t    info;
};

#endif // __BLOP_SHOW_MEMORY_IMPLEMENTATION__

BMPOOL    BMPOOL_Create();
void      BMPOOL_Destroy(BMPOOL pool);
size_t    BMPOOL_Count(BMPOOL pool);
size_t    BMPOOL_Total(BMPOOL pool);
void      BMPOOL_Print(BMPOOL pool);
void      BMPOOL_Clean(BMPOOL pool);

void      BMPOOL_Free(void* ptr);
void*     BMPOOL_Alloc(BMPOOL pool, size_t size);
void*     BMPOOL_Realloc(BMPOOL pool, void* ptr, size_t size);
void*     BMPOOL_Duplicate(BMPOOL pool, void* ptr, size_t size);


#define   BDPOOL_Create(alias) __BDPOOL_Create(alias, __FILE__, __LINE__, __FUNCTION__)
BDPOOL  __BDPOOL_Create(const char* alias, const char* file, uint32_t line, const char* function);
void      BDPOOL_Destroy(BDPOOL pool);
void      BDPOOL_Print(BDPOOL pool);
void      BDPOOL_Clean(BDPOOL pool);
size_t    BDPOOL_Count(BDPOOL pool);
size_t    BDPOOL_Total(BDPOOL pool);

void      BDPOOL_Free(void* ptr);
#define   BDPOOL_Alloc(pool, size, alias) __BDPOOL_Alloc(pool, size, alias, __FILE__, __LINE__, __FUNCTION__)
void*   __BDPOOL_Alloc(BDPOOL pool, size_t size, const char* alias, const char* file, uint32_t line, const char* function);
#define   BDPOOL_Realloc(pool, ptr, size, alias) __BDPOOL_Realloc(pool, ptr, size, alias, __FILE__, __LINE__, __FUNCTION__)
void*   __BDPOOL_Realloc(BDPOOL pool, void* ptr, size_t size, const char* alias, const char* file, uint32_t line, const char* function);
#define   BDPOOL_Duplicate(pool, ptr, size, alias) __BDPOOL_Duplicate(pool, ptr, size, alias, __FILE__, __LINE__, __FUNCTION__)
void*   __BDPOOL_Duplicate(BDPOOL pool, void* ptr, size_t size, const char* alias, const char* file, uint32_t line, const char* function);

#endif // __BLOP_MEMORY_H__