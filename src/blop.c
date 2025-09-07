#define __BLOP_DEFAULT_CALLBACKS__
#include <blop/blop.h>
#include <blop/utils.h>

static int   __df_blop_free(void* ptr);
static void* __df_blop_alloc(size_t size);
static void* __df_blop_realloc(void* ptr, size_t size);
static void  __df_blop_error(const char* file, uint32_t line, const char* function, const char* message);

PFN_BlopFreeCallback     __blop_free     = __df_blop_free; 
PFN_BlopAllocCallback    __blop_alloc    = __df_blop_alloc;
PFN_BlopReallocCallback  __blop_realloc  = __df_blop_realloc;
PFN_BlopErrorCallback    __blop_error    = __df_blop_error;

int   __df_blop_free(void* ptr) {
    return_if(ptr == NULL, BlopNullException)

    free(ptr);
    return BlopSuccess;
}
void* __df_blop_alloc(size_t size) {
    return_if(size == 0, NULL)

    return calloc(1, size);
};
void* __df_blop_realloc(void* ptr, size_t size) {
    if (size == 0) {
        __blop_free(ptr);
        return NULL;
    }

    return realloc(ptr, size);
};
void  __df_blop_error(const char* file, uint32_t line, const char* function, const char* message) {
    printf("\n[BLOP ERROR REPORT]\nfile: %s:%u\nfunction: %s\nmessage: %s\n\n", BlopUtilsPathLast(file), line, function, message);
}