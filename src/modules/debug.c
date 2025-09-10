#define __BLOP_DEFAULT_CALLBACKS__
#define __BLOP_SHOW_MEMORY_IMPLEMENTATION__
#include <blop/memory.h>
#include <blop/utils.h>

static const char* df_alias     = "NO ALIAS";
static const char* df_file      = "THATS NOT NICE";
static const char* df_function  = "FUCK YOU";

BlopResult    __BlopNewDebugPool     (BlopDebugPool* buffer, int mute, const char* alias, const char* file, uint32_t line, const char* function) {
    blop_debug_if(!mute, "Allocating BlopDebugPool...")
    *buffer = blop_alloc(sizeof(struct _BlopPool_t), 1);
    return_verbose_if(*buffer == NULL, BlopAllocationFailed, "Failed to allocate BlopPool")
    *buffer->total      =  0;
    *buffer->list       = {0};
    *buffer->alias      = blop_ternary(alias == NULL, df_alias, strdup(alias));
    *buffer->file       = blop_ternary(file == NULL, df_file, strdup(BlopUtilsPathLast(file)));
    *buffer->line       = line;
    *buffer->function   = blop_ternary(function == NULL, df_function, strdup(function));
    return BlopSuccess;
}
BlopResult    __BlopFreeDebugPool    (BlopDebugPool pool, int mute) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopPool cant be a null ptr")
    return_verbose_if(pool->total > 0, BlopNonEmptyStructure, "A BlopPool must be empty (all allocated ptr were freed) to avoid memory leaks")
    return_verbose_if(pool->list->size > 0, BlopNonEmptyStructure, "The fuck is going on")
    
    blop_debug_if(!mute, "Freeing BlopDebugPool...");
    blop_free_if(pool->alias    != df_alias, pool->alias);
    blop_free_if(pool->file     != df_file, pool->file);
    blop_free_if(pool->function != df_function, pool->function);
    blop_free(pool);
    return BlopSuccess;
}

BlopResult __BlopDebugFree(BlopDebugPool pool, void* ptr, int mute) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    return_verbose_if(ptr == NULL, BlopNullException, "Cant free a null ptr")

    struct _BlopDebugPtrHeader_t* header = blop_ptr_sub(ptr, sizeof(struct _BlopDebugPtrHeader_t));
    return_verbose_if(header->pool != pool, BlopWrongSignature, "Pointer allocated with a different pool")
    return_verbose_if(header->magic != 0xDEADBEEF, BlopWrongSignature, "Pointer not allocated with BlopDebugAlloc")

    blop_debug_if(!mute, "Freeing pointer from BlopDebugPool...");
    pool->total -= header->size;
    BlopListErase(&pool->list, &header->node, false);
    blop_free(header);
    return BlopSuccess;
}

BlopResult __BlopDebugAlloc(BlopDebugPool pool, void** buffer, size_t size, int mute, const char* alias, const char* file, uint32_t line, const char* function) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    return_if(size == 0, BlopNoAllocation)
    blop_debug_if(!mute, "Allocating pointer in BlopDebugPool...");
    
    struct _BlopDebugPtrHeader_t* header = blop_alloc(struct _BlopDebugPtrHeader_t, sizeof(struct _BlopDebugPtrHeader_t) + size);
    *buffer = blop_ptr_add(header, sizeof(struct _BlopDebugPtrHeader_t));
    header->magic       = 0xDEADBEEF;
    header->pool        = pool;
    header->node.heap   = header;
    header->size        = size;
    header->alias       = blop_ternary(alias == NULL, df_alias, strdup(alias));
    header->file        = blop_ternary(file == NULL, df_file, strdup(BlopUtilsPathLast(file)));
    header->line        = line;
    header->function    = blop_ternary(function == NULL, df_function, strdup(function));
    
    if (BlopListPushBack(&pool->list, &header->node) != BlopSuccess) {
        *buffer = NULL;
        blop_free(header);
        return_verbose(BlopAllocationFailed, "Failed to push back header->node");
    }
    pool->total += size;
    return BlopSuccess;
}

BlopResult __BlopDebugRealloc(BlopDebugPool pool, void** buffer, void* ptr, size_t size, int mute, const char* alias, const char* file, uint32_t line, const char* function) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    blop_debug_if(!mute, "Reallocating pointer in BlopDebugPool...");
    if (ptr == NULL) {
        return __BlopDebugAlloc(pool, buffer, size, mute, alias, file, line, function);
    }
    if (size == 0) {
        return __BlopDebugFree(pool, ptr, mute);
    }

    struct _BlopDebugPtrHeader_t* header = blop_ptr_sub(ptr, sizeof(struct _BlopDebugPtrHeader_t));
    return_verbose_if(header->pool != pool, BlopWrongSignature, "Pointer allocated with a different pool")
    return_verbose_if(header->magic != 0xDEADBEEF, BlopWrongSignature, "Pointer not allocated with BlopDebugAlloc")

    size_t old_size         = header->size;
    BlopDebugPool old_pool  = header->pool;
    BlopNode prev           = BlopNodeGetPrev(&header->node);
    BlopListErase(&old_pool->list, &header->node, false);

    header              = blop_realloc(struct _BlopDebugPtrHeader_t, header, sizeof(struct _BlopDebugPtrHeader_t) + size);
    header->magic       = 0xDEADBEEF;
    header->node.heap   = header;
    header->pool        = pool;
    header->size        = size;
    header->alias       = blop_ternary(alias == NULL, df_alias, strdup(alias));
    header->file        = blop_ternary(file == NULL, df_file, strdup(BlopUtilsPathLast(file)));
    header->line        = line;
    header->function    = blop_ternary(function == NULL, df_function, strdup(function));

    if (old_pool == pool) {
        BlopListInsertNext(&pool->list, prev, &header->node);
        pool->total += (size - old_size);
    } else {
        pool->total += size;
        old_pool->total -= old_size;
        BlopListPushBack(&pool->list, &header->node);
    }

    *buffer = blop_ptr_add(header, sizeof(struct _BlopDebugPtrHeader_t));
    return BlopSuccess;
}

BlopResult __BlopDebugDuplicate(BlopDebugPool pool, void** buffer, void* ptr, size_t size, int mute, const char* alias, const char* file, uint32_t line, const char* function) {
    return_verbose_if(ptr == NULL, BlopNullException, "Cant duplicate a null ptr")
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    return_if(size == 0, BlopNoAllocation)
    blop_debug_if(!mute, "Duplicating pointer in BlopDebugPool...");
    
    struct _BlopDebugPtrHeader_t* h1 = blop_ptr_sub(ptr, sizeof(struct _BlopDebugPtrHeader_t));
    return_verbose_if(h1->magic != 0xDEADBEEF, BlopWrongSignature, "Pointer not allocated with BlopDebugAlloc")
    
    struct _BlopDebugPtrHeader_t* h2 = blop_alloc(struct _BlopDebugPtrHeader_t, sizeof(struct _BlopDebugPtrHeader_t) + size);
    memcpy(h2, h1, blop_min(h1->size, size) + sizeof(struct _BlopDebugPtrHeader_t));
    h2->node.heap = h2;
    h2->pool = pool;
    h2->size = size;
    h2->alias = blop_ternary(alias == NULL, df_alias, strdup(alias));
    h2->file = blop_ternary(file == NULL, df_file, strdup(BlopUtilsPathLast(file)));
    h2->line = line;
    h2->function = blop_ternary(function == NULL, df_function, strdup(function));
    
    pool->total += size;
    if (pool == h1->pool) {
        BlopListInsertNext(&pool->list, &h1->node, &h2->node);
    } else {
        BlopListPushBack(&pool->list, &h2->node);
    }
    *buffer = blop_ptr_add(h2, sizeof(struct _BlopDebugPtrHeader_t));
    return BlopSuccess;
}

BlopResult __BlopDebugPoolClean(BlopDebugPool pool, int mute, int verbose) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    return_if(pool->total == 0, BlopSuccess)

    blop_debug_if(!mute, "Cleaning BlopDebugPool...");
    pool->total = 0;
    BlopNode node = BlopListGetFront(&pool->list);
    while (node != NULL) {
        struct _BlopDebugPtrHeader_t* header = node->heap;
        if (verbose) {
            printf("Freeing ptr '0x%X' (size: %zu bytes, alias: %s, file: %s, line: %u, function: %s)\n",
                blop_ptr_add(header, sizeof(struct _BlopDebugPtrHeader_t)), header->size,
                header->alias, header->file, header->line, header->function);
        }
        BlopListPopFront(&pool->list, false);
        blop_free_if(header->alias != df_alias, header->alias);
        blop_free_if(header->file != df_file, header->file);
        blop_free_if(header->function != df_function, header->function);
        blop_free(header);
        node = BlopListGetFront(&pool->list);
    }
    return BlopSuccess;
}

BlopResult __BlopDebugPtrGetSize(BlopDebugPool pool, void* ptr, size_t* buffer) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    return_verbose_if(ptr == NULL, BlopNullException, "Cant get size of a null ptr")

    struct _BlopDebugPtrHeader_t* header = blop_ptr_sub(ptr, sizeof(struct _BlopDebugPtrHeader_t));
    return_verbose_if(header->pool != pool, BlopWrongSignature, "Pointer allocated with a different pool")
    return_verbose_if(header->magic != 0xDEADBEEF, BlopWrongSignature, "Pointer not allocated with BlopDebugAlloc")

    *buffer = header->size;
    return BlopSuccess;
}

BlopResult __BlopDebugPoolGetTotal(BlopDebugPool pool, size_t* buffer) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    *buffer = pool->total;
    return BlopSuccess;
}

BlopResult __BlopDebugPoolGetCount(BlopDebugPool pool, size_t* buffer) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    *buffer = pool->list->size;
    return BlopSuccess;
}

BlopResult __BlopDebugPoolPrint(BlopDebugPool pool) {
    return_verbose_if(pool == NULL, BlopNullException, "BlopDebugPool cant be a null ptr")
    printf("BlopDebugPool (total: %zu bytes, allocations: %zu):\n", pool->total, pool->list->size);
    BlopNode node = BlopListGetFront(&pool->list);
    while (node != NULL) {
        struct _BlopDebugPtrHeader_t* header = node->heap;
        printf("  - Ptr '0x%X' (size: %zu bytes, alias: %s, file: %s, line: %u, function: %s)\n",
            blop_ptr_add(header, sizeof(struct _BlopDebugPtrHeader_t)), header->size,
            header->alias, header->file, header->line, header->function);
        node = BlopNodeGetNext(node);
    }
    return BlopSuccess;
}