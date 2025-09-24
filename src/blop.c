#define __BLOP_SHOW_APPLICATION_IMPLEMENTATION__
#define __BLOP_DEFAULT_CALLBACKS__
#include <blop/blop.h>
#include <blop/utils.h>

static const char* error_strings[] = {
    "Success"                   ,      
    "Null Pointer Exception"    , 
    "Allocation Failed"         , 
    "Deallocation Failed"       , 
    "Index Exception"           , 
    "Wrong Signature"           ,
    "Non Empty Structure"       ,
};

BlopApplication BlopCreateApplication(int exit_on_error) {
    BlopApplication app = bm_calloc(struct _BlopApplication_t, 1);
    app->exit_on_error = exit_on_error;
    app->error = BlopSuccess;
    app->pools.size = 0;
    app->pools.front = NULL;
    app->pools.back  = NULL;
    app->debug_pools.size = 0;
    app->debug_pools.front = NULL;
    app->debug_pools.back  = NULL;
    return app;
}
int             BlopDestroyApplication(BlopApplication app);
BlopError       BlopGetLastError(BlopApplication app) {
    return app->error;
}
const char*     BlopGetErrorString(BlopError error) {
    if (error < BlopSuccess || error > BlopNonEmptyStructure) {
        return "Unknown Error";
    }

    return error_strings[error];
}

BlopPool        BlopCreatePool(BlopApplication app) {
    if (app == NULL) {
        return NULL;
    }

    BlopPool pool = bm_calloc(struct _BlopPool_t, 1);
    pool->total = 0;
    pool->list.size = 0;
    pool->list.front = NULL;
    pool->list.back  = NULL;
    pool->app = app;

    if (pool == NULL) {
        app->error = BlopAllocationFailed;
        if (app->exit_on_error) {
            fprintf(stderr, "BLOP: Unable to allocate memory for Pool\n");
            exit(EXIT_FAILURE);
        }
        return NULL;
    }

    if (BlopListPushBack(&app->pools, pool) != BlopSuccess) {
        bm_free(pool);
        app->error = BlopAllocationFailed;
        if (app->exit_on_error) {
            fprintf(stderr, "BLOP: Unable to allocate memory for Pool\n");
            exit(EXIT_FAILURE);
        }
        return NULL;
    }

    return pool;
}
void            BlopDestroyPool(BlopPool pool) {

}
size_t          BlopPoolCount(BlopPool pool) {
    return pool->list.size;
}
size_t          BlopPoolTotal(BlopPool pool) {
    return pool->total;
}
void            BlopPoolPrint(BlopPool pool) {
    
}
void            BlopPoolClean(BlopPool pool) {

}

void*           BlopAlloc(BlopPool pool, size_t size);
void*           BlopRealloc(BlopPool pool, void* ptr, size_t size);
void*           BlopDuplicate(BlopPool pool, void* ptr, size_t size);
void            BlopFree(void* ptr);

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