#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <blop/blop.h>

#ifndef BLOP_VECTOR_NAME
    #define BLOP_VECTOR_NAME blop
#endif

#ifndef BLOP_VECTOR_DATA_TYPE
    #define BLOP_VECTOR_DATA_TYPE uint8_t
#endif

#if !defined(BLOP_VECTOR_INITIAL_COUNT) || BLOP_VECTOR_INITIAL_COUNT <= 0
    #define BLOP_VECTOR_INITIAL_COUNT 10
#endif

#if !defined(BLOP_VECTOR_SCALATOR) || BLOP_VECTOR_SCALATOR < 2
    #define BLOP_VECTOR_SCALATOR 2
#endif

#ifdef BLOP_VECTOR_SAFE_MODE
    #define BLOP_VECTOR_ASSERT_PTR(ptr, rt) if (ptr == NULL) {BLOP_ERROR_MESSAGE(#ptr " is a null ptr (returning without effect)"); BLOP_VECTOR_EXIT; return rt;}
    #define BLOP_VECTOR_ASSERT_PTR_VOID(ptr) if (ptr == NULL) {BLOP_ERROR_MESSAGE(#ptr " is a null ptr (returning without effect)"); BLOP_VECTOR_EXIT; return;}
#else
    #define BLOP_VECTOR_ASSERT_PTR(ptr, rt)
    #define BLOP_VECTOR_ASSERT_PTR_VOID(ptr)
#endif

#ifdef BLOP_VECTOR_EXIT_ON_ERROR
    #define BLOP_VECTOR_EXIT exit(-1)
#else
    #define BLOP_VECTOR_EXIT
#endif

#ifndef BLOP_VECTOR_CAMEL

    #define _BLOPS_VECTOR               BLOP_CONCAT3(_, BLOP_VECTOR_NAME, _t)
    #define _BLOPT_VECTOR               BLOP_VECTOR_NAME

    #define _BLOPF_VECTOR_CREATE        BLOP_CONCAT2(BLOP_VECTOR_NAME, _create)
    #define _BLOPF_VECTOR_DESTROY       BLOP_CONCAT2(BLOP_VECTOR_NAME, _destroy)

    #define _BLOPF_VECTOR_SET           BLOP_CONCAT2(BLOP_VECTOR_NAME, _set)
    #define _BLOPF_VECTOR_GET           BLOP_CONCAT2(BLOP_VECTOR_NAME, _get)

    #define _BLOPF_VECTOR_INSERT        BLOP_CONCAT2(BLOP_VECTOR_NAME, _insert)
    #define _BLOPF_VECTOR_PUSH_BACK     BLOP_CONCAT2(BLOP_VECTOR_NAME, _push_back)
    #define _BLOPF_VECTOR_PUSH_FRONT    BLOP_CONCAT2(BLOP_VECTOR_NAME, _push_front)

    #define _BLOPF_VECTOR_CLEAR         BLOP_CONCAT2(BLOP_VECTOR_NAME, _clear)
    #define _BLOPF_VECTOR_ERASE         BLOP_CONCAT2(BLOP_VECTOR_NAME, _erase)
    #define _BLOPF_VECTOR_POP_BACK      BLOP_CONCAT2(BLOP_VECTOR_NAME, _pop_back)
    #define _BLOPF_VECTOR_POP_FRONT     BLOP_CONCAT2(BLOP_VECTOR_NAME, _pop_front)

    #define _BLOPF_VECTOR_RESIZE        BLOP_CONCAT2(BLOP_VECTOR_NAME, _resize)
    #define _BLOPF_VECTOR_SHRINK        BLOP_CONCAT2(BLOP_VECTOR_NAME, _shrink)

    #define _BLOPF_VECTOR_MEMCPY        BLOP_CONCAT2(BLOP_VECTOR_NAME, _memcpy)
    #define _BLOPF_VECTOR_MEMSET        BLOP_CONCAT2(BLOP_VECTOR_NAME, _memset)

    #define _BLOPF_VECTOR_GET_DATA      BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_data)
    #define _BLOPF_VECTOR_GET_COUNT     BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_count)
    #define _BLOPF_VECTOR_GET_BACK      BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_back)
    #define _BLOPF_VECTOR_GET_FRONT     BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_front)

#else // BLOP_VECTOR_CAMEL

    #define _BLOPS_VECTOR               BLOP_CONCAT3(_, BLOP_VECTOR_NAME, _t)
    #define _BLOPT_VECTOR               BLOP_VECTOR_NAME

    #define _BLOPF_VECTOR_CREATE        BLOP_CONCAT2(BLOP_VECTOR_NAME, Create)
    #define _BLOPF_VECTOR_DESTROY       BLOP_CONCAT2(BLOP_VECTOR_NAME, Destroy)

    #define _BLOPF_VECTOR_SET           BLOP_CONCAT2(BLOP_VECTOR_NAME, Set)
    #define _BLOPF_VECTOR_GET           BLOP_CONCAT2(BLOP_VECTOR_NAME, Get)

    #define _BLOPF_VECTOR_CLEAR         BLOP_CONCAT2(BLOP_VECTOR_NAME, Clear)
    #define _BLOPF_VECTOR_ERASE         BLOP_CONCAT2(BLOP_VECTOR_NAME, Erase)
    #define _BLOPF_VECTOR_POP_BACK      BLOP_CONCAT2(BLOP_VECTOR_NAME, PopBack)
    #define _BLOPF_VECTOR_POP_FRONT     BLOP_CONCAT2(BLOP_VECTOR_NAME, PopFront)

    #define _BLOPF_VECTOR_INSERT        BLOP_CONCAT2(BLOP_VECTOR_NAME, Insert)
    #define _BLOPF_VECTOR_PUSH_BACK     BLOP_CONCAT2(BLOP_VECTOR_NAME, PushBack)
    #define _BLOPF_VECTOR_PUSH_FRONT    BLOP_CONCAT2(BLOP_VECTOR_NAME, PushFront)

    #define _BLOPF_VECTOR_RESIZE        BLOP_CONCAT2(BLOP_VECTOR_NAME, Resize)
    #define _BLOPF_VECTOR_SHRINK        BLOP_CONCAT2(BLOP_VECTOR_NAME, Shrink)

    #define _BLOPF_VECTOR_MEMCPY        BLOP_CONCAT2(BLOP_VECTOR_NAME, Memcpy)
    #define _BLOPF_VECTOR_MEMSET        BLOP_CONCAT2(BLOP_VECTOR_NAME, Memset)

    #define _BLOPF_VECTOR_GET_DATA      BLOP_CONCAT2(BLOP_VECTOR_NAME, GetData)
    #define _BLOPF_VECTOR_GET_COUNT     BLOP_CONCAT2(BLOP_VECTOR_NAME, GetCount)
    #define _BLOPF_VECTOR_GET_BACK      BLOP_CONCAT2(BLOP_VECTOR_NAME, GetBack)
    #define _BLOPF_VECTOR_GET_FRONT     BLOP_CONCAT2(BLOP_VECTOR_NAME, GetFront)

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BLOPS_VECTOR* _BLOPT_VECTOR;

_BLOPT_VECTOR           _BLOPF_VECTOR_CREATE();
void                    _BLOPF_VECTOR_DESTROY(_BLOPT_VECTOR vec);

void                    _BLOPF_VECTOR_SET(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);
BLOP_VECTOR_DATA_TYPE   _BLOPF_VECTOR_GET(_BLOPT_VECTOR vec, size_t idx);

void                    _BLOPF_VECTOR_INSERT(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);
void                    _BLOPF_VECTOR_PUSH_BACK(_BLOPT_VECTOR vec, BLOP_VECTOR_DATA_TYPE value);
void                    _BLOPF_VECTOR_PUSH_FRONT(_BLOPT_VECTOR vec, BLOP_VECTOR_DATA_TYPE value);

void                    _BLOPF_VECTOR_CLEAR(_BLOPT_VECTOR vec);
void                    _BLOPF_VECTOR_ERASE(_BLOPT_VECTOR vec, size_t idx);
void                    _BLOPF_VECTOR_POP_BACK(_BLOPT_VECTOR vec);  
void                    _BLOPF_VECTOR_POP_FRONT(_BLOPT_VECTOR vec);

void                    _BLOPF_VECTOR_RESIZE(_BLOPT_VECTOR vec, size_t count);
void                    _BLOPF_VECTOR_SHRINK(_BLOPT_VECTOR vec);

void                    _BLOPF_VECTOR_MEMCPY(_BLOPT_VECTOR vec, size_t idx, const BLOP_VECTOR_DATA_TYPE* src, size_t count);
void                    _BLOPF_VECTOR_MEMSET(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count);

BLOP_VECTOR_DATA_TYPE*  _BLOPF_VECTOR_GET_DATA(_BLOPT_VECTOR vec);  
size_t                  _BLOPF_VECTOR_GET_COUNT(_BLOPT_VECTOR vec);
BLOP_VECTOR_DATA_TYPE   _BLOPF_VECTOR_GET_BACK(_BLOPT_VECTOR vec);  
BLOP_VECTOR_DATA_TYPE   _BLOPF_VECTOR_GET_FRONT(_BLOPT_VECTOR vec);

#ifdef BLOP_VECTOR_IMPLEMENTATION

struct _BLOPS_VECTOR {
    BLOP_VECTOR_DATA_TYPE*  data;
    size_t                  count;
    size_t                  capacity;
};

_BLOPT_VECTOR _BLOPF_VECTOR_CREATE() {
    _BLOPT_VECTOR vec = BLOP_MALLOC(struct _BLOPS_VECTOR, 1);
    if (!vec) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector (returning NULL)");
        BLOP_VECTOR_EXIT;
        return NULL;
    }

    vec->data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, BLOP_VECTOR_INITIAL_COUNT);
    if (!vec->data) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector->data (returning NULL)");
        free(vec);
        BLOP_VECTOR_EXIT;
        return NULL;
    }

    memset(vec->data, 0, BLOP_VECTOR_INITIAL_COUNT * sizeof(BLOP_VECTOR_DATA_TYPE));
    vec->capacity = BLOP_VECTOR_INITIAL_COUNT;
    vec->count = 0;
    return vec;
}
void _BLOPF_VECTOR_DESTROY(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (vec->count > 0) {
        BLOP_ERROR_MESSAGE("A non empty vector can not be destroyed (returning without effect), clear the vector");
        BLOP_VECTOR_EXIT;
        return;
    }

    free(vec->data);
    free(vec);
}

void _BLOPF_VECTOR_SET(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (idx >= vec->count) {
        BLOP_ERROR_MESSAGE_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_VECTOR_EXIT;
        return;
    }

    vec->data[idx] = value;
}
BLOP_VECTOR_DATA_TYPE _BLOPF_VECTOR_GET(_BLOPT_VECTOR vec, size_t idx) {
    BLOP_VECTOR_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    if (idx >= vec->count) {
        BLOP_ERROR_MESSAGE_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_VECTOR_EXIT;
        return (BLOP_VECTOR_DATA_TYPE)0;
    }

    return vec->data[idx];
}

void _BLOPF_VECTOR_INSERT(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (idx >= vec->count) {
        BLOP_ERROR_MESSAGE_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_VECTOR_EXIT;
        return;
    }

    if (idx == 0) {
        _BLOPF_VECTOR_PUSH_FRONT(vec, value);
        return;
    }

    if (idx == vec->count - 1) {
        _BLOPF_VECTOR_PUSH_BACK(vec, value);
        return;
    }

    if (vec->count == vec->capacity) {
        size_t capacity = vec->capacity * BLOP_VECTOR_SCALATOR;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, capacity);
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_VECTOR_EXIT;
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
void _BLOPF_VECTOR_PUSH_BACK(_BLOPT_VECTOR vec, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

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
void _BLOPF_VECTOR_PUSH_FRONT(_BLOPT_VECTOR vec, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (vec->count == vec->capacity) {
        size_t capacity = vec->capacity * BLOP_VECTOR_SCALATOR;
        BLOP_VECTOR_DATA_TYPE* data = (BLOP_VECTOR_DATA_TYPE*)calloc(capacity, sizeof(BLOP_VECTOR_DATA_TYPE));
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_VECTOR_EXIT;
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

void _BLOPF_VECTOR_CLEAR(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

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
        BLOP_VECTOR_EXIT;
    }
}
void _BLOPF_VECTOR_ERASE(_BLOPT_VECTOR vec, size_t idx) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (idx >= vec->count) {
        BLOP_ERROR_MESSAGE_BONDS("Index out of bounds (returning without effect)", idx, vec->count);
        BLOP_VECTOR_EXIT;
        return;
    }

    if (idx == 0) {
        _BLOPF_VECTOR_POP_FRONT(vec);
        return;
    }

    if (idx == vec->count - 1) {
        _BLOPF_VECTOR_POP_BACK(vec);
        return;
    }

    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
        BLOP_VECTOR_DEALLOCATE_DATA(vec->data[idx]);
    #endif

    memmove(&vec->data[idx], &vec->data[idx + 1], ((vec->count - idx) - 1) * sizeof(BLOP_VECTOR_DATA_TYPE));
    vec->count--;

    _BLOPF_VECTOR_SHRINK(vec);
}
void _BLOPF_VECTOR_POP_BACK(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning without effect)");
        BLOP_VECTOR_EXIT;
        return;
    }

    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
        BLOP_VECTOR_DEALLOCATE_DATA(vec->data[vec->count - 1]);
    #endif

    vec->count--;

    _BLOPF_VECTOR_SHRINK(vec);
}
void _BLOPF_VECTOR_POP_FRONT(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning without effect)");
        BLOP_VECTOR_EXIT;
        return;
    }

    #ifdef BLOP_VECTOR_DEALLOCATE_DATA
        BLOP_VECTOR_DEALLOCATE_DATA(vec->data[0]);
    #endif

    if (vec->count > 1) {
        memmove(&vec->data[0], &vec->data[1], (vec->count - 1) * sizeof(BLOP_VECTOR_DATA_TYPE));
    }
    vec->count--;

    _BLOPF_VECTOR_SHRINK(vec);
}

void _BLOPF_VECTOR_RESIZE(_BLOPT_VECTOR vec, size_t count) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

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
        _BLOPF_VECTOR_SHRINK(vec);
        return;
    }

    if (count > vec->capacity) {
        vec->capacity = count;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_VECTOR_EXIT;
            return;
        }

        memcpy(data, vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        free(vec->data);
        vec->data = data;
    }

    memset(&vec->data[vec->count], 0, (count - vec->count) * sizeof(BLOP_VECTOR_DATA_TYPE));
    vec->count = count;
}
void _BLOPF_VECTOR_SHRINK(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (vec->count < vec->capacity / (BLOP_VECTOR_SCALATOR * 2) && vec->count > BLOP_VECTOR_INITIAL_COUNT) {
        vec->capacity = vec->count * BLOP_VECTOR_SCALATOR;
        vec->capacity = (vec->capacity < BLOP_VECTOR_INITIAL_COUNT) ? BLOP_VECTOR_INITIAL_COUNT : vec->capacity;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data (returning without effect)");
            BLOP_VECTOR_EXIT;
            return;
        }

        memcpy(data, vec->data, vec->count * sizeof(BLOP_VECTOR_DATA_TYPE));
        free(vec->data);
        vec->data = data;
    }
}

void _BLOPF_VECTOR_MEMCPY(_BLOPT_VECTOR vec, size_t idx, const BLOP_VECTOR_DATA_TYPE* src, size_t count) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);
    BLOP_VECTOR_ASSERT_PTR_VOID(src);

    if (idx + count > vec->count) {
        BLOP_ERROR_MESSAGE_BONDS("Index out of bounds (returning without effect)", idx + count, vec->count);
        BLOP_VECTOR_EXIT;
        return;
    }

    memcpy(&vec->data[idx], src, count * sizeof(BLOP_VECTOR_DATA_TYPE));
}
void _BLOPF_VECTOR_MEMSET(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count) {
    BLOP_VECTOR_ASSERT_PTR_VOID(vec);

    if (idx + count > vec->count) {
        BLOP_ERROR_MESSAGE_BONDS("Index out of bounds (returning without effect)", idx + count, vec->count);
        BLOP_VECTOR_EXIT;
        return;
    }

    for (size_t i = 0; i < count; i++) {
        vec->data[idx + i] = value;
    }
}

BLOP_VECTOR_DATA_TYPE* _BLOPF_VECTOR_GET_DATA(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR(vec, NULL);
    return vec->data;
}
size_t _BLOPF_VECTOR_GET_COUNT(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR(vec, 0);
    return vec->count;
}
BLOP_VECTOR_DATA_TYPE _BLOPF_VECTOR_GET_BACK(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning 0)");
        BLOP_VECTOR_EXIT;
        return (BLOP_VECTOR_DATA_TYPE)0;
    }

    return vec->data[vec->count - 1];
}
BLOP_VECTOR_DATA_TYPE _BLOPF_VECTOR_GET_FRONT(_BLOPT_VECTOR vec) {
    BLOP_VECTOR_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    if (vec->count == 0) {
        BLOP_ERROR_MESSAGE("The vector is empty (returning 0)");
        BLOP_VECTOR_EXIT;
        return (BLOP_VECTOR_DATA_TYPE)0;
    }

    return vec->data[0];
}

#endif

#ifdef __cplusplus
}
#endif

#undef BLOP_VECTOR_SAFE_MODE
#undef BLOP_VECTOR_EXIT_ON_ERROR
#undef BLOP_VECTOR_ASSERT_PTR
#undef BLOP_VECTOR_EXIT

#undef BLOP_VECTOR_NAME
#undef BLOP_VECTOR_DATA_TYPE
#undef BLOP_VECTOR_DEALLOCATE_DATA
#undef BLOP_VECTOR_INITIAL_COUNT
#undef BLOP_VECTOR_SCALATOR

#undef BLOP_VECTOR_CAMEL
#undef BLOP_VECTOR_IMPLEMENTATION

#undef _BLOPS_VECTOR           
#undef _BLOPT_VECTOR

#undef _BLOPF_VECTOR_CREATE    
#undef _BLOPF_VECTOR_DESTROY

#undef _BLOPF_VECTOR_SET       
#undef _BLOPF_VECTOR_GET
       
#undef _BLOPF_VECTOR_CLEAR     
#undef _BLOPF_VECTOR_ERASE     
#undef _BLOPF_VECTOR_POP_BACK  
#undef _BLOPF_VECTOR_POP_FRONT

#undef _BLOPF_VECTOR_INSERT    
#undef _BLOPF_VECTOR_PUSH_BACK 
#undef _BLOPF_VECTOR_PUSH_FRONT

#undef _BLOPF_VECTOR_RESIZE
#undef _BLOPF_VECTOR_SHRINK

#undef _BLOPF_VECTOR_MEMCPY    
#undef _BLOPF_VECTOR_MEMSET

#undef _BLOPF_VECTOR_GET_DATA  
#undef _BLOPF_VECTOR_GET_COUNT 
#undef _BLOPF_VECTOR_GET_BACK  
#undef _BLOPF_VECTOR_GET_FRONT 