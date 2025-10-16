#ifndef __BLOP_POOL_H__
#define __BLOP_POOL_H__

#include <blop/blop.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <blop/blop.h>

#ifndef BLOP_POOL_NAME
    #define BLOP_POOL_NAME BPool
#endif

#ifdef BLOP_POOL_SAFE_MODE
    #define BLOP_POOL_ASSERT_PTR(ptr, rt) if (ptr == NULL) {BLOP_ERROR_MESSAGE(#ptr " is a null ptr (returning without effect)"); BLOP_LIST_EXIT; return rt;}
    #define BLOP_POOL_ASSERT_PTR_VOID(ptr) if (ptr == NULL) {BLOP_ERROR_MESSAGE(#ptr " is a null ptr (returning without effect)"); BLOP_LIST_EXIT; return;}
#else
    #define BLOP_POOL_ASSERT_PTR(ptr, rt)
    #define BLOP_POOL_ASSERT_PTR_VOID(ptr)
#endif

#ifdef BLOP_POOL_EXIT_ON_ERROR
    #define BLOP_POOL_EXIT exit(-1)
#else
    #define BLOP_POOL_EXIT
#endif

#ifndef BLOP_POOL_CAMEL

    #define _BLOPS_POOL             BLOP_CONCAT3(_, BLOP_POOL_NAME, _t)
    #define _BLOPS_PTRHDR           BLOP_CONCAT3(_, BLOP_POOL_NAME, _ptrhdr_t)

    #define _BLOPT_POOL             BLOP_POOL_NAME
    #define _BLOPT_PTRHDR           BLOP_CONCAT2(BLOP_POOL_NAME, _ptrhdr)

    #define _BLOPF_POOL_CREATE      BLOP_CONCAT2(BLOP_POOL_NAME, _create)
    #define _BLOPF_POOL_DESTROY     BLOP_CONCAT2(BLOP_POOL_NAME, _destroy)

    #define _BLOPF_POOL_FREE        BLOP_CONCAT2(BLOP_POOL_NAME, _free)
    #define _BLOPF_POOL_FREE_ALL    BLOP_CONCAT2(BLOP_POOL_NAME, _free_all)
    #define _BLOPF_POOL_MALLOC      BLOP_CONCAT2(BLOP_POOL_NAME, _malloc)
    #define _BLOPF_POOL_CALLOC      BLOP_CONCAT2(BLOP_POOL_NAME, _calloc)
    #define _BLOPF_POOL_REALLOC     BLOP_CONCAT2(BLOP_POOL_NAME, _realloc)
    #define _BLOPF_POOL_DUPLICATE   BLOP_CONCAT2(BLOP_POOL_NAME, _duplicate)

    #define _BLOPF_POOL_PRINT       BLOP_CONCAT2(BLOP_POOL_NAME, _print)
    #define _BLOPF_POOL_GET_BYTES   BLOP_CONCAT2(BLOP_POOL_NAME, _get_bytes)
    #define _BLOPF_POOL_GET_COUNT   BLOP_CONCAT2(BLOP_POOL_NAME, _get_count)

#else // BLOP_LIST_CAMEL

    #define _BLOPS_POOL             BLOP_CONCAT3(_, BLOP_POOL_NAME, _t)
    #define _BLOPS_PTRHDR           BLOP_CONCAT3(_, BLOP_POOL_NAME, PtrHdr_t)

    #define _BLOPT_POOL             BLOP_POOL_NAME
    #define _BLOPT_PTRHDR           BLOP_CONCAT2(BLOP_POOL_NAME, PtrHdr)

    #define _BLOPF_POOL_CREATE      BLOP_CONCAT2(BLOP_POOL_NAME, Create)
    #define _BLOPF_POOL_DESTROY     BLOP_CONCAT2(BLOP_POOL_NAME, Destroy)

    #define _BLOPF_POOL_FREE        BLOP_CONCAT2(BLOP_POOL_NAME, Free)
    #define _BLOPF_POOL_FREE_ALL    BLOP_CONCAT2(BLOP_POOL_NAME, FreeAll)
    #define _BLOPF_POOL_MALLOC      BLOP_CONCAT2(BLOP_POOL_NAME, Malloc)
    #define _BLOPF_POOL_CALLOC      BLOP_CONCAT2(BLOP_POOL_NAME, Calloc)
    #define _BLOPF_POOL_REALLOC     BLOP_CONCAT2(BLOP_POOL_NAME, Realloc)
    #define _BLOPF_POOL_DUPLICATE   BLOP_CONCAT2(BLOP_POOL_NAME, Duplicate)

    #define _BLOPF_POOL_PRINT       BLOP_CONCAT2(BLOP_POOL_NAME, Print)
    #define _BLOPF_POOL_GET_BYTES   BLOP_CONCAT2(BLOP_POOL_NAME, GetBytes)
    #define _BLOPF_POOL_GET_COUNT   BLOP_CONCAT2(BLOP_POOL_NAME, GetCount)

#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _BLOPT_POOL_LIST        BLOP_CONCAT2(BLOP_POOL_NAME, _ptrs)
#define _BLOPT_POOL_LIST_NODE   BLOP_CONCAT2(BLOP_POOL_NAME, _ptrs_node)

#define _BLOPS_POOL_LIST        BLOP_CONCAT3(_, BLOP_POOL_NAME, _ptrs_t)
#define _BLOPS_POOL_LIST_NODE   BLOP_CONCAT3(_, BLOP_POOL_NAME, _ptrs_node_t)

#define _BLOPF_POOL_LIST_GET_FRONT  BLOP_CONCAT2(BLOP_POOL_NAME, _ptrs_get_front)

#define _BLOPF_POOL_LIST_ERASE      BLOP_CONCAT2(BLOP_POOL_NAME, _ptrs_erase)
#define _BLOPF_POOL_LIST_POP_FRONT  BLOP_CONCAT2(BLOP_POOL_NAME, _ptrs_pop_front)

#define BLOP_LIST_NAME _BLOPT_POOL_LIST
#define BLOP_LIST_DATA_TYPE void*
#ifdef BLOP_POOL_CAMEL
    #define BLOP_LIST_CAMEL
#endif
#ifdef BLOP_POOL_SAFE_MODE
    #define BLOP_LIST_SAFE_MODE
#endif
#ifdef BLOP_POOL_EXIT_ON_ERROR
    #define BLOP_LIST_EXIT_ON_ERROR
#endif
#include <blop/list.h>

typedef struct _BLOPS_POOL*     _BLOPT_POOL;
typedef struct _BLOPS_PTRHDR*   _BLOPT_PTRHDR;

_BLOPT_POOL _BLOPF_POOL_CREATE();
void        _BLOPF_POOL_DESTROY(_BLOPT_POOL pool);

void        _BLOPF_POOL_FREE(_BLOPT_POOL pool, void* ptr);
void        _BLOPF_POOL_FREE_ALL(_BLOPT_POOL pool);
void*       _BLOPF_POOL_MALLOC(_BLOPT_POOL pool, size_t size);
void*       _BLOPF_POOL_CALLOC(_BLOPT_POOL pool, size_t count, size_t size);
void*       _BLOPF_POOL_REALLOC(_BLOPT_POOL pool, void* ptr, size_t size);
void*       _BLOPF_POOL_DUPLICATE(_BLOPT_POOL pool, void* ptr, size_t size);

void        _BLOPF_POOL_PRINT(_BLOPT_POOL pool);
size_t      _BLOPF_POOL_GET_BYTES(_BLOPT_POOL pool);
size_t      _BLOPF_POOL_GET_COUNT(_BLOPT_POOL pool);

#ifdef BLOP_POOL_IMPLEMENTATION

#define BLOP_LIST_NAME      _BLOPT_POOL_LIST
#define BLOP_LIST_DATA_TYPE _BLOPT_PTRHDR
#ifdef BLOP_POOL_SAFE_MODE
    #define BLOP_LIST_SAFE_MODE
#endif
#ifdef BLOP_POOL_EXIT_ON_ERROR
    #define BLOP_LIST_EXIT_ON_ERROR
#endif
#define BLOP_LIST_IMPLEMENTATION
#include <blop/list.h>

struct _BLOPS_PTRHDR {
    size_t   size;
    _BLOPT_POOL pool;
    struct _BLOPS_POOL_LIST_NODE node;
};
struct _BLOPS_POOL {
    size_t                  bytes;
    struct _BLOPS_POOL_LIST ptrs;
};

_BLOPT_POOL _BLOPF_POOL_CREATE() {
    _BLOPT_POOL pool = BLOP_MALLOC(struct _BLOPS_POOL, 1);
    if (!pool) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for pool (returning NULL)");
        BLOP_POOL_EXIT;
        return NULL;
    }

    memset(pool, 0, sizeof(struct _BLOPS_POOL));
    return pool;
}
void        _BLOPF_POOL_DESTROY(_BLOPT_POOL pool) {
    BLOP_POOL_ASSERT_PTR_VOID(pool);

    if (pool->ptrs.size > 0) {
        BLOP_ERROR_MESSAGE("The pool is not empty, you must free all allocated memory before destroying the pool (returning without effect)");
        BLOP_POOL_EXIT;
        return;
    }

    free(pool);
}

void        _BLOPF_POOL_FREE(_BLOPT_POOL pool, void* ptr) {
    BLOP_POOL_ASSERT_PTR_VOID(pool);
    BLOP_POOL_ASSERT_PTR_VOID(ptr);

    _BLOPT_PTRHDR hdr = (_BLOPT_PTRHDR)BLOP_PSUB(ptr, sizeof(struct _BLOPS_PTRHDR));
    if (hdr->pool != pool) {
        BLOP_ERROR_MESSAGE("The pointer does not belong to this pool (returning without effect)");
        BLOP_POOL_EXIT;
        return;
    }

    _BLOPF_POOL_LIST_ERASE(&pool->ptrs, &hdr->node, false);
    pool->bytes -= hdr->size;
    free(hdr);
}
void        _BLOPF_POOL_FREE_ALL(_BLOPT_POOL pool) {
    BLOP_POOL_ASSERT_PTR_VOID(pool);

    _BLOPT_POOL_LIST_NODE current = BLOP_LIST_GET_FRONT(&pool->ptrs);
    while (current != NULL) {
        _BLOPF_POOL_LIST_POP_FRONT(&pool->ptrs, false);
        _BLOPT_PTRHDR hdr = current->data;
        pool->bytes -= hdr->size;
        free(hdr);
        current = (_BLOPT_PTRHDR)BLOP_LIST_GET_FRONT(&pool->ptrs);
    }
}
void*       _BLOPF_POOL_MALLOC(_BLOPT_POOL pool, size_t size) {
    BLOP_POOL_ASSERT_PTR(pool, NULL);
    if (size == 0) {
        BLOP_ERROR_MESSAGE("Requested allocation size is 0 (returning NULL)");
        BLOP_POOL_EXIT;
        return NULL;
    }

    _BLOPT_PTRHDR hdr = BLOP_ALLOC(struct _BLOPS_PTRHDR, size + sizeof(struct _BLOPS_PTRHDR));
    if (!hdr) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for pointer header (returning NULL)");
        BLOP_POOL_EXIT;
        return NULL;
    }

    hdr->size = size;
    hdr->pool = pool;
    
    if (BLOP_LIST_PUSH_BACK(&pool->ptrs, &hdr->node) != BlopSuccess) {
        BLOP_ERROR_MESSAGE("Failed to push back header->node (returning NULL)");
        free(hdr);
        BLOP_POOL_EXIT;
        return NULL;
    }
    pool->bytes += size;

    void* ptr = BLOP_PADD(hdr, sizeof(struct _BLOPS_PTRHDR));
    return ptr;
}
void*       _BLOPF_POOL_CALLOC(_BLOPT_POOL pool, size_t count, size_t size);
void*       _BLOPF_POOL_REALLOC(_BLOPT_POOL pool, void* ptr, size_t size);
void*       _BLOPF_POOL_DUPLICATE(_BLOPT_POOL pool, void* ptr, size_t size);

void        _BLOPF_POOL_PRINT(_BLOPT_POOL pool);

size_t      _BLOPF_POOL_GET_BYTES(_BLOPT_POOL pool) {
    BLOP_POOL_ASSERT_PTR(pool, 0);
    return pool->bytes;
}
size_t      _BLOPF_POOL_GET_COUNT(_BLOPT_POOL pool) {
    BLOP_POOL_ASSERT_PTR(pool, 0);
    return pool->ptrs.size;
}

#endif // BLOP_POOL_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // __BLOP_POOL_H__