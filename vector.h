#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <blop/blop.h>

#ifndef BLOP_VECTOR_NAME
    #define BLOP_VECTOR_NAME vectorb
#endif

#ifndef BLOP_VECTOR_DATA_TYPE
    #define BLOP_VECTOR_DATA_TYPE uint8_t
#endif

#if !defined(BLOP_VECTOR_SCALATOR) || BLOP_VECTOR_SCALATOR < 2
    #define BLOP_VECTOR_SCALATOR 2
#endif

#if !defined(BLOP_VECTOR_INITIAL_COUNT) || BLOP_VECTOR_INITIAL_COUNT <= 0
    #define BLOP_VECTOR_INITIAL_COUNT 10
#endif

#define blopt_vector                BLOP_VECTOR_NAME
#define blops_vector                BLOP_CONCAT3(_, blopt_vector, _t)

#define blopfn_vector_create        BLOP_CONCAT2(blopt_vector, _create)
#define blopfn_vector_destroy       BLOP_CONCAT2(blopt_vector, _destroy)

#define blopfn_vector_set           BLOP_CONCAT2(blopt_vector, _set)
#define blopfn_vector_get           BLOP_CONCAT2(blopt_vector, _get)

#define blopfn_vector_insert        BLOP_CONCAT2(blopt_vector, _insert)
#define blopfn_vector_push_back     BLOP_CONCAT2(blopt_vector, _push_back)
#define blopfn_vector_push_front    BLOP_CONCAT2(blopt_vector, _push_front)

#define blopfn_vector_clear         BLOP_CONCAT2(blopt_vector, _clear)
#define blopfn_vector_erase         BLOP_CONCAT2(blopt_vector, _erase)
#define blopfn_vector_pop_back      BLOP_CONCAT2(blopt_vector, _pop_back)
#define blopfn_vector_pop_front     BLOP_CONCAT2(blopt_vector, _pop_front)

#define blopfn_vector_resize        BLOP_CONCAT2(blopt_vector, _resize)
#define blopfn_vector_shrink        BLOP_CONCAT2(blopt_vector, _shrink)

#define blopfn_vector_memcpy        BLOP_CONCAT2(blopt_vector, _memcpy)
#define blopfn_vector_memset        BLOP_CONCAT2(blopt_vector, _memset)

#define blopfn_vector_get_data      BLOP_CONCAT2(blopt_vector, _get_data)
#define blopfn_vector_get_count     BLOP_CONCAT2(blopt_vector, _get_count)
#define blopfn_vector_get_back      BLOP_CONCAT2(blopt_vector, _get_back)
#define blopfn_vector_get_front     BLOP_CONCAT2(blopt_vector, _get_front)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_vector* blopt_vector;

blopt_vector            blopfn_vector_create    ();
void                    blopfn_vector_destroy   (blopt_vector vec);

void                    blopfn_vector_set       (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get       (blopt_vector vec, size_t idx);

void                    blopfn_vector_insert    (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);
void                    blopfn_vector_push_back (blopt_vector vec, BLOP_VECTOR_DATA_TYPE value);
void                    blopfn_vector_push_front(blopt_vector vec, BLOP_VECTOR_DATA_TYPE value);

void                    blopfn_vector_clear     (blopt_vector vec);
void                    blopfn_vector_erase     (blopt_vector vec, size_t idx);
void                    blopfn_vector_pop_back  (blopt_vector vec);  
void                    blopfn_vector_pop_front (blopt_vector vec);

void                    blopfn_vector_resize    (blopt_vector vec, size_t count);
void                    blopfn_vector_shrink    (blopt_vector vec);

void                    blopfn_vector_memcpy    (blopt_vector vec, size_t idx, const BLOP_VECTOR_DATA_TYPE* src, size_t count);
void                    blopfn_vector_memset    (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count);

BLOP_VECTOR_DATA_TYPE*  blopfn_vector_get_data  (blopt_vector vec);  
size_t                  blopfn_vector_get_count (blopt_vector vec);
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_back  (blopt_vector vec);  
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_front (blopt_vector vec);

#ifdef BLOP_VECTOR_IMPLEMENTATION

struct blops_vector {
    BLOP_VECTOR_DATA_TYPE*  data;
    size_t                  count;
    size_t                  capacity;
};

blopt_vector            blopfn_vector_create() {
    blopt_vector vec = BLOP_MALLOC(struct blops_vector, 1);
    if (!vec) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    vec->data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, BLOP_VECTOR_INITIAL_COUNT);
    if (!vec->data) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector->data (returning NULL)");
        free(vec);
        BLOP_ABORT();
        return NULL;
    }

    memset(vec->data, 0, BLOP_VECTOR_INITIAL_COUNT * sizeof(BLOP_VECTOR_DATA_TYPE));
    vec->capacity = BLOP_VECTOR_INITIAL_COUNT;
    vec->count = 0;
    return vec;
}
void                    blopfn_vector_destroy(blopt_vector vec) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (vec->count > 0) {
        BLOP_ERROR_MESSAGE("A non empty vector can not be destroyed (returning without effect), clear the vector");
        BLOP_ABORT();
        return;
    }

    free(vec->data);
    free(vec);
}

void                    blopfn_vector_set(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (idx >= vec->count) {
        BLOP_OUT_OF_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_ABORT();
        return;
    }

    vec->data[idx] = value;
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get(blopt_vector vec, size_t idx) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    if (idx >= vec->count) {
        BLOP_OUT_OF_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_ABORT();
        return (BLOP_VECTOR_DATA_TYPE)0;
    }

    return vec->data[idx];
}

void                    blopfn_vector_insert(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (idx >= vec->count) {
        BLOP_OUT_OF_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_ABORT();
        return;
    }

    if (idx == 0) {
        blopfn_vector_push_front(vec, value);
        return;
    }

    if (idx == vec->count - 1) {
        blopfn_vector_push_back(vec, value);
        return;
    }

    if (vec->count == vec->capacity) {
        size_t capacity = vec->capacity * BLOP_VECTOR_SCALATOR;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, capacity);
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_ABORT();
            return;
        }

        /*
            The new element is inserted at index idx, so we need to copy:
            -   from 0 to idx-1, as idx element is the idx + 1 number of element
                we only need to copy idx elements (0 to idx-1 contains exactly idx number of elements)
            -   then we place the new element at index idx
            -   we copy from idx to count-1, which is (count - idx) elements
            -   finally we fill with zeros the remaining capacity
        */
        memcpy(data, vec->data, idx * sizeof(BLOP_VECTOR_DATA_TYPE));
        data[idx] = value;
        memcpy(&data[idx + 1], &vec->data[idx], (vec->count - idx) * sizeof(BLOP_VECTOR_DATA_TYPE));
        memset(&data[vec->count + 1], 0, (capacity - (vec->count + 1)) * sizeof(BLOP_VECTOR_DATA_TYPE));

        free(vec->data);
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
        vec->capacity *= BLOP_VECTOR_SCALATOR;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
        memcpy(vec->data, data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        free(vec->data);
        vec->data = data;
    }

    vec->data[vec->count] = value;
    vec->count++;
}
void                    blopfn_vector_push_front(blopt_vector vec, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (vec->count == vec->capacity) {
        size_t capacity = vec->capacity * BLOP_VECTOR_SCALATOR;
        BLOP_VECTOR_DATA_TYPE* data = (BLOP_VECTOR_DATA_TYPE*)calloc(capacity, sizeof(BLOP_VECTOR_DATA_TYPE));
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_ABORT();
            return;
        }

        memcpy(&data[1], vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        data[0] = value;

        free(vec->data);
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

void                    blopfn_vector_clear(blopt_vector vec) {
    BLOP_ASSERT_PTR_VOID(vec);

    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
        for (size_t i = 0; i < vec->count; i++) {
            BLOP_VECTOR_DEALLOCATE_DATA(vec->data[i]);
        }
    #endif

    free(vec->data);
    vec->count = 0;
    vec->capacity = BLOP_VECTOR_INITIAL_COUNT;
    vec->data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
    if (!vec->data) {
        free(vec);
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (vector was deallocated)");
        BLOP_ABORT();
    }
}
void                    blopfn_vector_erase(blopt_vector vec, size_t idx) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (idx >= vec->count) {
        BLOP_OUT_OF_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_ABORT();
        return;
    }

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
    #endif

    memmove(&vec->data[idx], &vec->data[idx + 1], ((vec->count - idx) - 1) * sizeof(BLOP_VECTOR_DATA_TYPE));
    vec->count--;

    blopfn_vector_shrink(vec);
}
void                    blopfn_vector_pop_back(blopt_vector vec) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning without effect)");
        BLOP_ABORT();
        return;
    }

    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
        BLOP_VECTOR_DEALLOCATE_DATA(vec->data[vec->count - 1]);
    #endif

    vec->count--;

    blopfn_vector_shrink(vec);
}
void                    blopfn_vector_pop_front(blopt_vector vec) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning without effect)");
        BLOP_ABORT();
        return;
    }

    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
        BLOP_VECTOR_DEALLOCATE_DATA(vec->data[0]);
    #endif

    if (vec->count > 1) {
        memmove(&vec->data[0], &vec->data[1], (vec->count - 1) * sizeof(BLOP_VECTOR_DATA_TYPE));
    }
    vec->count--;

    blopfn_vector_shrink(vec);
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
        #endif
        vec->count = count;
        blopfn_vector_shrink(vec);
        return;
    }

    if (count > vec->capacity) {
        vec->capacity = count;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_ABORT();
            return;
        }

        memcpy(data, vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        free(vec->data);
        vec->data = data;
    }

    memset(&vec->data[vec->count], 0, (count - vec->count) * sizeof(BLOP_VECTOR_DATA_TYPE));
    vec->count = count;
}
void                    blopfn_vector_shrink(blopt_vector vec) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (vec->count < vec->capacity / (BLOP_VECTOR_SCALATOR * 2) && vec->count > BLOP_VECTOR_INITIAL_COUNT) {
        vec->capacity = vec->count * BLOP_VECTOR_SCALATOR;
        vec->capacity = (vec->capacity < BLOP_VECTOR_INITIAL_COUNT) ? BLOP_VECTOR_INITIAL_COUNT : vec->capacity;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_ABORT();
            return;
        }

        memcpy(data, vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        free(vec->data);
        vec->data = data;
    }
}

void                    blopfn_vector_memcpy(blopt_vector vec, size_t idx, const BLOP_VECTOR_DATA_TYPE* src, size_t count) {
    BLOP_ASSERT_PTR_VOID(vec);
    BLOP_ASSERT_PTR_VOID(src);

    if (idx + count > vec->count) {
        BLOP_OUT_OF_BONDS("Index out of bounds (returning without effect)", idx + count, vec->count);
        BLOP_ABORT();
        return;
    }

    memcpy(&vec->data[idx], src, count * sizeof(BLOP_VECTOR_DATA_TYPE));
}
void                    blopfn_vector_memset(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count) {
    BLOP_ASSERT_PTR_VOID(vec);

    if (idx + count > vec->count) {
        BLOP_OUT_OF_BONDS("Index out of bounds (returning without effect)", idx + count, vec->count);
        BLOP_ABORT();
        return;
    }

    for (size_t i = 0; i < count; i++) {
        vec->data[idx + i] = value;
    }
}

BLOP_VECTOR_DATA_TYPE*  blopfn_vector_get_data(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, NULL);
    return vec->data;
}
size_t                  blopfn_vector_get_count(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, 0);
    return vec->count;
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_back(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning 0)");
        BLOP_ABORT();
        return (BLOP_VECTOR_DATA_TYPE)0;
    }

    return vec->data[vec->count - 1];
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_front(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning 0)");
        BLOP_ABORT();
        return (BLOP_VECTOR_DATA_TYPE)0;
    }

    return vec->data[0];
}

#endif

#ifdef __cplusplus
}
#endif

#undef BLOP_VECTOR_NAME
#undef BLOP_VECTOR_DATA_TYPE
#undef BLOP_VECTOR_SCALATOR
#undef BLOP_VECTOR_INITIAL_COUNT
#undef BLOP_VECTOR_DEALLOCATE_DATA
#undef BLOP_VECTOR_IMPLEMENTATION

#undef blops_vector   
#undef blopt_vector

#undef blopfn_vector_create    
#undef blopfn_vector_destroy

#undef blopfn_vector_set       
#undef blopfn_vector_get
       
#undef blopfn_vector_insert     
#undef blopfn_vector_push_back     
#undef blopfn_vector_push_front  
#undef blopfn_vector_clear

#undef blopfn_vector_erase    
#undef blopfn_vector_pop_back 
#undef blopfn_vector_pop_front

#undef blopfn_vector_resize
#undef blopfn_vector_shrink

#undef blopfn_vector_memcpy    
#undef blopfn_vector_memset

#undef blopfn_vector_get_data  
#undef blopfn_vector_get_count 
#undef blopfn_vector_get_back  
#undef blopfn_vector_get_front 