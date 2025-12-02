#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
  #define BLOP_WINDOWS
#endif

#define BLOP_LOG_ERROR(msg)               fprintf(stderr, "[BLOP -> %s{%s} ]: %s\n", __FILE__, __FUNCTION__, msg)
#define BLOP_LOG_OUT_OF_BOUNDS(idx, bond) fprintf(stderr, "[BLOP -> %s{%s} ]: Index {%zu} out of bounds {%zu}\n", __FILE__, __FUNCTION__, idx, bond);

#ifdef BLOP_ABORT_ON_ERROR
  #define BLOP_ERROR() abort()
#else
  #define BLOP_ERROR() exit(-1)
#endif /* BLOP_ABORT_ON_ERROR */

#define BLOP_ASSERT(cnd, rtn, msg)          do { if (!(cnd)) { BLOP_LOG_ERROR(msg); BLOP_ERROR(); return rtn; } } while(0)
#define BLOP_ASSERT_VOID(cnd, msg)          do { if (!(cnd)) { BLOP_LOG_ERROR(msg); BLOP_ERROR(); return; } } while(0)

#define BLOP_ASSERT_BOUNDS(idx, bond, rtn)  do { if ((idx) >= (bond)) { BLOP_LOG_OUT_OF_BOUNDS(idx, bond); BLOP_ERROR(); return rtn; } } while(0)
#define BLOP_ASSERT_BOUNDS_VOID(idx, bond)  do { if ((idx) >= (bond)) { BLOP_LOG_OUT_OF_BOUNDS(idx, bond); BLOP_ERROR(); return; } } while(0)

#define BLOP_ASSERT_FORCED(cnd, rtn, msg)   do { if (!(cnd)) { BLOP_LOG_ERROR(msg); BLOP_ERROR(); return rtn; } } while(0)
#define BLOP_ASSERT_FORCED_VOID(cnd, msg)   do { if (!(cnd)) { BLOP_LOG_ERROR(msg); BLOP_ERROR(); return; } } while(0)

#ifdef BLOP_ENABLE_SAFETY
  #define BLOP_ASSERT_PTR(ptr, rtn)         do { if ((ptr) == NULL) { BLOP_LOG_ERROR(#ptr " parameter is NULL"); BLOP_ERROR(); return rtn; } } while(0)
  #define BLOP_ASSERT_PTR_VOID(ptr)         do { if ((ptr) == NULL) { BLOP_LOG_ERROR(#ptr " parameter is NULL"); BLOP_ERROR(); return; } } while(0)
#else
  #define BLOP_ASSERT_PTR(ptr, rtn) ((void)0)
  #define BLOP_ASSERT_PTR_VOID(ptr) ((void)0)
#endif /* BLOP_ENABLE_SAFETY */

#ifdef BLOP_ENABLE_THREADSAFE

  #ifdef BLOP_WINDOWS

    #include <windows.h>
    #define BLOP_RWLOCK_TYPE            SRWLOCK
    #define BLOP_RWLOCK_INIT(lock)      InitializeSRWLock(&(lock))
    #define BLOP_RWLOCK_RDLOCK(lock)    AcquireSRWLockShared(&(lock))
    #define BLOP_RWLOCK_WRLOCK(lock)    AcquireSRWLockExclusive(&(lock))
    #define BLOP_RWLOCK_RDUNLOCK(lock)  ReleaseSRWLockShared(&(lock))
    #define BLOP_RWLOCK_WRUNLOCK(lock)  ReleaseSRWLockExclusive(&(lock))
    #define BLOP_RWLOCK_DESTROY(lock)   ((void)0) /* SRWLOCK does not require explicit destroy */

  #else

    #include <pthread.h>
    #define BLOP_RWLOCK_TYPE            pthread_rwlock_t
    #define BLOP_RWLOCK_INIT(lock)      pthread_rwlock_init(&(lock), NULL)
    #define BLOP_RWLOCK_RDLOCK(lock)    pthread_rwlock_rdlock(&(lock))
    #define BLOP_RWLOCK_WRLOCK(lock)    pthread_rwlock_wrlock(&(lock))
    #define BLOP_RWLOCK_RDUNLOCK(lock)  pthread_rwlock_unlock(&(lock))
    #define BLOP_RWLOCK_WRUNLOCK(lock)  pthread_rwlock_unlock(&(lock))
    #define BLOP_RWLOCK_DESTROY(lock)   pthread_rwlock_destroy(&(lock))

  #endif /* BLOP_WINDOWS */

#else

  #define BLOP_RWLOCK_TYPE            void*
  #define BLOP_RWLOCK_INIT(lock)      ((void)0)
  #define BLOP_RWLOCK_RDLOCK(lock)    ((void)0)
  #define BLOP_RWLOCK_WRLOCK(lock)    ((void)0)
  #define BLOP_RWLOCK_RDUNLOCK(lock)  ((void)0)
  #define BLOP_RWLOCK_WRUNLOCK(lock)  ((void)0)
  #define BLOP_RWLOCK_DESTROY(lock)   ((void)0)

#endif /* BLOP_ENABLE_THREADSAFE */

#ifdef BLOP_ENABLE_EMPTY_POPPING
  #define BLOP_EMPTY_POPPING() ((void)0)
#else
  #define BLOP_EMPTY_POPPING() do { BLOP_LOG_ERROR("To enable empty popping and avoid this error define BLOP_VALID_EMPTY_POPPING"); BLOP_ERROR(); } while(0)
#endif /* BLOP_ENABLE_EMPTY_POPPING */

#ifdef BLOP_DISABLE_ASSERTIONS

  #undef  BLOP_ENABLE_SAFETY

  #undef  BLOP_ASSERT(cnd, rtn, msg)          ((void)0)
  #undef  BLOP_ASSERT_VOID(cnd, msg)          ((void)0)
  #undef  BLOP_ASSERT_BOUNDS(idx, bond, rtn)  ((void)0)
  #undef  BLOP_ASSERT_BOUNDS_VOID(idx, bond)  ((void)0)
  #undef  BLOP_ASSERT_PTR(ptr, rtn)           ((void)0)
  #undef  BLOP_ASSERT_PTR_VOID(ptr)           ((void)0)

  #define BLOP_ASSERT(cnd, rtn, msg)          ((void)0)
  #define BLOP_ASSERT_VOID(cnd, msg)          ((void)0)
  #define BLOP_ASSERT_BOUNDS(idx, bond, rtn)  ((void)0)
  #define BLOP_ASSERT_BOUNDS_VOID(idx, bond)  ((void)0)
  #define BLOP_ASSERT_PTR(ptr, rtn)           ((void)0)
  #define BLOP_ASSERT_PTR_VOID(ptr)           ((void)0)

#endif /* BLOP_DISABLE_ASSERTIONS */

#define BLOP_CONCAT2_IMPL(a, b)     a##b
#define BLOP_CONCAT3_IMPL(a, b, c)  a##b##c

#define BLOP_CONCAT2(a, b)          BLOP_CONCAT2_IMPL(a, b)
#define BLOP_CONCAT3(a, b, c)       BLOP_CONCAT3_IMPL(a, b, c)

#define BLOP_FREE(ptr)                       free ((void*)(ptr))
#define BLOP_MALLOC(cast, bytes)      (cast*)malloc((bytes))
#define BLOP_CALLOC(type, count)      (type*)calloc((count), sizeof(type))
#define BLOP_REALLOC(cast, ptr, size) (cast*)realloc((void*)(ptr), (size))

#define BLOP_CAST(type, tocast) ((type)(tocast))
#define BLOP_TERNARY(cnd, x, y) ((cnd) ? (x) : (y))

#define BLOP_PTR_ADD(ptr, value) ((void*)((uint8_t*)(ptr) + (value)))
#define BLOP_PTR_SUB(ptr, value) ((void*)((uint8_t*)(ptr) - (value)))

#define BLOP_MIN(x, y) BLOP_TERNARY(x > y, y, x)
#define BLOP_MAX(x, y) BLOP_TERNARY(x > y, x, y)
#define BLOP_DISTANCE(x, y) (BLOP_TERNARY(x > y, x - y, y - x))
#define BLOP_DIFFERENCE(x, y) ((ptrdiff_t)(x) - (ptrdiff_t)(y))

#ifdef _DOXYGEN_
  #define BLOP_ABORT_ON_ERROR
  #define BLOP_ENABLE_SAFETY
  #define BLOP_ENABLE_EMPTY_POPPING
  #define BLOP_DISABLE_ASSERTIONS

  #define BLOP_ABORT_ON_ERROR
  #define BLOP_ENABLE_SAFETY
  #define BLOP_ENABLE_EMPTY_POPPING
  #define BLOP_DISABLE_ASSERTIONS
#endif

#endif /* __BLOP_H__ */
