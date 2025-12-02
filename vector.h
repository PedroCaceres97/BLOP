#include <blop/blop.h>

#ifndef BLOP_VECTOR_NAME
    #define BLOP_VECTOR_NAME vectorb
#endif /* BLOP_VECTOR_NAME */

#ifndef BLOP_VECTOR_DATA_TYPE
    #define BLOP_VECTOR_DATA_TYPE uint8_t
#endif /* BLOP_VECTOR_DATA_TYPE */

#ifndef BLOP_VECTOR_RESIZE_POLICIE
    #define BLOP_VECTOR_RESIZE_POLICIE(count) (count * 2)
#endif /* BLOP_VECTOR_RESIZE_POLICIE */

#ifndef BLOP_VECTOR_SHRINK_POLICIE
    #define BLOP_VECTOR_SHRINK_POLICIE(capacity) (capacity / 4)
#endif

#if !defined(BLOP_VECTOR_INITIAL_COUNT) || BLOP_VECTOR_INITIAL_COUNT <= 0
    #define BLOP_VECTOR_INITIAL_COUNT 10
#endif /* BLOP_VECTOR_INITIAL_COUNT */

/** @cond doxygen_ignore */
#define blopt_vector                BLOP_VECTOR_NAME
#define blops_vector                BLOP_CONCAT3(_, blopt_vector, _t)

#define blopfn_vector_init          BLOP_CONCAT2(blopt_vector, _init)
#define blopfn_vector_deinit          BLOP_CONCAT2(blopt_vector, _deinit)
#define blopfn_vector_create        BLOP_CONCAT2(blopt_vector, _create)
#define blopfn_vector_destroy       BLOP_CONCAT2(blopt_vector, _destroy)

#define blopfn_vector_rdlock        BLOP_CONCAT2(blopt_vector, _rdlock)
#define blopfn_vector_wrlock        BLOP_CONCAT2(blopt_vector, _wrlock)
#define blopfn_vector_rdunlock      BLOP_CONCAT2(blopt_vector, _rdunlock)
#define blopfn_vector_wrunlock      BLOP_CONCAT2(blopt_vector, _wrunlock)

#define blopfn_vector_get_data      BLOP_CONCAT2(blopt_vector, _get_data)
#define blopfn_vector_get_size      BLOP_CONCAT2(blopt_vector, _get_size)
#define blopfn_vector_get_back      BLOP_CONCAT2(blopt_vector, _get_back)
#define blopfn_vector_get_front     BLOP_CONCAT2(blopt_vector, _get_front)

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

void                    blopfn_vector_init      (blopt_vector vec);
void                    blopfn_vector_deinit    (blopt_vector vec);
blopt_vector            blopfn_vector_create    ();
void                    blopfn_vector_destroy   (blopt_vector vec);

void                    blopfn_vector_rdlock    (blopt_vector vec);
void                    blopfn_vector_wrlock    (blopt_vector vec);
void                    blopfn_vector_rdunlock  (blopt_vector vec);
void                    blopfn_vector_wrunlock  (blopt_vector vec);

BLOP_VECTOR_DATA_TYPE*  blopfn_vector_get_data  (blopt_vector vec);
size_t                  blopfn_vector_get_size (blopt_vector vec);
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_back  (blopt_vector vec);
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_front (blopt_vector vec);

void                    blopfn_vector_set       (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get       (blopt_vector vec, size_t idx);
void                    blopfn_vector_resize    (blopt_vector vec, size_t size);
void                    blopfn_vector_shrink    (blopt_vector vec);

void                    blopfn_vector_clear     (blopt_vector vec);
void                    blopfn_vector_erase     (blopt_vector vec, size_t idx);
void                    blopfn_vector_pop_back  (blopt_vector vec);
void                    blopfn_vector_pop_front (blopt_vector vec);

void                    blopfn_vector_insert    (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);
void                    blopfn_vector_push_back (blopt_vector vec, BLOP_VECTOR_DATA_TYPE value);
void                    blopfn_vector_push_front(blopt_vector vec, BLOP_VECTOR_DATA_TYPE value);

void                    blopfn_vector_memcpy    (blopt_vector vec, size_t idx, const BLOP_VECTOR_DATA_TYPE* src, size_t count);
void                    blopfn_vector_memset    (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count);

#ifdef BLOP_VECTOR_IMPLEMENTATION
struct blops_vector {
    BLOP_VECTOR_DATA_TYPE*  data;   
    size_t                  count;  
    size_t                  capacity;
    BLOP_RWLOCK_TYPE        lock;
};

void                    blopfn_vector_init(blopt_vector vec) {
  vec->data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, BLOP_VECTOR_INITIAL_COUNT);
  BLOP_ASSERT_FORCED_VOID(vec->data != NULL, "Failed to allocate memory for vector data");
  memset(vec->data, 0, BLOP_VECTOR_INITIAL_COUNT * sizeof(BLOP_VECTOR_DATA_TYPE));

  vec->count = 0;
  vec->capacity = BLOP_VECTOR_INITIAL_COUNT;
  BLOP_RWLOCK_INIT(vec->lock);
}
void                    blopfn_vector_deinit(blopt_vector vec){
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_ASSERT_VOID(vec->count == 0, "Destroying non empty vector (HINT: Clear the vector)");

  BLOP_RWLOCK_DESTROY(vec->lock);
  BLOP_FREE(vec->data);
}
blopt_vector            blopfn_vector_create() {
  blopt_vector vec = BLOP_MALLOC(struct blops_vector, 1);
  BLOP_ASSERT_FORCED(vec != NULL, NULL, "Failed to allocate memory for vector structure");

  blopfn_vector_init(vec);
  return vec;
}
void                    blopfn_vector_destroy(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_ASSERT_VOID(vec->count == 0, "Destroying non empty vector (HINT: Clear the vector)");

  BLOP_RWLOCK_DESTROY(vec->lock);
  BLOP_FREE(vec->data);
  BLOP_FREE(vec);
}

void                    blopfn_vector_rdlock(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_RWLOCK_RDLOCK(vec->lock);
}
void                    blopfn_vector_wrlock(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_RWLOCK_WRLOCK(vec->lock);
}
void                    blopfn_vector_rdunlock(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_RWLOCK_RDUNLOCK(vec->lock);
}
void                    blopfn_vector_wrunlock(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_RWLOCK_WRUNLOCK(vec->lock);
}

BLOP_VECTOR_DATA_TYPE*  blopfn_vector_get_data(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, NULL);
    return vec->data;
}
size_t                  blopfn_vector_get_size(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, 0);
    return vec->count;
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_back(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    BLOP_ASSERT(vec->count > 0, (BLOP_VECTOR_DATA_TYPE)0, "The vector is empty");

    return vec->data[vec->count - 1];
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_front(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    BLOP_ASSERT(vec->count > 0, (BLOP_VECTOR_DATA_TYPE)0, "The vector is empty");

    return vec->data[0];
}

void                    blopfn_vector_set(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    BLOP_ASSERT_BOUNDS_VOID(idx, vec->count);

    vec->data[idx] = value;
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get(blopt_vector vec, size_t idx) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    BLOP_ASSERT_BOUNDS(idx, vec->count, (BLOP_VECTOR_DATA_TYPE)0);

    return vec->data[idx];
}
void                    blopfn_vector_resize(blopt_vector vec, size_t count) {
  BLOP_ASSERT_PTR_VOID(vec);

  if (count == vec->count) {
    return;
  }

  if (count < vec->count) {
    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
      for (size_t i = count; i < vec->count; i++) {
        BLOP_VECTOR_DEALLOCATE_DATA(vec->data[i]);
      }
    #endif /* BLOP_VECTOR_DEALLOCATE_DATA */
    vec->count = count;
    blopfn_vector_shrink(vec);
    return;
  }

  if (count > vec->capacity) {
    vec->capacity = BLOP_VECTOR_RESIZE_POLICIE(vec->count);
    BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, sizeof(BLOP_VECTOR_DATA_TYPE) * vec->capacity);
    BLOP_ASSERT_FORCED_VOID(data != NULL, "Failed to allocate memory for vector data");

    memcpy(data, vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
    BLOP_FREE(vec->data);
    vec->data = data;
  }

  memset(&vec->data[vec->count], 0, (count - vec->count) * sizeof(BLOP_VECTOR_DATA_TYPE));
  vec->count = count;
}
void                    blopfn_vector_shrink(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  if (vec->count < BLOP_VECTOR_SHRINK_POLICIE(vec->capacity) && vec->count > BLOP_VECTOR_INITIAL_COUNT) {
    vec->capacity = BLOP_VECTOR_RESIZE_POLICIE(vec->count);
    BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
    BLOP_ASSERT_FORCED_VOID(data != NULL, "Failed to allocate memory for vector data");

    memcpy(data, vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
    BLOP_FREE(vec->data);
    vec->data = data;
  }
}

void                    blopfn_vector_clear(blopt_vector vec) {
    BLOP_ASSERT_PTR_VOID(vec);

    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
        for (size_t i = 0; i < vec->count; i++) {
            BLOP_VECTOR_DEALLOCATE_DATA(vec->data[i]);
        }
    #endif /* BLOP_VECTOR_DEALLOCATE_DATA */

    BLOP_FREE(vec->data);
    vec->data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
    BLOP_ASSERT_FORCED_VOID(vec->data != NULL, "Failed to allocate memory for vector data");
    vec->count = 0;
    vec->capacity = BLOP_VECTOR_INITIAL_COUNT;
}
void                    blopfn_vector_erase(blopt_vector vec, size_t idx) {
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_ASSERT_BOUNDS_VOID(idx, vec->count);

  if (idx == 0) {
    blopfn_vector_pop_front(vec);
    return;
  }

  if (idx == vec->count - 1) {
    blopfn_vector_pop_back(vec);
    return;
  }

  #ifdef BLOP_VECTOR_DEALLOCATE_DATA
    BLOP_VECTOR_DEALLOCATE_DATA(vec->data[idx]);
  #endif /* BLOP_VECTOR_DEALLOCATE_DATA */

  memmove(&vec->data[idx], &vec->data[idx + 1], ((vec->count - idx) - 1) * sizeof(BLOP_VECTOR_DATA_TYPE));
  vec->count--;

  blopfn_vector_shrink(vec);
}
void                    blopfn_vector_pop_back(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  if (vec->count == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  #ifdef BLOP_VECTOR_DEALLOCATE_DATA
    BLOP_VECTOR_DEALLOCATE_DATA(vec->data[vec->count - 1]);
  #endif /* BLOP_VECTOR_DEALLOCATE_DATA */

  vec->count--;

  blopfn_vector_shrink(vec);
}
void                    blopfn_vector_pop_front(blopt_vector vec) {
  BLOP_ASSERT_PTR_VOID(vec);

  if (vec->count == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  #ifdef BLOP_VECTOR_DEALLOCATE_DATA
    BLOP_VECTOR_DEALLOCATE_DATA(vec->data[0]);
  #endif /* BLOP_VECTOR_DEALLOCATE_DATA */

  if (vec->count > 1) {
    memmove(&vec->data[0], &vec->data[1], (vec->count - 1) * sizeof(BLOP_VECTOR_DATA_TYPE));
  }
  vec->count--;

  blopfn_vector_shrink(vec);
}

void                    blopfn_vector_insert(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
  BLOP_ASSERT_PTR_VOID(vec);

  BLOP_ASSERT_BOUNDS_VOID(idx, vec->count + 1);

  if (idx == 0) {
    blopfn_vector_push_front(vec, value);
    return;
  }

  if (idx == vec->count) {
    blopfn_vector_push_back(vec, value);
    return;
  }

  if (vec->count == vec->capacity) {
    size_t capacity = BLOP_VECTOR_RESIZE_POLICIE(vec->count);
    BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, capacity);
    BLOP_ASSERT_FORCED_VOID(data != NULL, "Failed to allocate memory for vector data");

    /*
      The new element is inserted at index idx, so we need to copy:
      - from 0 to idx-1, as idx element is the idx + 1 number of element
          we only need to copy idx elements (0 to idx-1 contains exactly idx number of elements)
      - then we place the new element at index idx
      - we copy from idx to count-1, which is (count - idx) elements
      - finally we fill with zeros the remaining capacity
    */
    memcpy(data, vec->data, idx * sizeof(BLOP_VECTOR_DATA_TYPE));
    data[idx] = value;
    memcpy(&data[idx + 1], &vec->data[idx], (vec->count - idx) * sizeof(BLOP_VECTOR_DATA_TYPE));
    memset(&data[vec->count + 1], 0, (capacity - (vec->count + 1)) * sizeof(BLOP_VECTOR_DATA_TYPE));

    BLOP_FREE(vec->data);
    vec->data = data;
    vec->capacity = capacity;
    vec->count++;
    return;
  }

  memmove(&vec->data[idx + 1], &vec->data[idx], (vec->count - idx) * sizeof(BLOP_VECTOR_DATA_TYPE));
  vec->data[idx] = value;
  vec->count++;
}
void                    blopfn_vector_push_back(blopt_vector vec, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (vec->count == vec->capacity) {
        vec->capacity = BLOP_VECTOR_RESIZE_POLICIE(vec->count);
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
        BLOP_ASSERT_FORCED_VOID(data != NULL, "Failed to allocate memory for vector data");

        memcpy(vec->data, data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        BLOP_FREE(vec->data);
        vec->data = data;
    }

    vec->data[vec->count] = value;
    vec->count++;
}
void                    blopfn_vector_push_front(blopt_vector vec, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (vec->count == vec->capacity) {
        size_t capacity = BLOP_VECTOR_RESIZE_POLICIE(vec->count);
        BLOP_VECTOR_DATA_TYPE* data = (BLOP_VECTOR_DATA_TYPE*)calloc(capacity, sizeof(BLOP_VECTOR_DATA_TYPE));
        BLOP_ASSERT_FORCED_VOID(data != NULL, "Failed to allocate memory for vector data");

        memcpy(&data[1], vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        data[0] = value;

        BLOP_FREE(vec->data);
        vec->data = data;
        vec->capacity = capacity;
        vec->count++;
        return;
    }

    if (vec->count > 0) {
        memmove(&vec->data[1], &vec->data[0], vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
    }
    vec->data[0] = value;
    vec->count++;
}

void                    blopfn_vector_memcpy(blopt_vector vec, size_t idx, const BLOP_VECTOR_DATA_TYPE* src, size_t count) {
    BLOP_ASSERT_PTR_VOID(vec);
    BLOP_ASSERT_PTR_VOID(src);

    BLOP_ASSERT_BOUNDS_VOID(idx + count, vec->count + 1);

    memcpy(&vec->data[idx], src, count * sizeof(BLOP_VECTOR_DATA_TYPE));
}
void                    blopfn_vector_memset(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count) {
    BLOP_ASSERT_PTR_VOID(vec);

    BLOP_ASSERT_BOUNDS_VOID(idx + count, vec->count + 1);

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

#undef BLOP_VECTOR_NAME
#undef BLOP_VECTOR_DATA_TYPE
#undef BLOP_VECTOR_INITIAL_COUNT
#undef BLOP_VECTOR_RESIZE_POLICIE
#undef BLOP_VECTOR_DEALLOCATE_DATA
#undef BLOP_VECTOR_IMPLEMENTATION

#undef blopt_vector
#undef blops_vector   

#undef blopfn_vector_init
#undef blopfn_vector_deinit
#undef blopfn_vector_create    
#undef blopfn_vector_destroy

#undef blopfn_vector_rdlock  
#undef blopfn_vector_wrlock  
#undef blopfn_vector_rdunlock
#undef blopfn_vector_wrunlock

#undef blopfn_vector_get_data  
#undef blopfn_vector_get_count 
#undef blopfn_vector_get_back  
#undef blopfn_vector_get_front 

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