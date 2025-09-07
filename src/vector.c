#define __BLOP_DEFAULT_CALLBACKS__
#include <blop/blop.h>

#define ptr_idx(ptr, idx, size) (void*)(((uint8_t*)ptr) + (idx * size))

struct _BlopVector_t {
    size_t size;        // Logical elements count
    size_t capacity;    // Allocated elements count
    size_t element;     // Size of each element
    size_t last_resize; // Last logical elements count

    void* data;         // Memory
    uint8_t scalator;   // Factor of grown
};

static void blop_get(BlopVector vector, size_t idx, void* buffer) {
    memcpy(buffer, ptr_idx(vector->data, idx, vector->element), vector->element);
}
static void blop_set(BlopVector vector, size_t idx, void* buffer) {
    memcpy(ptr_idx(vector->data, idx, vector->element), buffer, vector->element);
}
static void blop_realloc(BlopVector vector, size_t size) {
    vector->last_resize = vector->size;
    vector->size = size;
    vector->capacity = vector->size * vector->scalator;
    vector->data = __blop_realloc(vector->data, vector->capacity * vector->element);
}

void BlopFreeVector(BlopVector vector) {
    if (vector == NULL) {
        blop_error("NULL pointer exception");
        return;
    }
    if (vector->data != NULL) {
        __blop_free(vector->data);
    }
    __blop_free(vector);
}
BlopVector BlopNewVector(uint8_t scalator, void* init_data, size_t init_count, size_t element) {
    if (element == 0) {
        blop_error("Element size is 0");
        return NULL;
    }
    if (scalator < 2) scalator = 2;

    BlopVector vector = __blop_alloc(sizeof(struct _BlopVector_t));
    vector->element = element;
    vector->scalator = scalator;
    vector->size = init_count;
    vector->last_resize = 0;
    vector->capacity = 0;
    vector->data = NULL;

    size_t initial_size = (init_count) ? scalator : init_count * scalator;
    vector->data = __blop_alloc(initial_size * element);
    
    if (init_data) {
        memcpy(vector->data, init_data, init_count * element);
    }

    return vector;
}

void BlopVectorResize(BlopVector vector, size_t size, void* data) {
    if (size == vector->size) {
        return;
    }

    if (size > vector->capacity || size < (vector->last_resize / vector->scalator)) {
        size_t old_size = vector->size;
        blop_realloc(vector, size);

        if (old_size < vector->size) {
            for (size_t i = vector->size - old_size; i > 0; i--) {
                blop_set(vector, vector->size - i, data);
            }
        }
    }
}
void* BlopVectorData(BlopVector vector) {
    if (vector == NULL) {
        blop_error("NULL pointer exception");
        return NULL;
    }

    return vector->data;
}
size_t BlopVectorSize(BlopVector vector) { 
    if (vector == NULL) {
        blop_error("NULL pointer exception");
        return -1;
    }

    return vector->size;
}

int BlopVectorSet(BlopVector vector, size_t idx, void* data) { 
    if (idx >= vector->size) {
        blop_error("Bound exceptions, idx is greater or equal to vector->size");
        return false;
    }

    if (data == NULL) {
        blop_error("NULL pointer exception");
        return false;
    }

    blop_set(vector, idx, data);
    return true;
}
int BlopVectorGet(BlopVector vector, size_t idx, void* data) { 
    if (idx >= vector->size) {
        blop_error("Bound exceptions, idx is greater or equal to vector->size");
        return false;
    }

    if (data == NULL) {
        blop_error("NULL pointer exception");
        return false;
    }

    blop_get(vector, idx, data);
    return true;
}

void BlopVectorPopBack(BlopVector vector, void* data) {
    if (vector->size == 0) {
        return;
    }

    if (data) {
        blop_get(vector, vector->size - 1, data);
    }
    vector->size--;

    if (vector->size < (vector->last_resize / vector->scalator)) {
        blop_realloc(vector, vector->size);
    }
}
void BlopVectorPushBack(BlopVector vector, void* data) { 
    if (data == NULL) {
        blop_error("NULL pointer exception");
        return;
    }

    if (vector->size == vector->capacity) {
        blop_realloc(vector, vector->size);
    }

    blop_set(vector, vector->size, data);
    vector->size++;
}