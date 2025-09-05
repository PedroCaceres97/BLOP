#ifndef __BLOP_VECTOR_H__
#define __BLOP_VECTOR_H__

#include <blop/blop.h>

typedef struct _BlopVector_t* BlopVector;

void BlopFreeVector(BlopVector vector);
BlopVector BlopNewVector(uint8_t scalator, void* init_data, size_t init_count, size_t element);

// if the new size of the vector is bigger than the current size, the new elements will contain the value pointed by data, unless data is a null pointer
void BlopVectorResize(BlopVector vector, size_t size, void* data);
void* BlopVectorData(BlopVector vector);
size_t BlopVectorSize(BlopVector vector);

// Returns zero on fail and non-zero on success
int BlopVectorSet(BlopVector vector, size_t idx, void* data);

// Returns zero on fail and non-zero on success
int BlopVectorGet(BlopVector vector, size_t idx, void* data);

// pop back will save the value popped in data, unless data is a null pointer
void BlopVectorPopBack(BlopVector vector, void* data);
void BlopVectorPushBack(BlopVector vector, void* data);

#endif // __BLOP_VECTOR_H__