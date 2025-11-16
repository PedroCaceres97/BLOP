/**
 * @file blop.h
 * @brief Core utility macros used across the BLOP project.
 *
 * This header defines lightweight helpers for error reporting, assertions,
 * abort/exit handling, pointer validation, memory allocation, and miscellaneous
 * arithmetic utilities. All modules in BLOP rely on these common primitives.
 *
 * ## Configuration Macros
 * - Define `BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING`
 *   to disable abort/exit on fatal errors.
 * - Define `BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING`
 *   to completely disable all BLOP assertions.
 * - Define `BLOP_SAFE_MODE` to enable NULL pointer checks.
 * - Define `BLOP_ABORT_ON_ERROR` to use abort() instead of exit(-1) on assertion.
 *
 * These flags allow each user to fine-tune BLOP's safety vs performance tradeoffs.
 *
 * @author Pedro Caceres
 * @date 2025-11-16
 */

#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/**
 * @defgroup blop_config BLOP Global Configuration Flags
 * @brief Optional compile-time flags that modify BLoP's behavior.
 *
 * These flags are **not defined by default**.  
 * Users may enable them through compiler options (e.g., `-DNAME`).
 *
 * @{
 */

/**
 * @def BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING
 * @ingroup blop_config
 * @brief Disable crash-on-error behavior.
 *
 * When defined, BLoP avoids aborting the program on fatal internal errors.  
 * Use only if you completely understand the consequences.
 */


/**
 * @def BLOP_I_DONT_WANT_TO_ASSERT_ANYTHING_I_KNOW_WHAT_I_AM_DOING
 * @ingroup blop_config
 * @brief Disable all internal assertions.
 *
 * Removes all `assert()`-based safety checks.  
 * Intended only for high-performance builds where safety tradeoffs are acceptable.
 */

/**
 * @def BLOP_VALID_EMPTY_POPPING
 * @ingroup blop_config
 * @brief Allow popping from empty containers.
 *
 * When enabled, popping an empty BLoP container returns a default value  
 * instead of treating it as an error.
 */

/**
 * @def BLOP_SAFE_MODE
 * @ingroup blop_config
 * @brief Enables full safety and validation mode.
 *
 * Turns on extra runtime checks and parameter validation.  
 * Greatly improves safety at a small performance cost.
 */

/**
 * @def BLOP_ABORT_ON_ERROR
 * @ingroup blop_config
 * @brief Makes it so BLOP_ABORT() uses abort() instead of exit(-1).
 *
 * When defined, BLOP_ABORT() will call abort() to generate a core dump on fatal errors.  
 * Otherwise, it will call exit(-1).
 */

/** @} */ /* end of blop_config */

/**
 * @defgroup blop_logging Logging & Error Reporting
 * @brief Macros for printing diagnostic messages.
 * @{
 */

/**
 * @brief Print a generic error message including file and function context.
 *
 * Example output:
 * ```
 * [BLOP -> file.c{my_function} ]: Something went wrong
 * ```
 *
 * @param msg Null-terminated message string to print.
 */
#define BLOP_ERROR_MESSAGE(msg)                 printf("[BLOP -> %s{%s} ]: %s\n", __FILE__, __FUNCTION__, msg)

/**
 * @brief Print an "index out of bounds" diagnostic message.
 *
 * The message includes the attempted index and the valid bound.
 *
 * @param idx  The index that was attempted.
 * @param bond The upper bound (array/list size).
 */
#define BLOP_OUT_OF_BONDS_MESSAGE(idx, bond)    printf("[BLOP -> %s{%s} idx = %zu while bond = %zu]: Index out of bonds\n", __FILE__, __FUNCTION__, idx, bond);

/** @} */ /* end of blop_logging */


/**
 * @defgroup blop_abort Abort / Exit Helpers
 * @brief Fatal error behavior configuration.
 * @{
 */


#ifndef BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING

    #ifdef BLOP_ABORT_ON_ERROR

        /**
         * @brief Abort or exit depending on compile-time configuration.
         *
         * Behavior:
         * - If `BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING` is defined:
         *   - `BLOP_ABORT()` becomes a no-op.
         * - Otherwise:
         *   - If `BLOP_ABORT_ON_ERROR` is defined → calls `abort()`.
         *   - Else → calls `exit(-1)`.
         */
        #define BLOP_ABORT() abort()
    
    #else

        /**
         * @brief Abort or exit depending on compile-time configuration.
         *
         * Behavior:
         * - If `BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING` is defined:
         *   - `BLOP_ABORT()` becomes a no-op.
         * - Otherwise:
         *   - If `BLOP_ABORT_ON_ERROR` is defined → calls `abort()`.
         *   - Else → calls `exit(-1)`.
         */
        #define BLOP_ABORT() exit(-1)

    #endif

#else

    /**
     * @brief Abort or exit depending on compile-time configuration.
     *
     * Behavior:
     * - If `BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING` is defined:
     *   - `BLOP_ABORT()` becomes a no-op.
     * - Otherwise:
     *   - If `BLOP_ABORT_ON_ERROR` is defined → calls `abort()`.
     *   - Else → calls `exit(-1)`.
     */
    #define BLOP_ABORT() ((void)0)

#endif

/** @} */ /* end of blop_abort */


/**
 * @defgroup blop_assert Assertions and Bounds Checking
 * @brief Runtime parameter and bounds validation helpers.
 * @{
 */

#ifndef BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING

    /**
     * @brief Assert a condition and return a value on failure.
     *
     * If the assertion fails:
     * - Prints the provided message via ::BLOP_ERROR_MESSAGE.
     * - Calls ::BLOP_ABORT (unless disabled).
     * - Returns `rtn`.
     *
     * Disabled when `BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING` is defined.
     *
     * @param cnd Condition to validate.
     * @param rtn Return value when the condition fails.
     * @param msg Error message printed on failure.
     */
    #define BLOP_ASSERT(cnd, rtn, msg) \
        if (!(cnd)) { BLOP_ERROR_MESSAGE(msg); BLOP_ABORT(); return rtn; }

    /**
     * @brief Assert a condition and return from a `void` function on failure.
     *
     * @param cnd Condition to validate.
     * @param msg Error message printed on failure.
     */
    #define BLOP_ASSERT_VOID(cnd, msg) \
        if (!(cnd)) { BLOP_ERROR_MESSAGE(msg); BLOP_ABORT(); return; }

    /**
     * @brief Ensure that an index is within bounds.
     *
     * Fails when `idx >= bond`.
     *
     * @param idx  Index being accessed.
     * @param bond Exclusive upper bound.
     * @param rtn  Return value when out of bounds.
     */
    #define BLOP_ASSERT_BONDS(idx, bond, rtn) \
        if ((idx) >= (bond)) { BLOP_OUT_OF_BONDS_MESSAGE(idx, bond); BLOP_ABORT(); return rtn; }

    /**
     * @brief Void-returning version of ::BLOP_ASSERT_BONDS.
     *
     * @param idx  Index being accessed.
     * @param bond Exclusive upper bound.
     */
    #define BLOP_ASSERT_BONDS_VOID(idx, bond) \
        if ((idx) >= (bond)) { BLOP_OUT_OF_BONDS_MESSAGE(idx, bond); BLOP_ABORT(); return; }

    #ifdef BLOP_SAFE_MODE

        /**
         * @brief Assert that a pointer parameter is not NULL.
         *
         * Enabled only when `BLOP_SAFE_MODE` is defined.
         *
         * @param ptr Pointer to validate.
         * @param rtn Return value when the pointer is NULL.
         */
        #define BLOP_ASSERT_PTR(ptr, rtn) \
            if ((ptr) == NULL) { BLOP_ERROR_MESSAGE(#ptr " parameter is NULL"); BLOP_ABORT(); return rtn; }

        /**
         * @brief Void-returning version of ::BLOP_ASSERT_PTR.
         *
         * @param ptr Pointer to validate.
         */
        #define BLOP_ASSERT_PTR_VOID(ptr) \
            if ((ptr) == NULL) { BLOP_ERROR_MESSAGE(#ptr " parameter is NULL"); BLOP_ABORT(); return; }

    #else
        /**
         * @brief Assert that a pointer parameter is not NULL.
         *
         * Enabled only when `BLOP_SAFE_MODE` is defined.
         *
         * @param ptr Pointer to validate.
         * @param rtn Return value when the pointer is NULL.
         */
        #define BLOP_ASSERT_PTR(ptr, rtn)  ((void)0)

        /**
         * @brief Void-returning version of ::BLOP_ASSERT_PTR.
         *
         * @param ptr Pointer to validate.
         */
        #define BLOP_ASSERT_PTR_VOID(ptr)  ((void)0)
    #endif

#else /* assertions disabled entirely */

    /**
     * @brief Assert a condition and return a value on failure.
     *
     * If the assertion fails:
     * - Prints the provided message via ::BLOP_ERROR_MESSAGE.
     * - Calls ::BLOP_ABORT (unless disabled).
     * - Returns `rtn`.
     *
     * Disabled when `BLOP_I_DONT_WANT_TO_CRASH_ON_ERROR_I_KNOW_WHAT_I_AM_DOING` is defined.
     *
     * @param cnd Condition to validate.
     * @param rtn Return value when the condition fails.
     * @param msg Error message printed on failure.
     */
    #define BLOP_ASSERT(cnd, rtn, msg)          ((void)0)

    /**
     * @brief Void-returning version of ::BLOP_ASSERT.
     *
     * @param cnd Condition to validate.
     * @param msg Error message printed on failure.
     */
    #define BLOP_ASSERT_VOID(cnd, msg)          ((void)0)

    /**
     * @brief Ensure that an index is within bounds.
     *
     * Fails when `idx >= bond`.
     *
     * @param idx  Index being accessed.
     * @param bond Exclusive upper bound.
     * @param rtn  Return value when out of bounds.
     */
    #define BLOP_ASSERT_BONDS(idx, bond, rtn)   ((void)0)

    /**
     * @brief Void-returning version of ::BLOP_ASSERT_BONDS.
     *
     * @param idx  Index being accessed.
     * @param bond Exclusive upper bound.
     */
    #define BLOP_ASSERT_BONDS_VOID(idx, bond)   ((void)0)

    /**
     * @brief Assert that a pointer parameter is not NULL.
     *
     * Enabled only when `BLOP_SAFE_MODE` is defined.
     *
     * @param ptr Pointer to validate.
     * @param rtn Return value when the pointer is NULL.
     */
    #define BLOP_ASSERT_PTR(ptr, rtn)           ((void)0)

    /**
     * @brief Void-returning version of ::BLOP_ASSERT_PTR.
     *
     * @param ptr Pointer to validate.
     */
    #define BLOP_ASSERT_PTR_VOID(ptr)           ((void)0)

#endif

/** @} */ /* end of blop_assert */


/**
 * @defgroup blop_popping Empty-Container Popping Behaviour
 * @brief Error handling when popping from empty containers.
 * @{
 */

/**
 * @brief Behavior when attempting to pop from an empty container.
 *
 * If `BLOP_VALID_EMPTY_POPPING` is **not** defined:
 * - Prints the provided message.
 * - Aborts or exits depending on ::BLOP_ABORT.
 *
 * Otherwise:
 * - Becomes a no-op.
 *
 * @param msg Message printed when popping from an empty container.
 */
#ifndef BLOP_VALID_EMPTY_POPPING
    #define BLOP_EMPTY_POPPING(msg) \
        BLOP_ERROR_MESSAGE(msg); BLOP_ABORT();
#else
    #define BLOP_EMPTY_POPPING(msg) ((void)0)
#endif

/** @} */ /* end of blop_popping */


/**
 * @defgroup blop_util Miscellaneous Utilities
 * @brief Token concatenation, memory helpers, casting helpers and arithmetic utilities.
 * @{
 */

/**
 * @brief Concatenate two tokens (raw implementation).
 *
 * @param a First token.
 * @param b Second token.
 */
#define BLOP_CONCAT2_IMPL(a, b) a##b

/**
 * @brief Concatenate three tokens (raw implementation).
 *
 * @param a First token.
 * @param b Second token.
 * @param c Third token.
 */
#define BLOP_CONCAT3_IMPL(a, b, c) a##b##c

/**
 * @brief Public interface for two token concatenation).
 */
#define BLOP_CONCAT2(a, b)         BLOP_CONCAT2_IMPL(a, b)

/**
 * @brief Public interface for three token concatenation.
 */
#define BLOP_CONCAT3(a, b, c)      BLOP_CONCAT3_IMPL(a, b, c)

/**
 * @name Memory Helpers
 * @brief Macros for malloc, calloc, realloc, and free.
 * @{
 */

/**
 * @brief Free a previously allocated pointer.
 *
 * @param ptr Pointer to free.
 */
#define BLOP_FREE(ptr) free((void*)(ptr))

/**
 * @brief Allocate raw memory and cast the result.
 *
 * @param cast  Pointer type (e.g., `int`).
 * @param bytes Number of bytes to allocate.
 * @return Pointer of type `(cast*)`.
 */
#define BLOP_ALLOC(cast, bytes) (cast*)malloc(bytes)

/**
 * @brief Allocate an array of a given type.
 *
 * @param type  Type of each element.
 * @param count Number of elements.
 * @return Allocated pointer.
 */
#define BLOP_MALLOC(type, count) (type*)malloc(sizeof(type) * (count))

/**
 * @brief Allocate and zero-initialize an array.
 *
 * @param type  Element type.
 * @param count Number of elements.
 */
#define BLOP_CALLOC(type, count) (type*)calloc(count, sizeof(type))

/**
 * @brief Reallocate memory and cast the returned pointer.
 *
 * @param cast Pointer type.
 * @param ptr  Pointer to reallocate.
 * @param size New size in bytes.
 */
#define BLOP_REALLOC(cast, ptr, size) (cast*)realloc((void*)(ptr), (size))

/** @} */ /* end memory helpers */

/**
 * @brief Cast a value to another type.
 *
 * @param type   Target type.
 * @param tocast Expression or value to cast.
 */
#define BLOP_CAST(type, tocast) ((type)(tocast))

/**
 * @brief Ternary operator helper macro.
 */
#define BLOP_TERNARY(cnd, x, y) ((cnd) ? (x) : (y))

/**
 * @brief Compute the minimum two values.
 *
 * @warning Arguments may be evaluated multiple times.
 */
#define BLOP_MIN(x, y) BLOP_TERNARY((x > y), y, x)

/**
 * @brief Compute the maximum two values.
 *
 * @warning Arguments may be evaluated multiple times.
 */
#define BLOP_MAX(x, y) BLOP_TERNARY((x > y), x, y)

/**
 * @brief Compute absolute difference between two values.
 */
#define BLOP_DISTANCE(x, y) (BLOP_TERNARY(x > y, x - y, y - x))

/**
 * @brief Compute signed difference between two values.
 */
#define BLOP_DIFFERENCE(x, y) (BLOP_TERNARY(x > y, x - y, 0 - (y - x)))

/**
 * @brief Perform byte-accurate pointer arithmetic.
 *
 * These macros convert the pointer to `uint8_t*` internally.
 *
 * @param ptr   Base pointer.
 * @param value Byte offset forwards.
 * @return Adjusted pointer as `void*`.
 */
#define BLOP_PADD(ptr, value) ((void*)((uint8_t*)(ptr) + (value)))

/**
 * @brief Perform byte-accurate pointer arithmetic.
 *
 * These macros convert the pointer to `uint8_t*` internally.
 *
 * @param ptr   Base pointer.
 * @param value Byte offset backwards.
 * @return Adjusted pointer as `void*`.
 */
#define BLOP_PSUB(ptr, value) ((void*)((uint8_t*)(ptr) - (value)))

/** @} */ /* end blop_util */

#endif /* __BLOP_H__ */
