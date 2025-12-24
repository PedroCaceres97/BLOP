#include <blop/blop.h>

#ifndef BLOP_VECTOR_NAME
  #define BLOP_VECTOR_NAME vectorb
#endif /* BLOP_VECTOR_NAME */

#ifndef BLOP_VECTOR_DATA_TYPE
  #define BLOP_VECTOR_DATA_TYPE uint8_t
#endif /* BLOP_VECTOR_DATA_TYPE */

#define BLOP_TYPE BLOP_VECTOR_DATA_TYPE

#ifndef BLOP_VECTOR_RESIZE_POLICIE
  #define BLOP_VECTOR_RESIZE_POLICIE(size) (size * 2)
#endif /* BLOP_VECTOR_RESIZE_POLICIE */

#ifndef BLOP_VECTOR_SHRINK_POLICIE
  #define BLOP_VECTOR_SHRINK_POLICIE(capacity) (capacity / 4)
#endif

#if !defined(BLOP_VECTOR_INITIAL_SIZE) || BLOP_VECTOR_INITIAL_SIZE <= 0
  #define BLOP_VECTOR_INITIAL_SIZE 10
#endif /* BLOP_VECTOR_INITIAL_SIZE */

/** @cond doxygen_ignore */
#define blopt_vector                BLOP_VECTOR_NAME
#define blops_vector                BLOP_CONCAT3(_, blopt_vector, _t)

#define blopfn_vector_create        BLOP_CONCAT2(blopt_vector, _create)
#define blopfn_vector_destroy       BLOP_CONCAT2(blopt_vector, _destroy)

#define blopfn_vector_rdlock        BLOP_CONCAT2(blopt_vector, _rdlock)
#define blopfn_vector_wrlock        BLOP_CONCAT2(blopt_vector, _wrlock)
#define blopfn_vector_rdunlock      BLOP_CONCAT2(blopt_vector, _rdunlock)
#define blopfn_vector_wrunlock      BLOP_CONCAT2(blopt_vector, _wrunlock)

#define blopfn_vector_data          BLOP_CONCAT2(blopt_vector, data)
#define blopfn_vector_size          BLOP_CONCAT2(blopt_vector, size)
#define blopfn_vector_back          BLOP_CONCAT2(blopt_vector, back)
#define blopfn_vector_front         BLOP_CONCAT2(blopt_vector, front)

#define blopfn_vector_set           BLOP_CONCAT2(blopt_vector, _set)
#define blopfn_vector_get           BLOP_CONCAT2(blopt_vector, _get)
#define blopfn_vector_resize        BLOP_CONCAT2(blopt_vector, _resize)
#define blopfn_vector_shrink        BLOP_CONCAT2(blopt_vector, _shrink)

#define blopfn_vector_clear         BLOP_CONCAT2(blopt_vector, _clear)
#define blopfn_vector_erase         BLOP_CONCAT2(blopt_vector, _erase)
#define blopfn_vector_pop_back      BLOP_CONCAT2(blopt_vector, _pop_back)
#define blopfn_vector_pop_front     BLOP_CONCAT2(blopt_vector, _pop_front)

#define blopfn_vector_insert        BLOP_CONCAT2(blopt_vector, _insert)
#define blopfn_vector_push_back     BLOP_CONCAT2(blopt_vector, _push_back)
#define blopfn_vector_push_front    BLOP_CONCAT2(blopt_vector, _push_front)

#define blopfn_vector_memcpy        BLOP_CONCAT2(blopt_vector, _memcpy)
#define blopfn_vector_memset        BLOP_CONCAT2(blopt_vector, _memset)

/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_vector* blopt_vector;

blopt_vector  blopfn_vector_create    (blopt_vector vec);
void          blopfn_vector_destroy   (blopt_vector vec);

void          blopfn_vector_rdlock    (blopt_vector vec);
void          blopfn_vector_wrlock    (blopt_vector vec);
void          blopfn_vector_rdunlock  (blopt_vector vec);
void          blopfn_vector_wrunlock  (blopt_vector vec);

BLOP_TYPE*    blopfn_vector_data      (blopt_vector vec);
size_t        blopfn_vector_size      (blopt_vector vec);
BLOP_TYPE     blopfn_vector_back      (blopt_vector vec);
BLOP_TYPE     blopfn_vector_front     (blopt_vector vec);

void          blopfn_vector_set       (blopt_vector vec, size_t idx, BLOP_TYPE value);
BLOP_TYPE     blopfn_vector_get       (blopt_vector vec, size_t idx);
void          blopfn_vector_resize    (blopt_vector vec, size_t size);
void          blopfn_vector_shrink    (blopt_vector vec);

void          blopfn_vector_clear     (blopt_vector vec);
void          blopfn_vector_erase     (blopt_vector vec, size_t idx);
void          blopfn_vector_pop_back  (blopt_vector vec);
void          blopfn_vector_pop_front (blopt_vector vec);

void          blopfn_vector_insert    (blopt_vector vec, size_t idx, BLOP_TYPE value);
void          blopfn_vector_push_back (blopt_vector vec, BLOP_TYPE value);
void          blopfn_vector_push_front(blopt_vector vec, BLOP_TYPE value);

void          blopfn_vector_memcpy    (blopt_vector vec, size_t idx, const BLOP_TYPE* src, size_t count);
void          blopfn_vector_memset    (blopt_vector vec, size_t idx, BLOP_TYPE value, size_t count);

#if (defined(BLOP_VECTOR_STRUCT) || defined(BLOP_VECTOR_IMPLEMENTATION)) && !defined(BLOP_VECTOR_NOT_STRUCT)
struct blops_vector {
  BLOP_TYPE*        data;
  size_t            size;
  size_t            capacity;
  int               allocated;
  BLOP_RWLOCK_TYPE  lock;
};
#endif

#ifdef BLOP_VECTOR_IMPLEMENTATION

blopt_vector  blopfn_vector_create(blopt_vector vec) {
  if (!vec) {
    BLOP_CALLOC(vec, struct blops_vector, 1);
    vec->allocated = true;
  } else {
    vec->allocated = false;
  }

  vec->size = 0;
  vec->capacity = BLOP_VECTOR_INITIAL_SIZE;
  BLOP_CALLOC(vec->data, BLOP_TYPE, vec->capacity);
  BLOP_RWLOCK_INIT(vec->lock);

  return vec;
}
void          blopfn_vector_destroy(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);

  BLOP_INTERNAL_ASSERT(vec->size == 0, "Destroying non empty vector (HINT: Clear the vector)");

  BLOP_RWLOCK_DESTROY(vec->lock);
  BLOP_FREE(vec->data);

  if (vec->allocated) {
    BLOP_FREE(vec);
  }
}

void          blopfn_vector_rdlock(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_RWLOCK_RDLOCK(vec->lock);
}
void          blopfn_vector_wrlock(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_RWLOCK_WRLOCK(vec->lock);
}
void          blopfn_vector_rdunlock(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_RWLOCK_RDUNLOCK(vec->lock);
}
void          blopfn_vector_wrunlock(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_RWLOCK_WRUNLOCK(vec->lock);
}

BLOP_TYPE*    blopfn_vector_data      (blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");
  return vec->data;
}
size_t        blopfn_vector_size      (blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");
  return vec->size;
}
BLOP_TYPE     blopfn_vector_back      (blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");
  
  BLOP_INTERNAL_ASSERT_FORCED(vec->size != 0, "Vector size is 0");
  return vec->data[vec->size - 1];
}
BLOP_TYPE     blopfn_vector_front     (blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");
  
  BLOP_INTERNAL_ASSERT_FORCED(vec->size != 0, "Vector size is 0");
  return vec->data[0];
}

void          blopfn_vector_set(blopt_vector vec, size_t idx, BLOP_TYPE value) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, vec->size);

  vec->data[idx] = value;
}
BLOP_TYPE     blopfn_vector_get(blopt_vector vec, size_t idx) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, vec->size);

  return vec->data[idx];
}
void          blopfn_vector_resize(blopt_vector vec, size_t size) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  if (size == vec->size) {
    return;
  }

  if (size == 0) {
    blopfn_vector_clear(vec);
    return;
  }

  if (size < vec->size) {
    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
      for (size_t i = size; i < vec->size; i++) {
        BLOP_VECTOR_DEALLOCATE_DATA(vec->data[i]);
      }
    #endif /* BLOP_VECTOR_DEALLOCATE_DATA */
  }

  vec->capacity = TERNARY(
    size < BLOP_VECTOR_INITIAL_SIZE,
    BLOP_VECTOR_INITIAL_SIZE,
    BLOP_VECTOR_RESIZE_POLICIE(size)
  );

  BLOP_TYPE* data = NULL;
  BLOP_CALLOC(data, BLOP_TYPE, vec->capacity);
  memcpy(data, vec->data, MIN(vec->size, size) * sizeof(BLOP_TYPE));

  BLOP_FREE(vec->data);
  vec->data = data;
  vec->size = size;
}
void          blopfn_vector_shrink(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  if (vec->size < BLOP_VECTOR_SHRINK_POLICIE(vec->capacity) && vec->size < BLOP_VECTOR_INITIAL_SIZE) {
    vec->capacity = BLOP_VECTOR_RESIZE_POLICIE(vec->size);
    BLOP_TYPE* data = NULL;
    BLOP_CALLOC(data, BLOP_TYPE, vec->capacity);

    memcpy(data, vec->data, vec->size * sizeof(BLOP_TYPE));
    BLOP_FREE(vec->data);
    vec->data = data;
  }
}

void          blopfn_vector_clear(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  #ifdef BLOP_VECTOR_DEALLOCATE_DATA
    for (size_t i = 0; i < vec->size; i++) {
      BLOP_VECTOR_DEALLOCATE_DATA(vec->data[i]);
    }
  #endif /* BLOP_VECTOR_DEALLOCATE_DATA */

  vec->size = 0;
  vec->capacity = BLOP_VECTOR_INITIAL_SIZE;
  BLOP_FREE(vec->data);
  BLOP_CALLOC(vec->data, BLOP_TYPE, vec->capacity);
}
void          blopfn_vector_erase(blopt_vector vec, size_t idx) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, vec->size);

  #ifdef BLOP_VECTOR_DEALLOCATE_DATA
    BLOP_VECTOR_DEALLOCATE_DATA(vec->data[idx]);
  #endif /* BLOP_VECTOR_DEALLOCATE_DATA */           

  if (idx != vec->size - 1) {
    memmove(&vec->data[idx], &vec->data[idx + 1], (vec->size - idx - 1) * sizeof(BLOP_TYPE));
  }
  
  vec->size--;

  blopfn_vector_shrink(vec);
}
void          blopfn_vector_pop_back(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  if (vec->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  blopfn_vector_erase(vec, vec->size - 1);
}
void          blopfn_vector_pop_front(blopt_vector vec) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  if (vec->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  #ifdef BLOP_VECTOR_DEALLOCATE_DATA
    BLOP_VECTOR_DEALLOCATE_DATA(vec->data[0]);
  #endif /* BLOP_VECTOR_DEALLOCATE_DATA */

  blopfn_vector_erase(vec, 0);
}

void          blopfn_vector_insert(blopt_vector vec, size_t idx, BLOP_TYPE value) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, vec->size + 1);

  if (vec->size != vec->capacity) {
    if (idx != vec->size) {
      memmove(&vec->data[idx + 1], &vec->data[idx], (vec->size - idx) * sizeof(BLOP_TYPE));
    }
  } else {
    vec->capacity = BLOP_VECTOR_RESIZE_POLICIE(vec->size);
    BLOP_TYPE* data = NULL;
    BLOP_CALLOC(data, BLOP_TYPE, vec->capacity);

    if (idx != 0) {
      memcpy(data, vec->data, idx * sizeof(BLOP_TYPE));
    }
    
    if (idx != vec->size) {
      memcpy(&data[idx + 1], &vec->data[idx], (vec->size - idx) * sizeof(BLOP_TYPE));
    }

    BLOP_FREE(vec->data);
    vec->data = data;
  }

  vec->data[idx] = value;
  vec->size++;
}
void          blopfn_vector_push_back(blopt_vector vec, BLOP_TYPE value) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  blopfn_vector_insert(vec, vec->size, value);
}
void          blopfn_vector_push_front(blopt_vector vec, BLOP_TYPE value) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  blopfn_vector_insert(vec, 0, value);
}

void          blopfn_vector_memcpy(blopt_vector vec, size_t idx, const BLOP_TYPE* src, size_t count) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT_PTR(src);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  if (count == 0) { return; }

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, vec->size);
  BLOP_INTERNAL_ASSERT_BOUNDS(idx + count, vec->size + 1);

  memcpy(&vec->data[idx], src, count * sizeof(BLOP_TYPE));
}
void          blopfn_vector_memset(blopt_vector vec, size_t idx, BLOP_TYPE value, size_t count) {
  BLOP_INTERNAL_ASSERT_PTR(vec);
  BLOP_INTERNAL_ASSERT(vec->capacity >= vec->size, "Vector capacity is smaller than vec->size");
  BLOP_INTERNAL_ASSERT(vec->data != NULL, "Vector data is NULL");

  if (count == 0) { return; }

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, vec->size);
  BLOP_INTERNAL_ASSERT_BOUNDS(idx + count, vec->size + 1);

  for (size_t i = 0; i < count; i++) {
    vec->data[idx + i] = value;
  }
}

#endif /* BLOP_VECTOR_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#ifdef _DOXYGEN_

  #define BLOP_VECTOR_DEALLOCATE_DATA(data)
  #define BLOP_VECTOR_IMPLEMENTATION
    
  #undef  BLOP_VECTOR_DEALLOCATE_DATA
  #undef  BLOP_VECTOR_IMPLEMENTATION

#endif

#undef BLOP_TYPE
#undef BLOP_VECTOR_NAME
#undef BLOP_VECTOR_DATA_TYPE
#undef BLOP_VECTOR_INITIAL_SIZE
#undef BLOP_VECTOR_SHRINK_POLICIE
#undef BLOP_VECTOR_RESIZE_POLICIE
#undef BLOP_VECTOR_DEALLOCATE_DATA

#undef BLOP_VECTOR_STRUCT
#undef BLOP_VECTOR_NOT_STRUCT
#undef BLOP_VECTOR_IMPLEMENTATION

#undef blopt_vector            
#undef blops_vector

#undef blopfn_vector_create    
#undef blopfn_vector_destroy

#undef blopfn_vector_rdlock    
#undef blopfn_vector_wrlock    
#undef blopfn_vector_rdunlock  
#undef blopfn_vector_wrunlock

#undef blopfn_vector_data      
#undef blopfn_vector_size      
#undef blopfn_vector_back      
#undef blopfn_vector_front

#undef blopfn_vector_set       
#undef blopfn_vector_get       
#undef blopfn_vector_resize    
#undef blopfn_vector_shrink

#undef blopfn_vector_clear     
#undef blopfn_vector_erase     
#undef blopfn_vector_pop_back  
#undef blopfn_vector_pop_front

#undef blopfn_vector_insert    
#undef blopfn_vector_push_back 
#undef blopfn_vector_push_front

#undef blopfn_vector_memcpy    
#undef blopfn_vector_memset