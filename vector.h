/**
 * @file vector.h
 * @brief Configurable, header-only dynamic array (vector) implementation.
 *
 * This header emits a typed, configurable dynamic array API when included.
 * Configure the generated type and behavior using the following macros prior
 * to inclusion:
 *
 * - BLOP_VECTOR_NAME:       Identifier for the generated vector type and API
 *                          (default: `vectorb`).
 * - BLOP_VECTOR_DATA_TYPE:  Element type stored in the vector
 *                          (default: `uint8_t`).
 * - BLOP_VECTOR_SCALATOR:   Growth factor used when expanding capacity
 *                          (default: 2; minimum is 2).
 * - BLOP_VECTOR_INITIAL_COUNT:
 *                          Initial capacity when a vector is created
 *                          (default: 10).
 * - BLOP_VECTOR_DEALLOCATE_DATA:
 *                          Optional macro/function called to deallocate an
 *                          element when removed (should accept one argument).
 * - BLOP_VECTOR_IMPLEMENTATION:
 *                          Define in exactly one translation unit to emit the
 *                          function implementations. Without this only the
 *                          declarations are produced.
 *
 * The API is emitted under mangled names derived from BLOP_VECTOR_NAME. Use
 * the produced symbols (functions/typedefs/macros) to manipulate the vector.
 *
 * @author Pedro Caceres
 * @date 2025-11-16
 */

#include <blop/blop.h>

/**
 * @defgroup vector_config Configuration macros
 * @{
 */

/**
 * @brief Name used to build the public API identifiers.
 *
 * Example: defining BLOP_VECTOR_NAME myvec will produce functions such as
 * `myvec_create`, `myvec_push_back`, etc.
 */
#ifndef BLOP_VECTOR_NAME
    #define BLOP_VECTOR_NAME vectorb
#endif /* BLOP_VECTOR_NAME */

/**
 * @brief Type stored in each element of the vector.
 *
 * Default is `uint8_t`. To store ints, structs, or pointers set this before
 * including the header.
 */
#ifndef BLOP_VECTOR_DATA_TYPE
    #define BLOP_VECTOR_DATA_TYPE uint8_t
#endif /* BLOP_VECTOR_DATA_TYPE */

/**
 * @brief Growth factor to use when increasing capacity.
 *
 * Must be >= 2; if not defined or less than 2 the default 2 is used.
 */
#if !defined(BLOP_VECTOR_SCALATOR) || BLOP_VECTOR_SCALATOR < 2
    #define BLOP_VECTOR_SCALATOR 2
#endif /* BLOP_VECTOR_SCALATOR */

/**
 * @brief Initial number of elements allocated for a new vector.
 */
#if !defined(BLOP_VECTOR_INITIAL_COUNT) || BLOP_VECTOR_INITIAL_COUNT <= 0
    #define BLOP_VECTOR_INITIAL_COUNT 10
#endif /* BLOP_VECTOR_INITIAL_COUNT */

/** @} */ /* end vector_config */

/**
 * @defgroup vector_tokens Generated token macros
 * @{
 *
 * These internal macros unify naming: public function and type names are
 * produced by concatenating BLOP_VECTOR_NAME with suffixes.
 */

/** @brief Opaque pointer typedef name for the generated vector. */
#define blopt_vector                BLOP_VECTOR_NAME
/** @brief Internal struct tag for the concrete vector when IMPLEMENTATION is defined. */
#define blops_vector                BLOP_CONCAT3(_, blopt_vector, _t)

/** @brief Function name tokens generated from the chosen vector name. */
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

/** @} */ /* end vector_tokens */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef blopt_vector
 * @brief Opaque pointer to the generated vector type.
 *
 * When BLOP_VECTOR_IMPLEMENTATION is defined the concrete struct is declared
 * and this typedef resolves to `struct blops_vector *`.
 */
typedef struct blops_vector* blopt_vector;

/**
 * @name Public API: creation / destruction
 * @{
 */

/**
 * @brief Allocate and initialize a new vector instance.
 *
 * Behavior:
 * - Allocates the vector structure and an internal buffer with
 *   BLOP_VECTOR_INITIAL_COUNT capacity.
 * - On allocation failure prints an error and calls BLOP_ABORT (may exit/abort).
 *
 * @return Pointer to the new vector, or NULL on failure (after BLOP_ABORT if configured).
 */
blopt_vector            blopfn_vector_create    ();

/**
 * @brief Destroy a vector instance.
 *
 * Requirements:
 * - The vector must be empty (count == 0) when destroyed. Use clear() to
 *   remove elements first.
 *
 * @param vec Vector to destroy (may be NULL - implementation asserts).
 */
void                    blopfn_vector_destroy   (blopt_vector vec);

/** @} */ /* end creation / destruction */

/**
 * @name Public API: element access
 * @{
 */

/**
 * @brief Set element value at index.
 * @param vec Vector to modify.
 * @param idx Zero-based index (must be < count).
 * @param value Value to store.
 */
void                    blopfn_vector_set       (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);

/**
 * @brief Get element value at index.
 * @param vec Vector to query.
 * @param idx Zero-based index (must be < count).
 * @return Stored value, or a cast-zero when vec is NULL or on error.
 */
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get       (blopt_vector vec, size_t idx);

/** @} */ /* end element access */

/**
 * @name Public API: insertion / removal
 * @{
 */

/**
 * @brief Insert value at index shifting subsequent elements right.
 * @param vec Vector to modify.
 * @param idx Index to insert at (0..count).
 * @param value Value to insert.
 */
void                    blopfn_vector_insert    (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);

/**
 * @brief Append value to the end of the vector.
 * @param vec Vector to modify.
 * @param value Value to append.
 */
void                    blopfn_vector_push_back (blopt_vector vec, BLOP_VECTOR_DATA_TYPE value);

/**
 * @brief Prepend value to the start of the vector.
 * @param vec Vector to modify.
 * @param value Value to prepend.
 */
void                    blopfn_vector_push_front(blopt_vector vec, BLOP_VECTOR_DATA_TYPE value);

/**
 * @brief Remove all elements from the vector.
 *
 * If BLOP_VECTOR_DEALLOCATE_DATA is defined it is called for each element
 * prior to clearing the underlying buffer.
 *
 * After clearing the internal buffer is reset to BLOP_VECTOR_INITIAL_COUNT.
 *
 * @param vec Vector to clear.
 */
void                    blopfn_vector_clear     (blopt_vector vec);

/**
 * @brief Erase element at index shifting subsequent elements left.
 * @param vec Vector to modify.
 * @param idx Index of element to erase (0..count-1).
 */
void                    blopfn_vector_erase     (blopt_vector vec, size_t idx);

/**
 * @brief Pop the last element.
 * @param vec Vector to modify.
 */
void                    blopfn_vector_pop_back  (blopt_vector vec);

/**
 * @brief Pop the first element.
 * @param vec Vector to modify.
 */
void                    blopfn_vector_pop_front (blopt_vector vec);

/** @} */ /* end insertion / removal */

/**
 * @name Public API: resizing / capacity management
 * @{
 */

/**
 * @brief Ensure vector has exactly `count` elements.
 *
 * - If count > current count, capacity may be expanded and new elements are
 *   zero-initialized.
 * - If count < current count, elements beyond `count` are optionally
 *   deallocated via BLOP_VECTOR_DEALLOCATE_DATA if defined.
 *
 * @param vec Vector to resize.
 * @param count New element count.
 */
void                    blopfn_vector_resize    (blopt_vector vec, size_t count);

/**
 * @brief Attempt to shrink capacity to match usage.
 *
 * The implementation reduces capacity when the used count is much smaller
 * than the allocated capacity according to BLOP_VECTOR_SCALATOR heuristics.
 *
 * @param vec Vector to shrink.
 */
void                    blopfn_vector_shrink    (blopt_vector vec);

/** @} */ /* end resizing / capacity management */

/**
 * @name Public API: bulk memory ops
 * @{
 */

/**
 * @brief Copy `count` elements from `src` into vector starting at `idx`.
 * @param vec Target vector.
 * @param idx Destination index.
 * @param src  Source array (must not be NULL).
 * @param count Number of elements to copy.
 */
void                    blopfn_vector_memcpy    (blopt_vector vec, size_t idx, const BLOP_VECTOR_DATA_TYPE* src, size_t count);

/**
 * @brief Set `count` elements starting at `idx` to `value`.
 * @param vec Target vector.
 * @param idx Start index.
 * @param value Value to set.
 * @param count Number of elements to set.
 */
void                    blopfn_vector_memset    (blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count);

/** @} */ /* end bulk memory ops */

/**
 * @name Public API: inspectors
 * @{
 */

/**
 * @brief Get pointer to internal data buffer.
 * @param vec Vector to query.
 * @return Pointer to data buffer or NULL if vec is NULL.
 *
 * Note: Direct writes into the buffer bypass bounds checks; use with care.
 */
BLOP_VECTOR_DATA_TYPE*  blopfn_vector_get_data  (blopt_vector vec);

/**
 * @brief Get number of elements currently stored.
 * @param vec Vector to query.
 * @return Element count (0 if vec is NULL).
 */
size_t                  blopfn_vector_get_count (blopt_vector vec);

/**
 * @brief Get last element value.
 * @param vec Vector to query.
 * @return Value at back (or cast-zero on error).
 */
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_back  (blopt_vector vec);

/**
 * @brief Get first element value.
 * @param vec Vector to query.
 * @return Value at front (or cast-zero on error).
 */
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_front (blopt_vector vec);

/** @} */ /* end inspectors */

#ifdef BLOP_VECTOR_IMPLEMENTATION

/**
 * @internal
 * @struct blops_vector
 * @brief Concrete vector representation (visible only when IMPLEMENTATION is defined).
 *
 * Fields:
 * - data: pointer to the element buffer (allocated on create/resize).
 * - count: number of elements currently present.
 * - capacity: current allocated capacity.
 */
struct blops_vector {
    BLOP_VECTOR_DATA_TYPE*  data;   /**< Element buffer (size capacity) */
    size_t                  count;  /**< Number of elements used */
    size_t                  capacity;/**< Allocated capacity (in elements) */
};

/* Implementation follows: functions defined above are implemented here.
   Each function documents its behavior in the declaration section. */

blopt_vector            blopfn_vector_create() {
    blopt_vector vec = BLOP_MALLOC(struct blops_vector, 1);
    if (!vec) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector structure");
        BLOP_ABORT();
        return NULL;
    }

    vec->data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, BLOP_VECTOR_INITIAL_COUNT);
    if (!vec->data) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector->data");
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

    BLOP_ASSERT_VOID(vec->count == 0, "A non empty vector can not be destroyed (returning without effect), clear the vector");

    free(vec->data);
    free(vec);
}

void                    blopfn_vector_set(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    BLOP_ASSERT_BONDS_VOID(idx, vec->count);

    vec->data[idx] = value;
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get(blopt_vector vec, size_t idx) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    BLOP_ASSERT_BONDS(idx, vec->count, (BLOP_VECTOR_DATA_TYPE)0);

    return vec->data[idx];
}

void                    blopfn_vector_insert(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value) {
    BLOP_ASSERT_PTR_VOID(vec);

    BLOP_ASSERT_BONDS_VOID(idx, vec->count);

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
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data");
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
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector data");
            BLOP_ABORT();
            return;
        }

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
    #endif /* BLOP_VECTOR_DEALLOCATE_DATA */

    free(vec->data);
    vec->count = 0;
    vec->capacity = BLOP_VECTOR_INITIAL_COUNT;
    vec->data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
    if (!vec->data) {
        free(vec);
        BLOP_ERROR_MESSAGE("Failed to allocate memory for vector->data");
        BLOP_ABORT();
    }
}
void                    blopfn_vector_erase(blopt_vector vec, size_t idx) {
    BLOP_ASSERT_PTR_VOID(vec);

    BLOP_ASSERT_BONDS_VOID(idx, vec->count);

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
        BLOP_EMPTY_POPPING("To enable empty popping and avoid this error define BLOP_VALID_EMPTY_POPPING");
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
        BLOP_EMPTY_POPPING("To enable empty popping and avoid this error define BLOP_VALID_EMPTY_POPPING");
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
        vec->capacity = count;
        BLOP_VECTOR_DATA_TYPE* data = BLOP_MALLOC(BLOP_VECTOR_DATA_TYPE, vec->capacity);
        if (!data) {
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector->data");
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
            BLOP_ERROR_MESSAGE("Failed to allocate memory for vector->data");
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

    BLOP_ASSERT_BONDS_VOID(idx + count, vec->count + 1)

    memcpy(&vec->data[idx], src, count * sizeof(BLOP_VECTOR_DATA_TYPE));
}
void                    blopfn_vector_memset(blopt_vector vec, size_t idx, BLOP_VECTOR_DATA_TYPE value, size_t count) {
    BLOP_ASSERT_PTR_VOID(vec);

    BLOP_ASSERT_BONDS_VOID(idx + count, vec->count + 1)

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

    BLOP_ASSERT(vec->count > 0, (BLOP_VECTOR_DATA_TYPE)0, "The vector is empty");

    return vec->data[vec->count - 1];
}
BLOP_VECTOR_DATA_TYPE   blopfn_vector_get_front(blopt_vector vec) {
    BLOP_ASSERT_PTR(vec, (BLOP_VECTOR_DATA_TYPE)0);

    BLOP_ASSERT(vec->count > 0, (BLOP_VECTOR_DATA_TYPE)0, "The vector is empty");

    return vec->data[0];
}

#endif /* BLOP_VECTOR_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

/* Cleanup generated tokens to avoid leaking macros. */

#undef BLOP_VECTOR_NAME
#undef BLOP_VECTOR_DATA_TYPE
#undef BLOP_VECTOR_SCALATOR
#undef BLOP_VECTOR_INITIAL_COUNT
#undef BLOP_VECTOR_DEALLOCATE_DATA
#undef BLOP_VECTOR_IMPLEMENTATION

#undef blopt_vector
#undef blops_vector   

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