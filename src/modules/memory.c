#define __BLOP_DEFAULT_CALLBACKS__
#define __BLOP_SHOW_MEMORY_IMPLEMENTATION__
#include <blop/memory.h>
#include <blop/utils.h>

static const char* df_alias = "NO ALIAS";

BlopPool    BlopNewPool     () {
    BlopPool pool = blop_alloc(struct BlopPool, 1);
    return_verbose_if(pool == NULL, NULL, "Failed to allocate BlopPool")
    pool->total  = 0;
    pool->alias = df_alias;
    pool->list  = BlopNewList();
    return_verbose_if(pool->list == NULL, NULL, "Failed to allocate pool->list")
    return pool;
}
int         BlopFreePool    (BlopPool pool) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    return_verbose_if(pool->total > 0, BlopLogicalException, "A BlopPool must be empty (all allocated ptr were freed) to avoid memory leaks")
    return_verbose_if(pool->list->size > 0, BlopLogicalException, "The fuck is going on")

    return_verbose_if(BlopFreeList(pool->list) != BlopSuccess, BlopLogicalException, "Failed to free pool->list");
    blop_free_if(pool->alias != df_alias, pool);
    blop_free(pool);
    return BlopSuccess;
}

int         BlopFree        (void* ptr) {
    return_verbose_if(ptr == NULL, BlopNullException, "Cant free a null ptr")

    struct _BlopPtrHeader_t* header = ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, BlopLogicalException, "The given pointer was not allocated with BlopAlloc")

    BlopPool pool = header->pool;
    pool->total -= header->size;
    BlopListErase(pool->list, &header->node, false);
    blop_free_if(header->alias != df_alias, header->alias)
    blop_free(header);
    return BlopSuccess;
}
int         BlopEmpty       (BlopPool pool) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    return_if(pool->total == 0, BlopSuccess)

    pool->total = 0;
    size_t size = BlopListGetSize(pool->list);
    while (size != 0) {
        BlopNode back = BlopListGetBack(pool->list);
        struct _BlopPtrHeader_t* header = back->heap;
        BlopListPopBack(pool->list, false);
        blop_free(header);
    }
    return BlopSuccess;
}
void*       BlopAlloc       (BlopPool pool, size_t size) {
    return_verbose_if(pool == NULL, NULL , "BlopPool cant be a null ptr")
    return_if(size == 0, NULL)

    void* raw = blop_alloc(void*, sizeof(struct _BlopPtrHeader_t) + size);
    void* ptr = ptr_add(raw, sizeof(struct _BlopPtrHeader_t));
    struct _BlopPtrHeader_t* header = raw;
    header->magic       = 0xDEADBEEF;
    header->pool        = pool;
    header->node.heap   = header;
    header->size        = size;
    header->alias       = df_alias;
    if (BlopListPushBack(pool->list, &header->node) != BlopSuccess) {
        blop_free(raw);
        return_verbose(NULL, "Failed to push back header->node");
    }
    pool->total += size;
    return ptr;
}
void*       BlopRealloc     (BlopPool pool, void* ptr, size_t size) {
    return_verbose_if(pool == NULL, NULL, "BlopPool cant be a null ptr")

    if (ptr == NULL) {
        return BlopAlloc(pool, size);
    }
    if (size == 0) {
        BlopFree(ptr);
        return NULL;
    }

    struct _BlopPtrHeader_t* header = ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, NULL, "The given pointer was not allocated with BlopAlloc")
    
    const char* alias = header->alias;
    size_t      hsize = header->size;
    BlopPool    hpool = header->pool;
    BlopNode    prev  = BlopNodeGetPrev(&header->node);
    BlopListErase(hpool->list, &header->node, false);

    header = blop_realloc(struct _BlopPtrHeader_t, header, sizeof(struct _BlopPtrHeader_t) + size);
    header->magic = 0xDEADBEEF;
    header->node.heap = header;
    header->pool = pool;
    header->size = size;
    header->alias = alias;

    if (hpool == pool) {
        BlopListInsertNext(hpool->list, prev, &header->node);
        add_difference(pool->total, size, hsize)
    } else {
        pool->total += size;
        hpool->total -= hsize;
        BlopListPushBack(pool->list, &header->node);
    }

    return ptr_add(header, sizeof(struct _BlopPtrHeader_t));
}
void*       BlopDuplicate   (BlopPool pool, void* ptr, size_t size) {
    return_verbose_if(ptr == NULL, NULL, "Cant duplicate a null ptr")
    return_verbose_if(pool == NULL, NULL, "BlopPool cant be a null ptr")
    return_if(size == 0, NULL)

    struct _BlopPtrHeader_t* h1 = ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(h1->magic != 0xDEADBEEF, NULL, "The given pointer was not allocated with BlopAlloc")

    struct _BlopPtrHeader_t* h2 = blop_alloc(struct _BlopPtrHeader_t, sizeof(struct _BlopPtrHeader_t) + size);
    memcpy(h2, h1, min(h1->size, size) + sizeof(struct _BlopPtrHeader_t));
    h2->node.heap = h2;
    h2->pool = pool;
    h2->size = size;
    h2->alias = ternary(h1->alias == df_alias, df_alias, strdup(h1->alias));
    pool->total += size;

    if (pool == h1->pool) {
        BlopListInsertNext(pool->list, &h1->node, &h2->node);
    } else {
        BlopListPushBack(pool->list, &h2->node);
    }
    return ptr_add(h2, sizeof(struct _BlopPtrHeader_t));
}

int         BlopPoolPrint   (BlopPool pool) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    printf("BlopPool '%s' (total: %zu bytes, allocations: %zu):\n", pool->alias, pool->total, pool->list->size);
    BlopNode node = BlopListGetFront(pool->list);
    while (node != NULL) {
        struct _BlopPtrHeader_t* header = node->heap;
        printf("  - Ptr '%s' (size: %zu bytes)\n", header->alias, header->size);
        node = BlopNodeGetNext(node);
    }
    return BlopSuccess;
}

size_t      BlopGetPtrSize  (void* ptr) {
    struct _BlopPtrHeader_t* header = ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, 0, "The given pointer was not allocated with BlopAlloc")
    return header->size;
}
const char* BlopGetPtrAlias (void* ptr) {
    struct _BlopPtrHeader_t* header = ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, NULL, "The given pointer was not allocated with BlopAlloc")
    return header->alias;
}
int         BlopSetPtrAlias (void* ptr, const char* alias) {
    struct _BlopPtrHeader_t* header = ptr_sub(ptr, sizeof(struct _BlopPtrHeader_t));
    return_verbose_if(header->magic != 0xDEADBEEF, BlopLogicalException, "The given pointer was not allocated with BlopAlloc")
    blop_free_if(header->alias != df_alias, header->alias)
    header->alias = ternary(alias == NULL, df_alias, strdup(alias));
    return BlopSuccess;
}

size_t      BlopGetPoolSize (BlopPool pool) {
    return_verbose_if(pool == NULL, -1, "BlopPool cant be a null ptr")
    return pool->total;
}
const char* BlopGetPoolAlias(BlopPool pool) {
    return_verbose_if(pool == NULL, NULL, "BlopPool cant be a null ptr")
    return pool->alias;
}
int         BlopSetPoolAlias(BlopPool pool, const char* alias) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    blop_free_if(pool->alias != df_alias, pool->alias)
    pool->alias = ternary(alias == NULL, df_alias, strdup(alias));
    return BlopSuccess;
}