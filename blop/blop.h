#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------------
 * SYSTEM & TOOLCHAIN
 * -------------------------------------------------------------------------- */

/* OS Family -------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64)
  #define OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
  #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE
    #define OS_IOS
  #else
    #define OS_MACOS
  #endif
#elif defined(__linux__)
  #define OS_LINUX
#elif defined(__unix__) || defined(__unix)
  #define OS_UNIX
#elif defined(__ANDROID__)
  #define OS_ANDROID
#else
  #define OS_UNKNOWN
#endif

#if defined(OS_LINUX) || defined(OS_MACOS) || defined(OS_UNIX) || defined(OS_ANDROID) || defined(OS_IOS)
  #define OS_POSIX
#endif
/* ------------------------------------------------------------------------ */

/* Compiler --------------------------------------------------------------- */
#if defined(__clang__)
  #define COMPILER_CLANG
#elif defined(__GNUC__)
  #define COMPILER_GCC
#elif defined(_MSC_VER)
  #define COMPILER_MSVC
#else
  #define COMPILER_UNKNOWN
#endif

/* CPU Architecture ------------------------------------------------------- */
#if defined(__x86_64__) || defined(_M_X64)
  #define CPU_X64
#elif defined(__i386__) || defined(_M_IX86)
  #define CPU_X86
#elif defined(__aarch64__) || defined(_M_ARM64)
  #define CPU_ARM64
#elif defined(__arm__) || defined(_M_ARM)
  #define CPU_ARM32
#else
  #define CPU_UNKNOWN
#endif

/* Pointer Size ----------------------------------------------------------- */
#if UINTPTR_MAX == 0xffffffffffffffffULL
  #define PTR_64BIT
#else
  #define PTR_32BIT
#endif

/* --------------------------------------------------------------------------
 * CROSSPLATFORM
 * -------------------------------------------------------------------------- */

#if defined(OS_WINDOWS) || defined(__CYGWIN__)
  #ifdef DLL_BUILD
    #define DLL_EXPORT __declspec(dllexport)
  #else
    #define DLL_EXPORT __declspec(dllimport)
  #endif
#else
  #define DLL_EXPORT
#endif

#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
  #define NORETURN __attribute__((noreturn))
#elif defined(COMPILER_MSVC)
  #define NORETURN __declspec(noreturn)
#else
  #define NORETURN _Noreturn
#endif

#if defined(__FILE_NAME__)
  #define FILE_PATH __FILE_NAME__
#else
  #define FILE_PATH __FILE__
#endif

/* --------------------------------------------------------------------------
 * RWLOCK
 * -------------------------------------------------------------------------- */

//! Enable rwlocks
#if defined(ENABLE_RWLOCK) && !defined(OS_UNKNOWN) 
  #ifdef OS_WINDOWS
    #include <windows.h>

    #define RWLOCK_TYPE            SRWLOCK
    #define RWLOCK_INIT(lock)      InitializeSRWLock(&(lock))
    #define RWLOCK_RDLOCK(lock)    AcquireSRWLockShared(&(lock))
    #define RWLOCK_WRLOCK(lock)    AcquireSRWLockExclusive(&(lock))
    #define RWLOCK_RDUNLOCK(lock)  ReleaseSRWLockShared(&(lock))
    #define RWLOCK_WRUNLOCK(lock)  ReleaseSRWLockExclusive(&(lock))
    #define RWLOCK_DESTROY(lock)   ((void)0) /* SRWLOCK does not require explicit destroy */
  #else
    #include <pthread.h>

    #define RWLOCK_TYPE            pthread_rwlock_t
    #define RWLOCK_INIT(lock)      pthread_rwlock_init(&(lock), NULL)
    #define RWLOCK_RDLOCK(lock)    pthread_rwlock_rdlock(&(lock))
    #define RWLOCK_WRLOCK(lock)    pthread_rwlock_wrlock(&(lock))
    #define RWLOCK_RDUNLOCK(lock)  pthread_rwlock_unlock(&(lock))
    #define RWLOCK_WRUNLOCK(lock)  pthread_rwlock_unlock(&(lock))
    #define RWLOCK_DESTROY(lock)   pthread_rwlock_destroy(&(lock))
  #endif /* OS_WINDOWS */
#else
  #define RWLOCK_TYPE            void*
  #define RWLOCK_INIT(lock)      ((void)0)
  #define RWLOCK_RDLOCK(lock)    ((void)0)
  #define RWLOCK_WRLOCK(lock)    ((void)0)
  #define RWLOCK_RDUNLOCK(lock)  ((void)0)
  #define RWLOCK_WRUNLOCK(lock)  ((void)0)
  #define RWLOCK_DESTROY(lock)   ((void)0)
#endif /* ENABLE_RWLOCK */
//! Enable rwlocks

/* --------------------------------------------------------------------------
 * ANSI
 * -------------------------------------------------------------------------- */

#define ANSI_ESC  "\x1b["

#define ANSI_COLOR(color, text)  color text ANSI_RESET

#define ANSI_FG_BLACK           ANSI_ESC "30m"
#define ANSI_FG_RED             ANSI_ESC "31m"
#define ANSI_FG_GREEN           ANSI_ESC "32m"
#define ANSI_FG_YELLOW          ANSI_ESC "33m"
#define ANSI_FG_BLUE            ANSI_ESC "34m"
#define ANSI_FG_MAGENTA         ANSI_ESC "35m"
#define ANSI_FG_CYAN            ANSI_ESC "36m"
#define ANSI_FG_WHITE           ANSI_ESC "37m"

#define ANSI_BG_BLACK           ANSI_ESC "40m"
#define ANSI_BG_RED             ANSI_ESC "41m"
#define ANSI_BG_GREEN           ANSI_ESC "42m"
#define ANSI_BG_YELLOW          ANSI_ESC "43m"
#define ANSI_BG_BLUE            ANSI_ESC "44m"
#define ANSI_BG_MAGENTA         ANSI_ESC "45m"
#define ANSI_BG_CYAN            ANSI_ESC "46m"
#define ANSI_BG_WHITE           ANSI_ESC "47m"

#define ANSI_RESET              ANSI_ESC "0m"
#define ANSI_BOLD               ANSI_ESC "1m"
#define ANSI_DIM                ANSI_ESC "2m"
#define ANSI_ITALIC             ANSI_ESC "3m"
#define ANSI_UNDERLINE          ANSI_ESC "4m"
#define ANSI_BLINK              ANSI_ESC "5m"
#define ANSI_REVERSE            ANSI_ESC "7m"
#define ANSI_HIDDEN             ANSI_ESC "8m"
#define ANSI_STRIKETHROUGH      ANSI_ESC "9m"
#define ANSI_DOUBLE_UNDER       ANSI_ESC "21m"
#define ANSI_OVERLINE           ANSI_ESC "53m"
#define ANSI_RESET_TERMINAL     ANSI_ESC "c"

#define ANSI_FG_256(n)          ANSI_ESC "38;5;" #n "m"
#define ANSI_BG_256(n)          ANSI_ESC "48;5;" #n "m"
#define ANSI_FG_RGB(r, g, b)    ANSI_ESC "38;2;" #r ";" #g ";" #b "m"
#define ANSI_BG_RGB(r, g, b)    ANSI_ESC "48;2;" #r ";" #g ";" #b "m"

#define ANSI_CURSOR_UP(n)       ANSI_ESC #n "A"
#define ANSI_CURSOR_DOWN(n)     ANSI_ESC #n "B"
#define ANSI_CURSOR_FORWARD(n)  ANSI_ESC #n "C"
#define ANSI_CURSOR_BACK(n)     ANSI_ESC #n "D"
#define ANSI_CURSOR_POS(x, y)   ANSI_ESC #y ";" #x "H"
#define ANSI_CURSOR_HOME        ANSI_ESC "H"

static inline char* ansi_u16_to_str    (char* buffer, uint16_t n) {
  int i = 0;
  char tmp[8];        // max 65535

  do {
    tmp[i++] = (char)('0' + (n % 10));
    n /= 10;
  } while (n);

  while (i--) {
    *buffer++ = tmp[i];
  }

  return buffer;
}

static inline char* ansi_fg_256        (char* buffer, uint8_t n) {
  *buffer++ = '\x1b';
  *buffer++ = '[';
  *buffer++ = '3';
  *buffer++ = '8';
  *buffer++ = ';';
  *buffer++ = '5';
  *buffer++ = ';';

  buffer = ansi_u16_to_str(buffer, n);

  *buffer++ = 'm';
  return buffer;
}
static inline char* ansi_bg_256        (char* buffer, uint8_t n) {
  *buffer++ = '\x1b';
  *buffer++ = '[';
  *buffer++ = '4';
  *buffer++ = '8';
  *buffer++ = ';';
  *buffer++ = '5';
  *buffer++ = ';';

  buffer = ansi_u16_to_str(buffer, n);

  *buffer++ = 'm';
  return buffer;
}
static inline char* ansi_fg_rgb        (char* buffer, uint8_t r, uint8_t g, uint8_t b) {
  *buffer++ = '\x1b';
  *buffer++ = '[';
  *buffer++ = '3';
  *buffer++ = '8';
  *buffer++ = ';';
  *buffer++ = '2';
  *buffer++ = ';';
  
  buffer = ansi_u16_to_str(buffer, r);
  *buffer++ = ';';

  buffer = ansi_u16_to_str(buffer, g);
  *buffer++ = ';';

  buffer = ansi_u16_to_str(buffer, b);

  *buffer++ = 'm';
  return buffer;
}
static inline char* ansi_bg_rgb        (char* buffer, uint8_t r, uint8_t g, uint8_t b) {
  *buffer++ = '\x1b';
  *buffer++ = '[';
  *buffer++ = '4';
  *buffer++ = '8';
  *buffer++ = ';';
  *buffer++ = '2';
  *buffer++ = ';';
  
  buffer = ansi_u16_to_str(buffer, r);
  *buffer++ = ';';

  buffer = ansi_u16_to_str(buffer, g);
  *buffer++ = ';';

  buffer = ansi_u16_to_str(buffer, b);

  *buffer++ = 'm';
  return buffer;
}

static inline char* ansi_cursor_up     (char* buffer, uint16_t n) {
  *buffer++ = '\x1b';
  *buffer++ = '[';

  buffer = ansi_u16_to_str(buffer, n);

  *buffer++ = 'A';
  return buffer;
}
static inline char* ansi_cursor_down   (char* buffer, uint16_t n) {
  *buffer++ = '\x1b';
  *buffer++ = '[';

  buffer = ansi_u16_to_str(buffer, n);

  *buffer++ = 'B';
  return buffer;
}
static inline char* ansi_cursor_forward(char* buffer, uint16_t n) {
  *buffer++ = '\x1b';
  *buffer++ = '[';

  buffer = ansi_u16_to_str(buffer, n);

  *buffer++ = 'C';
  return buffer;
}
static inline char* ansi_cursor_back   (char* buffer, uint16_t n) {
  *buffer++ = '\x1b';
  *buffer++ = '[';

  buffer = ansi_u16_to_str(buffer, n);

  *buffer++ = 'D';
  return buffer;
}
static inline char* ansi_cursor_pos    (char* buffer, uint16_t x, uint16_t y) {
  *buffer++ = '\x1b';
  *buffer++ = '[';
  
  buffer = ansi_u16_to_str(buffer, y);
  *buffer++ = ';';

  buffer = ansi_u16_to_str(buffer, x);

  *buffer++ = 'H';
  return buffer;
}

#define ANSI_CURSOR_SAVE        ANSI_ESC "s"
#define ANSI_CURSOR_RESTORE     ANSI_ESC "u"
#define ANSI_CURSOR_HIDE        ANSI_ESC "?25l"
#define ANSI_CURSOR_SHOW        ANSI_ESC "?25h"

#define ANSI_CLEAR_SCREEN       ANSI_ESC "2J"
#define ANSI_CLEAR_LINE         ANSI_ESC "2K"
#define ANSI_CLEAR_TO_END       ANSI_ESC "0J"
#define ANSI_CLEAR_TO_START     ANSI_ESC "1J"
#define ANSI_CLEAR_LINE_END     ANSI_ESC "0K"
#define ANSI_CLEAR_LINE_START   ANSI_ESC "1K"

/* --------------------------------------------------------------------------
 * LOG SYSTEM
 * -------------------------------------------------------------------------- */

typedef struct Context {
  const char* alias;
  const char* file;
  const char* func;
  uint32_t    line;
} Context;

#define CONTEXT(alias) ((Context){(alias), FILE_PATH, __func__, __LINE__})

#ifndef LOG_STDOUT
  #define LOG_STDOUT(format, ...) fprintf(stdout, format, __VA_ARGS__)
#endif
#ifndef LOG_STDERR
  #define LOG_STDERR(format, ...) fprintf(stderr, format, __VA_ARGS__)
#endif

#ifdef LOG_COLOURED
  #ifndef LOG_COLOR
    #define LOG_COLOR(msg)          ANSI_COLOR(ANSI_FG_256(212), msg)
  #endif
  #ifndef LOG_DEBUG_COLOR
    #define LOG_DEBUG_COLOR(msg)    ANSI_COLOR(ANSI_FG_256(87),  msg)
  #endif
  #ifndef LOG_SUCCESS_COLOR
    #define LOG_SUCCESS_COLOR(msg)  ANSI_COLOR(ANSI_FG_256(46),  msg)
  #endif
  #ifndef LOG_WARNING_COLOR
    #define LOG_WARNING_COLOR(msg)  ANSI_COLOR(ANSI_FG_256(214), msg)
  #endif
  #ifndef LOG_ERROR_COLOR
    #define LOG_ERROR_COLOR(msg)    ANSI_COLOR(ANSI_FG_256(196), msg)
  #endif
  #ifndef LOG_FATAL_COLOR
    #define LOG_FATAL_COLOR(msg)    ANSI_COLOR(ANSI_FG_256(165), msg)
  #endif
  #ifndef LOG_BOUNDS_COLOR
    #define LOG_BOUNDS_COLOR(msg)   ANSI_COLOR(ANSI_FG_256(165), msg)
  #endif
#else
  #define LOG_COLOR(msg)          msg
  #define LOG_DEBUG_COLOR(msg)    msg
  #define LOG_SUCCESS_COLOR(msg)  msg
  #define LOG_WARNING_COLOR(msg)  msg
  #define LOG_ERROR_COLOR(msg)    msg
  #define LOG_FATAL_COLOR(msg)    msg
  #define LOG_BOUNDS_COLOR(msg)   msg
#endif

#ifndef LOG_TITLE
  #define LOG_TITLE             "[LOG]"
#endif /* LOG_TITLE */
#ifndef LOG_DEBUG_TITLE
  #define LOG_DEBUG_TITLE       "[DEBUG]"
#endif /* LOG_DEBUG_TITLE */
#ifndef LOG_SUCCESS_TITLE
  #define LOG_SUCCESS_TITLE     "[SUCCESS]"
#endif /* LOG_SUCCESS_TITLE */
#ifndef LOG_WARNING_TITLE
  #define LOG_WARNING_TITLE     "[WARNING]"
#endif /* LOG_WARNING_TITLE */
#ifndef LOG_ERROR_TITLE
  #define LOG_ERROR_TITLE       "[ERROR]"
#endif /* LOG_ERROR_TITLE */
#ifndef LOG_FATAL_TITLE 
  #define LOG_FATAL_TITLE       "[FATAL]"
#endif /* LOG_FATAL_TITLE */
#ifndef LOG_BOUNDS_TITLE
  #define LOG_BOUNDS_TITLE      "[BOUNDS]"
#endif /* LOG_BOUNDS_TITLE */

#define LOG(msg)                    do { LOG_STDOUT(LOG_COLOR(LOG_TITLE)                  "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)            
#define LOG_DEBUG(msg)              do { LOG_STDOUT(LOG_DEBUG_COLOR(LOG_DEBUG_TITLE)      "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)      
#define LOG_SUCCESS(msg)            do { LOG_STDOUT(LOG_SUCCESS_COLOR(LOG_SUCCESS_TITLE)  "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)
#define LOG_WARNING(msg)            do { LOG_STDERR(LOG_WARNING_COLOR(LOG_WARNING_TITLE)  "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)
#define LOG_ERROR(msg)              do { LOG_STDERR(LOG_ERROR_COLOR(LOG_ERROR_TITLE)      "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)
#define LOG_FATAL(msg)              do { LOG_STDERR(LOG_FATAL_COLOR(LOG_FATAL_TITLE)      "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)
#define LOG_BOUNDS(idx, bound)      do { LOG_STDERR(LOG_BOUNDS_COLOR(LOG_BOUNDS_TITLE)    "\n Context: %s:%u (%s)\n Message: Index (%zu) out of bounds (%zu)\n\n", FILE_PATH, __LINE__, __func__, idx, bound); } while (0)

#define LOG_BUFFER_SIZE 1024
#define LOG_TEMP_BUFFER char log_temp_buffer[LOG_BUFFER_SIZE] = {0}

#define LOGF(format, ...)           do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); LOG(log_temp_buffer);          } while (0)            
#define LOGF_DEBUG(format, ...)     do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); LOG_DEBUG(log_temp_buffer);    } while (0)      
#define LOGF_SUCCESS(format, ...)   do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); LOG_SUCCESS(log_temp_buffer);  } while (0)
#define LOGF_WARNING(format, ...)   do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); LOG_WARNING(log_temp_buffer);  } while (0)
#define LOGF_ERROR(format, ...)     do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); LOG_ERROR(log_temp_buffer);    } while (0)
#define LOGF_FATAL(format, ...)     do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); LOG_FATAL(log_temp_buffer);    } while (0)

#define BLOP_LOG(msg)               do { ((void)0) } while(0)            
#define BLOP_DEBUG(msg)             do { ((void)0) } while(0)      
#define BLOP_SUCCESS(msg)           do { ((void)0) } while(0)
#define BLOP_WARNING(msg)           do { ((void)0) } while(0)
#define BLOP_ERROR(msg)             do { ((void)0) } while(0)
#define BLOP_FATAL(msg)             do { LOG_STDERR(LOG_FATAL_COLOR("[BLOP FATAL]")     "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)
#define BLOP_BOUNDS(idx, bound)     do { LOG_STDERR(LOG_FATAL_COLOR("[BLOP FATAL]")     "\n Context: %s:%u (%s)\n Message: Index (%zu) out of bounds (%zu)\n\n", FILE_PATH, __LINE__, __func__, idx, bound); } while (0)

#define BLOPF_LOG(format, ...)      do { ((void)0) } while(0)            
#define BLOPF_DEBUG(format, ...)    do { ((void)0) } while(0)      
#define BLOPF_SUCCESS(format, ...)  do { ((void)0) } while(0)
#define BLOPF_WARNING(format, ...)  do { ((void)0) } while(0)
#define BLOPF_ERROR(format, ...)    do { ((void)0) } while(0)
#define BLOPF_FATAL(format, ...)    do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); BLOP_FATAL(log_temp_buffer); } while (0)

#ifdef ENABLE_BLOP_LOGS
  #undef  BLOP_LOG
  #undef  BLOP_DEBUG
  #undef  BLOP_SUCCESS
  #undef  BLOP_WARNING
  #undef  BLOP_ERROR

  #define BLOP_LOG(msg)               do { LOG_STDOUT(LOG_COLOR("[BLOP LOG]")             "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)            
  #define BLOP_DEBUG(msg)             do { LOG_STDOUT(LOG_DEBUG_COLOR("[BLOP DEBUG]")     "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)      
  #define BLOP_SUCCESS(msg)           do { LOG_STDOUT(LOG_SUCCESS_COLOR("[BLOP SUCCESS]") "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)
  #define BLOP_WARNING(msg)           do { LOG_STDERR(LOG_WARNING_COLOR("[BLOP WARNING]") "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)
  #define BLOP_ERROR(msg)             do { LOG_STDERR(LOG_ERROR_COLOR("[BLOP ERROR]")     "\n Context: %s:%u (%s)\n Message: %s\n\n", FILE_PATH, __LINE__, __func__, msg); } while (0)

  #undef  BLOPF_LOG
  #undef  BLOPF_DEBUG
  #undef  BLOPF_SUCCESS
  #undef  BLOPF_WARNING
  #undef  BLOPF_ERROR

  #define BLOPF_LOG(format, ...)      do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); BLOP_LOG(log_temp_buffer);     } while (0)            
  #define BLOPF_DEBUG(format, ...)    do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); BLOP_DEBUG(log_temp_buffer);   } while (0)      
  #define BLOPF_SUCCESS(format, ...)  do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); BLOP_SUCCESS(log_temp_buffer); } while (0)
  #define BLOPF_WARNING(format, ...)  do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); BLOP_WARNING(log_temp_buffer); } while (0)
  #define BLOPF_ERROR(format, ...)    do { LOG_TEMP_BUFFER; snprintf(log_temp_buffer, LOG_BUFFER_SIZE, format, __VA_ARGS__); BLOP_ERROR(log_temp_buffer);   } while (0)
#endif

#if defined(LOG_DISABLE_LOG) || defined(LOG_DISABLE_ALL) 
  #undef  LOG
  #undef  LOGF
  #define LOG(msg)                  do { ((void)0) } while(0)
  #define LOGF(format, ...)         do { ((void)0) } while(0)
#endif

#if defined(LOG_DISABLE_DEBUG) || defined(LOG_DISABLE_ALL)
  #undef  LOG_DEBUG
  #undef  LOGF_DEBUG
  #define LOG_DEBUG(msg)            do { ((void)0) } while(0)
  #define LOGF_DEBUG(format, ...)   do { ((void)0) } while(0)
#endif

#if defined(LOG_DISABLE_SUCCESS) || defined(LOG_DISABLE_ALL)
  #undef  LOG_SUCCESS
  #undef  LOGF_SUCCESS
  #define LOG_SUCCESS(msg)          do { ((void)0) } while(0)
  #define LOGF_SUCCESS(format, ...) do { ((void)0) } while(0)
#endif

#if defined(LOG_DISABLE_WARNING) || defined(LOG_DISABLE_ALL)
  #undef  LOG_WARNING
  #undef  LOGF_WARNING
  #define LOG_WARNING(msg)          do { ((void)0) } while(0)
  #define LOGF_WARNING(format, ...) do { ((void)0) } while(0)
#endif

#if defined(LOG_DISABLE_ERROR) || defined(LOG_DISABLE_ALL)
  #undef  LOG_ERROR
  #undef  LOGF_ERROR
  #define LOG_ERROR(msg)            do { ((void)0) } while(0)
  #define LOGF_ERROR(format, ...)   do { ((void)0) } while(0)
#endif

#if defined(LOG_DISABLE_FATAL) || defined(LOG_DISABLE_ALL)
  #undef  LOG_FATAL
  #undef  LOGF_FATAL
  #define LOG_FATAL(msg)            do { ((void)0) } while(0)
  #define LOGF_FATAL(format, ...)   do { ((void)0) } while(0)
#endif

#if defined(LOG_DISABLE_BOUNDS) || defined(LOG_DISABLE_ALL)
  #undef  LOG_BOUNDS
  #define LOG_BOUNDS(idx, bound)    do { ((void)0) } while(0)
#endif

/* --------------------------------------------------------------------------
 * ASSERT SYSTEM
 * -------------------------------------------------------------------------- */

static inline NORETURN void ABORT() {
  #ifndef ABORT_ON_ERROR
    exit(-1);
  #else
    abort();
  #endif
}

//! Enable Empty Popping
#ifndef ENABLE_EMPTY_POPPING
  #define EMPTY_POPPING()               do { BLOP_FATAL("To enable empty popping and avoid this error define ENABLE_EMPTY_POPPING");              ABORT();   } while(0)
#else
  #define EMPTY_POPPING()               do { ((void)0) } while(0)
#endif /* ENABLE_EMPTY_POPPING */
//! Enable Empty Popping

#define ASSERT(cnd, msg)                      do { if (!(cnd))            { LOG_FATAL(msg);                                                                 ABORT(); } } while(0)
#define ASSERT_PTR(ptr)                       do { if ((ptr) == NULL)     { LOG_FATAL("'"#ptr "' is NULL");                                                 ABORT(); } } while(0)
#define ASSERT_BOUNDS(idx, bound)             do { if ((idx) >= (bound))  { LOG_BOUNDS(idx, bound);                                                         ABORT(); } } while(0)
#define ASSERT_FORCED(cnd, msg)               do { if (!(cnd))            { LOG_FATAL(msg);                                                                 ABORT(); } } while(0)
#define ASSERT_MALLOC(ptr, type, size)        do { if (ptr == NULL)       { LOG_FATAL("Malloc failed for "  #ptr " of type " #type " and size "  #size);    ABORT(); } } while(0)
#define ASSERT_CALLOC(ptr, type, count)       do { if (ptr == NULL)       { LOG_FATAL("Calloc failed for "  #ptr " of type " #type " and count " #count);   ABORT(); } } while(0)
#define ASSERT_REALLOC(ptr, type, size)       do { if (ptr == NULL)       { LOG_FATAL("Realloc failed for " #ptr " of type " #type " and size "  #size);    ABORT(); } } while(0)

#define ASSERTF(cnd, format, ...)             do { if (!(cnd))            { LOGF_FATAL(format, __VA_ARGS__);                                                ABORT(); } } while(0) 
#define ASSERTF_FORCED(cnd, format, ...)      do { if (!(cnd))            { LOGF_FATAL(format, __VA_ARGS__);                                                ABORT(); } } while(0) 

#define BLOP_ASSERT(cnd, msg)                 do { if (!(cnd))            { BLOP_FATAL(msg);                                                                ABORT(); } } while(0)
#define BLOP_ASSERT_PTR(ptr)                  do { if ((ptr) == NULL)     { BLOP_FATAL("'"#ptr "' is NULL");                                                ABORT(); } } while(0)
#define BLOP_ASSERT_BOUNDS(idx, bound)        do { if ((idx) >= (bound))  { BLOP_BOUNDS(idx, bound);                                                        ABORT(); } } while(0)
#define BLOP_ASSERT_FORCED(cnd, msg)          do { if (!(cnd))            { BLOP_FATAL(msg);                                                                ABORT(); } } while(0)

#define BLOPF_ASSERT(cnd, format, ...)        do { if (!(cnd))            { BLOPF_FATAL(format, __VA_ARGS__);                                               ABORT(); } } while(0) 
#define BLOPF_ASSERT_FORCED(cnd, format, ...) do { if (!(cnd))            { BLOPF_FATAL(format, __VA_ARGS__);                                               ABORT(); } } while(0) 

#ifdef DISABLE_BLOP_ASSERTIONS
  #undef BLOP_ASSERT
  #undef BLOP_ASSERT_PTR
  #undef BLOPF_ASSERT

  #define BLOP_ASSERT(cnd, msg)           do { ((void)0) } while(0)
  #define BLOP_ASSERT_PTR(ptr)            do { ((void)0) } while(0)
  #define BLOPF_ASSERT(cnd, format, ...)  do { ((void)0) } while(0)
#endif /* DISABLE_BLOP_ASSERTIONS */
  
#if defined(ASSERT_DISABLE) || defined(ASSERT_DISABLE_ALL)
  #undef  ASSERT
  #undef  ASSERTF
  #define ASSERT(cnd, msg)            do { ((void)0) } while(0)
  #define ASSERTF(cnd, format, ...)   do { ((void)0) } while(0)
#endif

#if defined(ASSERT_DISABLE_PTR) || defined(ASSERT_DISABLE_ALL)
  #undef  ASSERT_PTR
  #define ASSERT_PTR(ptr)             do { ((void)0) } while(0)
#endif

#if defined(ASSERT_DISABLE_BOUNDS) || defined(ASSERT_DISABLE_ALL)
  #undef  ASSERT_BOUNDS
  #define ASSERT_BOUNDS(idx, bound)   do { ((void)0) } while(0)
#endif

/* --------------------------------------------------------------------------
 * This goes here for the only purpose to use the assertion system
 * Putting this code upper in the file will generate an error
 * -------------------------------------------------------------------------- */

#ifdef OS_WINDOWS
  #include <windows.h>

  static inline void ANSI_ENABLE() {
    HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    BLOP_ASSERT_FORCED(err != INVALID_HANDLE_VALUE, "Failed to get stderr handle (windows.h)");
    BLOP_ASSERT_FORCED(out != INVALID_HANDLE_VALUE, "Failed to get stdout handle (windows.h)");

    DWORD err_mode = 0;
    DWORD out_mode = 0;
    BLOP_ASSERT_FORCED(GetConsoleMode(err, &err_mode), "Failed to get console err_mode (windows.h)");
    BLOP_ASSERT_FORCED(GetConsoleMode(out, &out_mode), "Failed to get console out_mode (windows.h)");

    if (!(err_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
      DWORD err_nmode = err_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
      BLOP_ASSERT_FORCED(SetConsoleMode(err, err_nmode) != 0, "Failed to set console err_mode (windows.h)");
    }

    if (!(out_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
      DWORD out_nmode = out_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
      BLOP_ASSERT_FORCED(SetConsoleMode(out, out_nmode) != 0, "Failed to set console out_mode (windows.h)");
    }
  }
  static inline void ANSI_DISABLE()  {
    HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    BLOP_ASSERT_FORCED(err != INVALID_HANDLE_VALUE, "Failed to get std handle (windows.h)");
    BLOP_ASSERT_FORCED(out != INVALID_HANDLE_VALUE, "Failed to get std handle (windows.h)");

    DWORD err_mode = 0;
    DWORD out_mode = 0;
    BLOP_ASSERT_FORCED(GetConsoleMode(err, &err_mode), "Failed to get console mode (windows.h)");
    BLOP_ASSERT_FORCED(GetConsoleMode(out, &out_mode), "Failed to get console mode (windows.h)");

    DWORD err_nmode = err_mode & ~(ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
    DWORD out_nmode = out_mode & ~(ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
  
    BLOP_ASSERT_FORCED(SetConsoleMode(err, err_nmode) != 0, "Failed to set console mode (windows.h)");
    BLOP_ASSERT_FORCED(SetConsoleMode(out, out_nmode) != 0, "Failed to set console mode (windows.h)");
  }
#else
  #define ANSI_ENABLE()  ((void)0)
  #define ANSI_DISABLE() ((void)0)
#endif

/* --------------------------------------------------------------------------
 * WRAPPERS
 * -------------------------------------------------------------------------- */

#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)

#define CONCAT2_IMPL(a, b)     a##b
#define CONCAT3_IMPL(a, b, c)  a##b##c

#define CONCAT2(a, b)          CONCAT2_IMPL(a, b)
#define CONCAT3(a, b, c)       CONCAT3_IMPL(a, b, c)

#define FREE(ptr)                   do { free((void*)(ptr)); (ptr) = NULL; } while(0)
#define FREE_IF(ptr)                do { if ((ptr)) { FREE((ptr)); } } while(0)
#define MALLOC(v, type, size)       do { (v) = (type*)malloc((size));                ASSERT_MALLOC((v), type, (size)); memset(v, 0, (size)); } while(0)
#define CALLOC(v, type, count)      do { (v) = (type*)calloc((count), sizeof(type)); ASSERT_CALLOC((v), type, (count));                      } while(0)
#define REALLOC(v, type, ptr, size) do { (v) = (type*)realloc((void*)(ptr), (size)); ASSERT_REALLOC((v), type, (size));                      } while(0)

#define CAST(type, tocast) ((type)(tocast))
#define TERNARY(cnd, x, y) ((cnd) ? (x) : (y))

#define PTR_ADD(ptr, value) ((void*)((uint8_t*)(ptr) + (value)))
#define PTR_SUB(ptr, value) ((void*)((uint8_t*)(ptr) - (value)))

#define MIN(x, y)              TERNARY(x > y, y, x)
#define MAX(x, y)              TERNARY(x > y, x, y)
#define DISTANCE(x, y)        (TERNARY(x > y, x - y, y - x))

#endif /* __BLOP_H__ */
