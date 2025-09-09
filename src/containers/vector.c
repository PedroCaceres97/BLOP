#define __BLOP_DEFAULT_CALLBACKS__
#define __BLOP_SHOW_VECTOR_IMPLEMENTATION__
#include <blop/blop.h>
#include <blop/utils.h>
#include <blop/vector.h>

static void vector_get          (BlopVector vector, size_t idx, void* buffer) {
    memcpy(buffer, ptr_add(vector->data, idx * vector->element), vector->element);
}
static void vector_set          (BlopVector vector, size_t idx, void* buffer) {
    memcpy(ptr_add(vector->data, idx * vector->element), buffer, vector->element);
}
static void vector_realloc      (BlopVector vector, size_t size) {
    if (vector->size == size) return;

    vector->min         = ternary(vector->size > size, (vector->size * 2) / 3, size / 2);
    vector->size        = size;
    vector->capacity    = vector->size * vector->scalator;
    vector->data        = blop_realloc(void, vector->data, vector->capacity * vector->element);
}

BlopVector  BlopNewVector       (void* init, size_t initc, size_t element, uint8_t scalator) {
    return_verbose_if(element == 0, NULL, "A BlopVector cannot have elements of size 0. Remember that the element parameter specifies the size in bytes of each element.");

    scalator = ternary(scalator < 2, 2, scalator);

    BlopVector vector   = blop_calloc(_BlopVector_t, 1);
    vector->element     = element;
    vector->scalator    = scalator;
    vector->size        = initc;
    vector->capacity    = ternary(initc > 0, initc * scalator, 10);
    vector->data        = blop_alloc(void, vector->capacity * element);
    
    if (init != NULL && initc > 0) {
        memcpy(vector->data, init, initc * element);
    }

    return vector;
}
int         BlopFreeVector      (BlopVector vector, int keep_data) {
    return_verbose_if(vector == NULL, BlopNullException, "BlopVector cant be a null ptr");

    blop_free_if(vector->data != NULL && !keep_data, vector->data);
    blop_free(vector);
}

int         BlopVectorResize    (BlopVector vector, size_t size, void* init) {
    return_verbose_if(vector == NULL, BlopNullException, "BlopVector cant be a null ptr");
    return_if(size == vector->size, BlopSuccess);

    if (size > vector->capacity || size < vector->min) {
        size_t old = vector->size;
        vector_realloc(vector, size);

        if (old < vector->size && init != NULL) {
            for (size_t i = vector->size - old; i > 0; i--) {
                vector_set(vector, vector->size - i, init);
            }
        }
    }

    return BlopSuccess;
}
void*       BlopVectorData      (BlopVector vector) {
    return_verbose_if(vector == NULL, NULL, "BlopVector cant be a null ptr");

    return vector->data;
}   
size_t      BlopVectorSize      (BlopVector vector) {
    return_verbose_if(vector == NULL, BlopNullException, "BlopVector cant be a null ptr");

    return vector->size;
}

int         BlopVectorSet       (BlopVector vector, size_t idx, void* buffer) {
    return_verbose_if(vector == NULL, BlopNullException, "BlopVector cant be a null ptr");
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr, what value do you expect to set?");
    return_verbose_if(vector->size == 0, BlopIndexException, "BlopVector is empty");
    return_verbose_if(idx >= vector->size, BlopIndexException, "Bound exceptions, idx is greater or equal to vector->size");

    vector_set(vector, idx, buffer);
    return true;
}
int         BlopVectorGet       (BlopVector vector, size_t idx, void* buffer) { 
    return_verbose_if(vector == NULL, BlopNullException, "BlopVector cant be a null ptr");
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr, where do you expect to recieve the data?");
    return_verbose_if(vector->size == 0, BlopIndexException, "BlopVector is empty");
    return_verbose_if(idx >= vector->size, BlopIndexException, "Bound exceptions, idx is greater or equal to vector->size");

    vector_get(vector, idx, buffer);
    return true;
}

int         BlopVectorPopBack   (BlopVector vector, void* buffer) {
    return_verbose_if(vector == NULL, BlopNullException, "BlopVector cant be a null ptr");
    return_verbose_if(vector->size == 0, BlopIndexException, "BlopVector is empty");

    vector->size--;
    if (buffer != NULL) {
        vector_get(vector, vector->size, buffer);
    }

    if (vector->size < vector->min) {
        vector_realloc(vector, vector->size);
    }

    return BlopSuccess;
}
int         BlopVectorPushBack  (BlopVector vector, void* buffer) { 
    return_verbose_if(vector == NULL, BlopNullException, "BlopVector cant be a null ptr");
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr, what value do you expect to push?");

    if (vector->size == vector->capacity) {
        vector_realloc(vector, vector->size);
    }

    vector_set(vector, vector->size, buffer);
    vector->size++;
    return BlopSuccess;
}