#ifndef __BLOP_VECTOR_H__
#define __BLOP_VECTOR_H__

#include <blop/common.h>

typedef struct _BVECTOR_t* BVECTOR;

#ifdef __BLOP_SHOW_VECTOR_IMPLEMENTATION__

struct _BVECTOR_t {
    size_t min;         // Minimum size of the vector before shrinking
    size_t size;       // Number of elements in the vector
    size_t capacity;    // Allocated capacity of the vector
    size_t element;     // Size of each element in bytes

    void* data;         // Pointer to the data array
    uint8_t scalator;   // Growth factor for resizing
};

#endif // __BLOP_SHOW_VECTOR_IMPLEMENTATION__

/*
Creates a new BVECTOR with the specified initial data, count of elements in initial data, 
size of each element, and the scalator (growth factor for resizing).
If init_data is NULL, the vector is initialized with all zeros.
*/
BVECTOR BVECTOR_Create  (void* init, size_t initc, size_t element, uint8_t scalator);
void    BVECTOR_Destroy (BVECTOR vector, int keep_data);

/*
When BVECTOR_Resize is called with a size greater than the current size, new elements are
initialized with the data provided. If data is NULL, the new elements are left uninitialized (handle carefully).
*/
void    BVECTOR_Resize  (BVECTOR vector, size_t size, void* init);
void*   BVECTOR_GetData (BVECTOR vector);
size_t  BVECTOR_GetSize (BVECTOR vector);

void    BVECTOR_Set     (BVECTOR vector, size_t idx, void* buffer);
void    BVECTOR_Get     (BVECTOR vector, size_t idx, void* buffer);
void    BVECTOR_PopBack (BVECTOR vector, void* buffer);
void    BVECTOR_PushBack(BVECTOR vector, void* buffer);

#endif // __BLOP_VECTOR_H__