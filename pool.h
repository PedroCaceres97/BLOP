#ifndef __BLOP_POOL_H__
#define __BLOP_POOL_H__

#include <blop/blop.h>

#ifndef BLOP_POOL_NAME
    #define BLOP_POOL_NAME BPool
#endif

#define blopt_pool                  BLOP_POOL_NAME
#define blopt_ptrhdr                BLOP_CONCAT2(blopt_pool, _ptrhdr)

#define blops_pool                  BLOP_CONCAT3(_, blopt_pool, _t)
#define blops_ptrhdr                BLOP_CONCAT3(_, blopt_pool, _ptrhdr_t)

#define blopfn_pool_create          BLOP_CONCAT2(blopt_pool, _create)
#define blopfn_pool_destroy         BLOP_CONCAT2(blopt_pool, _destroy)

#define blopfn_pool_free            BLOP_CONCAT2(blopt_pool, _free)
#define blopfn_pool_free_all        BLOP_CONCAT2(blopt_pool, _free_all)
#define blopfn_pool_malloc          BLOP_CONCAT2(blopt_pool, _malloc)
#define blopfn_pool_calloc          BLOP_CONCAT2(blopt_pool, _calloc)
#define blopfn_pool_realloc         BLOP_CONCAT2(blopt_pool, _realloc)
#define blopfn_pool_duplicate       BLOP_CONCAT2(blopt_pool, _duplicate)

#define blopfn_pool_print           BLOP_CONCAT2(blopt_pool, _print)
#define blopfn_pool_get_bytes       BLOP_CONCAT2(blopt_pool, _get_bytes)
#define blopfn_pool_get_count       BLOP_CONCAT2(blopt_pool, _get_count)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_pool*      blopt_pool;
typedef struct blops_ptrhdr*    blopt_ptrhdr;

blopt_pool  blopfn_pool_create      ();
void        blopfn_pool_destroy     (blopt_pool pool);

void        blopfn_pool_free        (blopt_pool pool, void* ptr);
void        blopfn_pool_free_all    (blopt_pool pool);
void*       blopfn_pool_malloc      (blopt_pool pool, size_t size);
void*       blopfn_pool_calloc      (blopt_pool pool, size_t count, size_t size);
void*       blopfn_pool_realloc     (blopt_pool pool, void* ptr, size_t size);
void*       blopfn_pool_duplicate   (blopt_pool pool, void* ptr, size_t size);

void        blopfn_pool_print       (blopt_pool pool);
size_t      blopfn_pool_get_bytes   (blopt_pool pool);
size_t      blopfn_pool_get_count   (blopt_pool pool);

#ifdef BLOP_POOL_IMPLEMENTATION

#define blopt_pool_list                 BLOP_CONCAT2(blopt_pool, _ptrs)
#define blopt_pool_list_node            BLOP_CONCAT2(blopt_pool_list, _node)

#define blops_pool_list                 BLOP_CONCAT3(_, blopt_pool_list, _t)
#define blops_pool_list_node            BLOP_CONCAT3(_, blopt_pool_list_node, _t)

#define blopfn_pool_list_get_front      BLOP_CONCAT2(blopt_pool_list, _get_front)

#define blopfn_pool_list_erase          BLOP_CONCAT2(blopt_pool_list, _erase)
#define blopfn_pool_list_pop_front      BLOP_CONCAT2(blopt_pool_list, _pop_front)

#define blopfn_pool_list_push_back      BLOP_CONCAT2(blopt_pool_list, _push_back)
#define blopfn_pool_list_insert_next    BLOP_CONCAT2(blopt_pool_list, _insert_next)

#define BLOP_LIST_NAME              blopt_pool_list
#define BLOP_LIST_DATA_TYPE         void*
#define BLOP_LIST_IMPLEMENTATION
#include <blop/list.h>

struct blops_ptrhdr {
    size_t                      size;
    blopt_pool                  pool;
    struct blops_pool_list_node node;
};
struct blops_pool {
    size_t                  bytes;
    struct blops_pool_list  ptrs;
};

blopt_pool  blopfn_pool_create() {
    blopt_pool pool = BLOP_MALLOC(struct blops_pool, 1);
    if (!pool) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for pool (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    memset(pool, 0, sizeof(struct blops_pool));
    return pool;
}
void        blopfn_pool_destroy(blopt_pool pool) {
    BLOP_ASSERT_PTR_VOID(pool);

    if (pool->ptrs.size > 0) {
        BLOP_ERROR_MESSAGE("The pool is not empty, you must free all allocated memory before destroying the pool (returning without effect)");
        BLOP_ABORT();
        return;
    }

    free(pool);
}

void        blopfn_pool_free(blopt_pool pool, void* ptr) {
    BLOP_ASSERT_PTR_VOID(pool);
    BLOP_ASSERT_PTR_VOID(ptr);

    blopt_ptrhdr hdr = (blopt_ptrhdr)BLOP_PSUB(ptr, sizeof(struct blops_ptrhdr));
    if (hdr->pool != pool) {
        BLOP_ERROR_MESSAGE("The pointer does not belong to this pool (returning without effect)");
        BLOP_ABORT();
        return;
    }

    blopfn_pool_list_erase(&pool->ptrs, &hdr->node, false);
    pool->bytes -= hdr->size;
    free(hdr);
}
void        blopfn_pool_free_all(blopt_pool pool) {
    BLOP_ASSERT_PTR_VOID(pool);

    blopt_pool_list_node current = blopfn_pool_get_front(&pool->ptrs);
    while (current != NULL) {
        blopfn_pool_pop_front(&pool->ptrs, false);
        blopt_ptrhdr hdr = current->data;
        pool->bytes -= hdr->size;
        free(hdr);
        current = blopfn_pool_get_front(&pool->ptrs);
    }
}
void*       blopfn_pool_malloc(blopt_pool pool, size_t size) {
    BLOP_ASSERT_PTR(pool, NULL);
    if (size == 0) {
        BLOP_ERROR_MESSAGE("Requested allocation size is 0 (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    blopt_ptrhdr hdr = BLOP_ALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
    if (!hdr) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for pointer header (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    memset(hdr, 0, size + sizeof(struct blops_ptrhdr));
    hdr->size = size;
    hdr->pool = pool;
    hdr->node.data = hdr;
    
    blopfn_pool_list_push_back(&pool->ptrs, &hdr->node)
    
    pool->bytes += size;
    void*  ptr   = BLOP_PADD(hdr, sizeof(struct blops_ptrhdr));
    return ptr;
}
void*       blopfn_pool_calloc(blopt_pool pool, size_t count, size_t size) {
    BLOP_ASSERT_PTR(pool, NULL);
    if (size == 0) {
        BLOP_ERROR_MESSAGE("Requested allocation size is 0 (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    size *= count;
    blopt_ptrhdr hdr = BLOP_ALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
    if (!hdr) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for pointer header (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    memset(hdr, 0, size + sizeof(struct blops_ptrhdr));
    hdr->size = size;
    hdr->pool = pool;
    hdr->node.data = hdr;
    
    blopfn_pool_list_push_back(&pool->ptrs, &hdr->node)
    
    pool->bytes += size;
    void*  ptr   = BLOP_PADD(hdr, sizeof(struct blops_ptrhdr));
    return ptr;
}
void*       blopfn_pool_realloc(blopt_pool pool, void* ptr, size_t size) {
    BLOP_ASSERT_PTR(pool, NULL);
    BLOP_ASSERT_PTR(ptr, NULL);
    if (size == 0) {
        BLOP_ERROR_MESSAGE("Requested allocation size is 0 (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    blopt_ptrhdr srchdr = (blopt_ptrhdr)BLOP_PSUB(ptr, sizeof(struct blops_ptrhdr));
    if (srchdr->pool != pool) {
        BLOP_ERROR_MESSAGE("The pointer does not belong to this pool (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    blopt_ptrhdr newhdr = BLOP_ALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
    if (!newhdr) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for pointer header (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    newhdr->size = size;
    newhdr->pool = pool;
    newhdr->node.data = newhdr;
    
    blopfn_pool_list_insert_next(&pool->ptrs, &srchdr->node, &newhdr->node);
    
    pool->bytes += size;
    void*  newptr = BLOP_PADD(newhdr, sizeof(struct blops_ptrhdr));
    memcpy(newptr, ptr, BLOP_MIN(size, srchdr->size));
    
    if (size > srchdr->size) {
        memset(BLOP_PADD(newptr, srchdr->size), 0, size - srchdr->size);
    }

    blopfn_pool_list_erase(&pool->ptrs, &srchdr->node, false);
    pool->bytes -= srchdr->size;
    free(srchdr);
    
    return newptr;
}
void*       blopfn_pool_duplicate(blopt_pool pool, void* ptr, size_t size) {
    BLOP_ASSERT_PTR(pool, NULL);
    BLOP_ASSERT_PTR(ptr, NULL);
    if (size == 0) {
        BLOP_ERROR_MESSAGE("Requested allocation size is 0 (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    blopt_ptrhdr srchdr = (blopt_ptrhdr)BLOP_PSUB(ptr, sizeof(struct blops_ptrhdr));
    if (srchdr->pool != pool) {
        BLOP_ERROR_MESSAGE("The pointer does not belong to this pool (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    blopt_ptrhdr newhdr = BLOP_ALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
    if (!newhdr) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for pointer header (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    memset(newhdr, 0, size + sizeof(struct blops_ptrhdr));
    newhdr->size = size;
    newhdr->pool = pool;
    newhdr->node.data = newhdr;
    
    blopfn_pool_list_insert_next(&pool->ptrs, &srchdr->node, &newhdr->node);
    
    pool->bytes += size;
    void*  newptr = BLOP_PADD(newhdr, sizeof(struct blops_ptrhdr));
    memcpy(newptr, ptr, BLOP_MIN(size, srchdr->size));
    return newptr;
}

void        blopfn_pool_print(blopt_pool pool) {
    BLOP_ASSERT_PTR_VOID(pool);

    printf("Pool at %p\n", (void*)pool);
    printf("  Total bytes allocated: %zu\n", pool->bytes);
    printf("  Allocated pointers (%zu):\n", pool->ptrs.size);

    blopt_pool_list_node current = blopfn_pool_get_front(&pool->ptrs);
    size_t index = 0;
    while (current != NULL) {
        blopt_ptrhdr hdr = (blopt_ptrhdr)current->data;
        printf("    [%zu] Pointer at %p - Size: %zu bytes\n", index, BLOP_PADD(hdr, sizeof(struct blops_ptrhdr)), hdr->size);
        current = current->next;
        index++;
    }
}

size_t      blopfn_pool_get_bytes(blopt_pool pool) {
    BLOP_ASSERT_PTR(pool, 0);
    return pool->bytes;
}
size_t      blopfn_pool_get_count(blopt_pool pool) {
    BLOP_ASSERT_PTR(pool, 0);
    return pool->ptrs.size;
}

#endif // BLOP_POOL_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // __BLOP_POOL_H__