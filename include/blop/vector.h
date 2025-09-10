#ifndef __BLOP_VECTOR_H__
#define __BLOP_VECTOR_H__

#include <blop/blop.h>

typedef struct _BlopVector_t* BlopVector;

#ifdef __BLOP_SHOW_VECTOR_IMPLEMENTATION__

struct _BlopVector_t {
    size_t min;         // Minimum size of the vector before shrinking
    size_t size;       // Number of elements in the vector
    size_t capacity;    // Allocated capacity of the vector
    size_t element;     // Size of each element in bytes

    void* data;         // Pointer to the data array
    uint8_t scalator;   // Growth factor for resizing
};

#endif // __BLOP_SHOW_VECTOR_IMPLEMENTATION__

/*
Creates a new BlopVector with the specified initial data, count of elements in initial data, 
size of each element, and the scalator (growth factor for resizing).
If init_data is NULL, the vector is initialized with all zeros.
*/
BlopVector  BlopNewVector       (void* init, size_t initc, size_t element, uint8_t scalator);
int         BlopFreeVector      (BlopVector vector, int keep_data);

/*
When BlopVectorResize is called with a size greater than the current size, new elements are
initialized with the data provided. If data is NULL, the new elements are left uninitialized (handle carefully).
*/
int         BlopVectorResize    (BlopVector vector, size_t size, void* init);
void*       BlopVectorGetData   (BlopVector vector);
size_t      BlopVectorGetSize   (BlopVector vector);

int         BlopVectorSet       (BlopVector vector, size_t idx, void* buffer);
int         BlopVectorGet       (BlopVector vector, size_t idx, void* buffer);
int         BlopVectorPopBack   (BlopVector vector, void* buffer);
int         BlopVectorPushBack  (BlopVector vector, void* buffer);

#endif // __BLOP_VECTOR_H__