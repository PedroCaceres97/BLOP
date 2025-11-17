/**
 * @file blop.h
 * @brief Core utility macros used across the BLOP project.
 *
 * This header defines lightweight helpers for error reporting, assertions,
 * abort/exit handling, pointer validation, memory allocation, and miscellaneous
 * arithmetic utilities. All modules in BLOP rely on these common primitives.
 *
 * @author Pedro Caceres
 * @date November 2025
 */

#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/**
 * @defgroup blop_global BLOP Global
 */

/**
 * @defgroup blop_configuration Configuration
 * @ingroup blop_global
 * @brief Optional compile-time macro flags that modify BLOP's behavior.
 * @{
 */

/**
 * @def BLOP_SAFE_MODE
 * @brief Enables safe mode validation
 *
 * Enables:
 * - ::BLOP_ASSERT_PTR
 * - ::BLOP_ASSERT_PTR_VOID
 */

/**
 * @def BLOP_ABORT_ON_ERROR
 * @brief When defined ::BLOP_ABORT calls `abort()` instead of `exit(-1)`
 */

/**
 * @def BLOP_VALID_EMPTY_POPPING
 * @brief Enables the feature of popping from empty containers.
 * 
 * When defined ::BLOP_EMPTY_POPPING becomes no-op
 *  
 * By default when calling pop in any form in any container
 * produces an error, defining this macro avoid this error.
 * The functions are coded so nothing happens on popping from any container.
 */

/**
 * @def BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING
 * @brief ONLY USE IF YOU KNOW WHAT YOU ARE DOING, Disables all kind of BLOP_ASSERT_* to improve every inch of performance
 * 
 * When defined prevents blop from asserting almost everything
 * More properly explained it turns all of ::BLOP_ASSERT macro family into no-op
 * 
 * There are very few functions that make asserts even when
 * this is defined, specifically those functions who allocate memory.
 */

/** @} */ /* end of blop_configuration */

/**
 * @defgroup blop_reporting Error Reporting
 * @ingroup blop_global
 * @brief Macros for printing diagnostic messages.
 * @{
 */

/** 
 * @name Error Reporting
 * @brief Macros for printing diagnostic messages.
 */

/**
 * @def BLOP_ABORT()
 * @brief Abort or exit depending on compile-time configuration.
 *
 * When any kind of error is detected ::BLOP_ABORT is called to exit program.
 * By default it uses `exit(-1)` to allow the use of `atexit()` function.
 * If ::BLOP_ABORT_ON_EXIT is defined the it will use `abort()` to core dump.
 *
 * ::BLOP_ABORT is called almost entirily by ::BLOP_ASSERT and family of macros
 */

/**
 * @def BLOP_ERROR_MESSAGE(msg)
 * @brief Print a generic error message including file and function context.
 *
 * Example output:
 * ```
 * [BLOP -> file.c{my_function} ]: Something went wrong
 * ```
 *
 * @param msg Null-terminated message string to print.
 */

/**
 * @def BLOP_OUT_OF_BONDS_MESSAGE(idx, bond)
 * @brief Print an "index out of bounds" diagnostic message.
 *
 * The message includes the attempted index and the valid bound.
 *
 * @param idx  The index that was attempted.
 * @param bond The upper bound (array/list size).
 */

/** @} */ /* end of blop_reporting (name) */

/** @} */ /* end of blop_reporting */

/**
 * @defgroup blop_assertions Assertions
 * @ingroup blop_global
 * @brief Runtime parameter and bounds validation helpers.
 * @{
 */

/**
 * @name Assertions
 * @brief Macros for runtime parameter and bounds validation.
 */

/**
 * @def BLOP_EMPTY_POPPING()
 * @brief Behavior when attempting to pop from an empty container.
 *
 * By default, popping from an empty container produces:
 * - Call ::BLOP_ERROR_MESSAGE.
 * - Call ::BLOP_ABORT.
 *
 * Disabled when ::BLOP_VALID_EMPTY_POPPING is defined, this mean that you can
 * pop from an empty container and the program wont crash.
 */

/**
 * @def BLOP_ASSERT(cnd, rtn, msg)
 * @brief Assert a condition and return a value on failure.
 *
 * If the assertion fails:
 * - Prints the provided message via ::BLOP_ERROR_MESSAGE.
 * - Calls ::BLOP_ABORT (unless disabled).
 * - Returns `rtn`.
 *
 * Disabled when `BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING is defined`.
 *
 * @param cnd Condition to validate.
 * @param rtn Return value when the condition fails.
 * @param msg Error message printed on failure.
 */

/**
 * @def BLOP_ASSERT_VOID(cnd, msg)
 * @brief Void-returning version of ::BLOP_ASSERT.
 *
 * @param cnd Condition to validate.
 * @param msg Error message printed on failure.
 */

/**
 * @def BLOP_ASSERT_BONDS(idx, bond, rtn)
 * @brief Ensure that an index is within bounds (idx >= bond).
 *
 * If the assertion fails:
 * - Prints the provided message via ::BLOP_ERROR_MESSAGE.
 * - Calls ::BLOP_ABORT (unless disabled).
 * - Returns `rtn`.
 *
 * Disabled when ::BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING is defined. 
 *
 * @param idx  Index being accessed.
 * @param bond Exclusive upper bound.
 * @param rtn  Return value when out of bounds.
 */

/**
 * @def BLOP_ASSERT_BONDS_VOID(idx, bond)
 * @brief Void-returning version of ::BLOP_ASSERT_BONDS.
 *
 * @param idx  Index being accessed.
 * @param bond Exclusive upper bound.
 */
 
/**
 * @def BLOP_ASSERT_PTR(ptr, rtn)
 * @brief Assert that a pointer parameter is not NULL.
 *
 * Enabled only when ::BLOP_SAFE_MODE is defined.
 * Disabled when ::BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING is defined. 
 *
 * @param ptr Pointer to validate.
 * @param rtn Return value when the pointer is NULL.
 */

/**
 * @def BLOP_ASSERT_PTR_VOID(ptr)
 * @brief Void-returning version of ::BLOP_ASSERT_PTR.
 *
 * @param ptr Pointer to validate.
 */

/** @} */ /* end of blop_assertions (name) */

/** @} */ /* end of blop_assertions */

/**
 * @defgroup blop_util Utilities
 * @ingroup blop_global
 * @brief Token concatenation, memory helpers, casting helpers and arithmetic utilities.
 * @{
 */

/**
 * @name Unique Symbol Generation
 * @brief Macros for generating unique symbol names.
 * @{
 */

/**
 * @def BLOP_CONCAT2_IMPL(a, b)
 * @brief Concatenate two tokens (raw implementation).
 *
 * @param a First token.
 * @param b Second token.
 */

/**
 * @def BLOP_CONCAT3_IMPL(a, b, c)
 * @brief Concatenate three tokens (raw implementation).
 *
 * @param a First token.
 * @param b Second token.
 * @param c Third token.
 */

/**
 * @def BLOP_CONCAT2(a, b)
 * @brief Public interface for two token concatenation).
*
 * @param a First token.
 * @param b Second token.
 */

/**
 * @def BLOP_CONCAT3(a, b, c)
 * @brief Public interface for three token concatenation.
 *
 * @param a First token.
 * @param b Second token.
 * @param c Third token.
 */

/**
 * @} */ /* end of unique symbol generation */

/**
 * @name Memory Helpers
 * @brief Macros for malloc, calloc, realloc, and free.
 * @{
 */

/**
 * @def BLOP_FREE(ptr)
 * @brief Free a previously allocated pointer.
 * 
 * Intended to use in integration with ::BLOP_ALLOC, ::BLOP_MALLOC, ::BLOP_CALLOC, and ::BLOP_REALLOC.
 *
 * @param ptr Pointer to free.
 */

/**
 * @def BLOP_ALLOC(cast, bytes)
 * @brief Allocate raw memory and cast the result.
 * 
 * This would be equivalent to `(cast*)malloc(bytes)`. 
 * Intended to use in integration with ::BLOP_FREE, ::BLOP_MALLOC, ::BLOP_CALLOC, and ::BLOP_REALLOC.
 *
 * @param cast  Pointer type (e.g., `int`).
 * @param bytes Number of bytes to allocate.
 * @return Pointer of type `(cast*)`.
 */

/**
 * @def BLOP_MALLOC(type, count)
 * @brief Allocate an array of a given type.
 * 
 * This would be equivalent to a normal calloc() call but using malloc().
 * Intended to use in integration with ::BLOP_FREE, ::BLOP_ALLOC, ::BLOP_CALLOC, and ::BLOP_REALLOC.
 *
 * @param type  Type of each element.
 * @param count Number of elements.
 * @return Allocated pointer.
 */

/**
 * @def BLOP_CALLOC(type, count)
 * @brief Allocate and zero-initialize an array.
 * 
 * Intended to use in integration with ::BLOP_FREE, ::BLOP_ALLOC, ::BLOP_MALLOC, and ::BLOP_REALLOC.
 *
 * @param type  Element type.
 * @param count Number of elements.
 */

/**
 * @def BLOP_REALLOC(cast, ptr, size)
 * @brief Reallocate memory and cast the returned pointer.
 * 
 * Intended to use in integration with ::BLOP_FREE, ::BLOP_ALLOC, ::BLOP_MALLOC, and ::BLOP_CALLOC.
 *
 * @param cast Pointer type.
 * @param ptr  Pointer to reallocate.
 * @param size New size in bytes.
 */

/** @} */ /* end of memory helpers */

/**
 * @name C Sintax Simplifiers
 * @brief Macros that provides functionality of native C sintaxis.
 * @{
 */

/**
 * @def BLOP_CAST(type, tocast)
 * @brief Cast a value to another type.
 *
 * @param type   Target type.
 * @param tocast Expression or value to cast.
 * @return Value cast to `type`, (type)tocast.
 */

/**
 * @def BLOP_TERNARY(cnd, x, y)
 * @brief Ternary operator helper macro.
 * @param cnd Condition to evaluate.
 * @param x   Value if condition is true.
 * @param y   Value if condition is false.
 * @return `x` if `cnd` is true, else `y`.
 */

/** @} */ /* end of c sintax simplifiers */

/** 
 * @name Arithmetic Utilities
 * @brief Macros for common arithmetic operations.
 * @{
 */

/**
 * @def BLOP_MIN(x, y)
 * @brief Compute the minimum two values.
 *
 * @param x First value.
 * @param y Second value.
 * @return The smaller of x and y.
 */

/**
 * @def BLOP_MAX(x, y)
 * @brief Compute the maximum two values.
 *
 * @param x First value.
 * @param y Second value.
 * @return The larger of x and y.
 */

/**
 * @def BLOP_DISTANCE(x, y)
 * @brief Compute absolute difference between two values.
 *
 * @param x First value.
 * @param y Second value.
 * @return Absolute difference |x - y|.
 */

/**
 * @def BLOP_DIFFERENCE(x, y)
 * @brief Compute signed difference between two values.
 *
 * @param x First value.
 * @param y Second value.
 * @return Signed difference (x - y).
 */

/** @} */ /* end of arithmetic utilities */

/**
 * @name Pointer Arithmetic
 * @brief Macros for byte-accurate pointer arithmetic.
 * @{
 */

/**
 * @def BLOP_PADD(ptr, value)
 * @brief Perform byte-accurate pointer arithmetic.
 *
 * These macros convert the pointer to `uint8_t*` internally.
 *
 * @param ptr   Base pointer.
 * @param value Byte offset forwards.
 * @return Adjusted pointer as `void*`.
 */

/**
 * @def BLOP_PSUB(ptr, value)
 * @brief Perform byte-accurate pointer arithmetic.
 *
 * These macros convert the pointer to `uint8_t*` internally.
 *
 * @param ptr   Base pointer.
 * @param value Byte offset backwards.
 * @return Adjusted pointer as `void*`.
 */

/** @} */ /* end of pointer arithmetic */

/** @} */ /* end of blop_util */

#define BLOP_ERROR_MESSAGE(msg)                 printf("[BLOP -> %s{%s} ]: %s\n", __FILE__, __FUNCTION__, msg)
#define BLOP_OUT_OF_BONDS_MESSAGE(idx, bond)    printf("[BLOP -> %s{%s} idx = %zu while bond = %zu]: Index out of bonds\n", __FILE__, __FUNCTION__, idx, bond);

#ifdef BLOP_ABORT_ON_ERROR
    #define BLOP_ABORT() abort()
#else
    #define BLOP_ABORT() exit(-1)
#endif /* BLOP_ABORT_ON_ERROR */

#ifndef BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING

    #ifndef BLOP_VALID_EMPTY_POPPING
        #define BLOP_EMPTY_POPPING() BLOP_ERROR_MESSAGE("To enable empty popping and avoid this error define BLOP_VALID_EMPTY_POPPING"); BLOP_ABORT();
    #else
        #define BLOP_EMPTY_POPPING(msg) ((void)0)
    #endif /* BLOP_VALID_EMPTY_POPPING */

    #define BLOP_ASSERT(cnd, rtn, msg) if (!(cnd)) { BLOP_ERROR_MESSAGE(msg); BLOP_ABORT(); return rtn; }
    #define BLOP_ASSERT_VOID(cnd, msg) if (!(cnd)) { BLOP_ERROR_MESSAGE(msg); BLOP_ABORT(); return; }

    #define BLOP_ASSERT_BONDS(idx, bond, rtn) if ((idx) >= (bond)) { BLOP_OUT_OF_BONDS_MESSAGE(idx, bond); BLOP_ABORT(); return rtn; }
    #define BLOP_ASSERT_BONDS_VOID(idx, bond) if ((idx) >= (bond)) { BLOP_OUT_OF_BONDS_MESSAGE(idx, bond); BLOP_ABORT(); return; }

    #ifdef BLOP_SAFE_MODE

        #define BLOP_ASSERT_PTR(ptr, rtn) if ((ptr) == NULL) { BLOP_ERROR_MESSAGE(#ptr " parameter is NULL"); BLOP_ABORT(); return rtn; }
        #define BLOP_ASSERT_PTR_VOID(ptr) if ((ptr) == NULL) { BLOP_ERROR_MESSAGE(#ptr " parameter is NULL"); BLOP_ABORT(); return; }

    #else

        #define BLOP_ASSERT_PTR(ptr, rtn)  ((void)0)
        #define BLOP_ASSERT_PTR_VOID(ptr)  ((void)0)

    #endif /* BLOP_SAFE_MODE */

#else

    #define BLOP_EMPTY_POPPING(msg) ((void)0)
    #define BLOP_ASSERT(cnd, rtn, msg)          ((void)0)
    #define BLOP_ASSERT_VOID(cnd, msg)          ((void)0)
    #define BLOP_ASSERT_BONDS(idx, bond, rtn)   ((void)0)
    #define BLOP_ASSERT_BONDS_VOID(idx, bond)   ((void)0)
    #define BLOP_ASSERT_PTR(ptr, rtn)           ((void)0)
    #define BLOP_ASSERT_PTR_VOID(ptr)           ((void)0)

#endif /* BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING */

#define BLOP_CONCAT2_IMPL(a, b)     a##b
#define BLOP_CONCAT3_IMPL(a, b, c)  a##b##c

#define BLOP_CONCAT2(a, b)          BLOP_CONCAT2_IMPL(a, b)
#define BLOP_CONCAT3(a, b, c)       BLOP_CONCAT3_IMPL(a, b, c)

#define BLOP_FREE(ptr)                         free ((void*)(ptr))
#define BLOP_ALLOC(cast, bytes)         (cast*)malloc(bytes)
#define BLOP_MALLOC(type, count)        (type*)malloc(sizeof(type) * (count))
#define BLOP_CALLOC(type, count)        (type*)calloc(count, sizeof(type))
#define BLOP_REALLOC(cast, ptr, size)   (cast*)realloc((void*)(ptr), (size))

#define BLOP_CAST(type, tocast) ((type)(tocast))
#define BLOP_TERNARY(cnd, x, y) ((cnd) ? (x) : (y))

#define BLOP_MIN(x, y) BLOP_TERNARY((x > y), y, x)
#define BLOP_MAX(x, y) BLOP_TERNARY((x > y), x, y)
#define BLOP_DISTANCE(x, y) (BLOP_TERNARY(x > y, x - y, y - x))
#define BLOP_DIFFERENCE(x, y) ((ptrdiff_t)(a) - (ptrdiff_t)(b))

#define BLOP_PADD(ptr, value) ((void*)((uint8_t*)(ptr) + (value)))
#define BLOP_PSUB(ptr, value) ((void*)((uint8_t*)(ptr) - (value)))

#ifdef _DOXYGEN_

#define BLOP_SAFE_MODE
#define BLOP_ABORT_ON_ERROR
#define BLOP_VALID_EMPTY_POPPING
#define BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING

#undef  BLOP_SAFE_MODE
#undef  BLOP_ABORT_ON_ERROR
#undef  BLOP_VALID_EMPTY_POPPING
#undef  BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING

#endif

#endif /* __BLOP_H__ */
