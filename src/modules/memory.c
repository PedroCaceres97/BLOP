#define __BLOP_DEFAULT_CALLBACKS__
#define __BLOP_SHOW_MEMORY_IMPLEMENTATION__
#include <blop/memory.h>
#include <blop/utils.h>

BlopResult    BlopNewPool     (BlopPool* buffer) {
    *buffer = blop_alloc(struct _BlopPool_t, sizeof(struct _BlopPool_t));
    return_verbose_if(*buffer == NULL, BlopAllocationFailed, "Failed to allocate BlopPool")
    BlopPool pool = *buffer;
    pool->total =  0;
    pool->list.size = 0;
    pool->list.front = NULL;
    pool->list.back  = NULL;
    return BlopSuccess;
}
BlopResult    BlopFreePool    (BlopPool pool) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    return_verbose_if(pool->total > 0, BlopNonEmptyStructure, "A BlopPool must be empty (all allocated ptr were freed) to avoid memory leaks")
    return_verbose_if(pool->list.size > 0, BlopNonEmptyStructure, "The fuck is going on")

    blop_free(pool);
    return BlopSuccess;
}

BlopResult    BlopFree        (void* ptr) {
    return_verbose_if(ptr == NULL, BlopNullException, "Cant free a null ptr")

    struct _BlopPtrHeader_t* header = blop_ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, BlopWrongSignature, "The given pointer was not allocated with BlopAlloc")

    BlopPool pool = header->pool;
    pool->total  -= header->size;
    BlopListErase(&pool->list, &header->node, false);
    blop_free(header);
    return BlopSuccess;
}
BlopResult    BlopAlloc       (BlopPool pool, void** buffer, size_t size) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    return_if(size == 0, BlopNoAllocation)

    struct _BlopPtrHeader_t* header = blop_alloc(struct _BlopPtrHeader_t, sizeof(struct _BlopPtrHeader_t) + size);
    *buffer                         = blop_ptr_add(header, sizeof(struct _BlopPtrHeader_t));
    header->magic                   = 0xDEADBEEF;
    header->pool                    = pool;
    header->node.heap               = header;
    header->size                    = size;
    if (BlopListPushBack(&pool->list, &header->node) != BlopSuccess) {
        *buffer = NULL;
        blop_free(header);
        return_verbose(BlopAllocationFailed, "Failed to push back header->node");
    }
    pool->total += size;
    return BlopSuccess;
}
BlopResult    BlopRealloc     (BlopPool pool, void** buffer, void* ptr, size_t size) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")

    if (ptr == NULL) {
        BlopAlloc(pool, buffer, size);
        return BlopSuccess;
    }
    if (size == 0) {
        BlopFree(ptr);
        return BlopSuccess;
    }

    struct _BlopPtrHeader_t* header = blop_ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, BlopWrongSignature, "The given pointer was not allocated with BlopAlloc")
    
    size_t      hsize = header->size;
    BlopPool    hpool = header->pool;
    BlopNode    prev  = BlopNodeGetPrev(&header->node);
    BlopListErase(&hpool->list, &header->node, false);

    header = blop_realloc(struct _BlopPtrHeader_t, header, sizeof(struct _BlopPtrHeader_t) + size);
    header->magic = 0xDEADBEEF;
    header->node.heap = header;
    header->pool = pool;
    header->size = size;

    if (hpool == pool) {
        BlopListInsertNext(&hpool->list, prev, &header->node);
        blop_add_difference(pool->total, size, hsize)
    } else {
        pool->total  += size;
        hpool->total -= hsize;
        BlopListPushBack(&pool->list, &header->node);
    }

    *buffer = blop_ptr_add(header, sizeof(struct _BlopPtrHeader_t));
    return BlopSuccess;
}
BlopResult    BlopDuplicate   (BlopPool pool, void** buffer, void* ptr, size_t size) {
    return_verbose_if(ptr == NULL, BlopNullException, "Cant duplicate a null ptr")
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    return_if(size == 0, BlopNoAllocation)

    struct _BlopPtrHeader_t* h1 = blop_ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(h1->magic != 0xDEADBEEF, BlopWrongSignature, "The given pointer was not allocated with BlopAlloc")

    struct _BlopPtrHeader_t* h2 = blop_alloc(struct _BlopPtrHeader_t, sizeof(struct _BlopPtrHeader_t) + size);
    memcpy(h2, h1, blop_min(h1->size, size) + sizeof(struct _BlopPtrHeader_t));
    h2->node.heap = h2;
    h2->pool = pool;
    h2->size = size;
    pool->total += size;

    if (pool == h1->pool) {
        BlopListInsertNext(&pool->list, &h1->node, &h2->node);
    } else {
        BlopListPushBack(&pool->list, &h2->node);
    }
    *buffer = blop_ptr_add(h2, sizeof(struct _BlopPtrHeader_t));
    return BlopSuccess;
}
BlopResult    BlopPoolClean   (BlopPool pool) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    return_if(pool->total == 0, BlopSuccess)

    pool->total = 0;
    BlopNode node = BlopListGetFront(&pool->list);
    while (node != NULL) {
        struct _BlopPtrHeader_t* header = node->heap;
        BlopListPopFront(&pool->list, false);
        blop_free(header);
        node = BlopListGetFront(&pool->list);
    }
    return BlopSuccess;
}

BlopResult    BlopPtrGetSize  (void* ptr, size_t* buffer) {
    return_verbose_if(ptr == NULL, BlopNullException, "Cant get size of a null ptr")
    struct _BlopPtrHeader_t* header = blop_ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, BlopWrongSignature, "The given pointer was not allocated with BlopAlloc")
    *buffer = header->size;
    return BlopSuccess;
}
BlopResult    BlopPoolGetTotal(BlopPool pool, size_t* buffer) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    *buffer = pool->total;
    return BlopSuccess;
}
BlopResult    BlopPoolGetCount(BlopPool pool, size_t* buffer) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    *buffer = pool->list->size;
    return BlopSuccess;
}

BlopResult    BlopPoolPrint   (BlopPool pool) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    printf("BlopPool (total: %zu bytes, allocations: %zu):\n", pool->total, pool->list->size);
    BlopNode node = BlopListGetFront(&pool->list);
    while (node != NULL) {
        struct _BlopPtrHeader_t* header = node->heap;
        printf("  - Ptr '0x%X' (size: %zu bytes)\n", blop_ptr_add(header, sizeof(struct _BlopPtrHeader_t)), header->size);
        node = BlopNodeGetNext(node);
    }
    return BlopSuccess;
}