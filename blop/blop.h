#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------------
 * SYSTEM & TOOLCHAIN
 * -------------------------------------------------------------------------- */

/* OS Family -------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64)
  #define BLOP_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
  #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE
    #define BLOP_IOS
  #else
    #define BLOP_MACOS
  #endif
#elif defined(__linux__)
  #define BLOP_LINUX
#elif defined(__unix__) || defined(__unix)
  #define BLOP_UNIX
#elif defined(__ANDROID__)
  #define BLOP_ANDROID
#else
  #define BLOP_UNKNOWN_PLATFORM
#endif

#if defined(BLOP_LINUX) || defined(BLOP_MACOS) || defined(BLOP_UNIX) || defined(BLOP_ANDROID) || defined(BLOP_IOS)
  #define BLOP_POSIX
#endif
/* ------------------------------------------------------------------------ */

/* Compiler --------------------------------------------------------------- */
#if defined(__clang__)
    #define BLOP_COMPILER_CLANG
#elif defined(__GNUC__)
    #define BLOP_COMPILER_GCC
#elif defined(_MSC_VER)
    #define BLOP_COMPILER_MSVC
#else
    #define BLOP_COMPILER_UNKNOWN
#endif

/* CPU Architecture ------------------------------------------------------- */
#if defined(__x86_64__) || defined(_M_X64)
    #define BLOP_CPU_X64
#elif defined(__i386__) || defined(_M_IX86)
    #define BLOP_CPU_X86
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define BLOP_CPU_ARM64
#elif defined(__arm__) || defined(_M_ARM)
    #define BLOP_CPU_ARM32
#else
    #define BLOP_CPU_UNKNOWN
#endif

/* Pointer Size ----------------------------------------------------------- */
#if UINTPTR_MAX == 0xffffffffffffffffULL
    #define BLOP_64BIT
#else
    #define BLOP_32BIT
#endif

/* --------------------------------------------------------------------------
 * CROSSPLATFORM
 * -------------------------------------------------------------------------- */

#if defined(BLOP_COMPILER_GCC) || defined(BLOP_COMPILER_CLANG)
  #define BLOP_NORETURN __attribute__((noreturn))
#elif defined(BLOP_COMPILER_MSVC)
  #define BLOP_NORETURN __declspec(noreturn)
#else
  #define BLOP_NORETURN _Noreturn
#endif

#if defined(__FILE_NAME__)
  #define BLOP_FILE_PATH __FILE_NAME__
#else
  #define BLOP_FILE_PATH __FILE__
#endif

/* --------------------------------------------------------------------------
 * RWLOCK
 * -------------------------------------------------------------------------- */

//! Enable rwlocks
#if defined(BLOP_ENABLE_RWLOCK) && !defined(BLOP_UNKNOWN_PLATFORM) 
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
#endif /* BLOP_ENABLE_RWLOCK */
//! Enable rwlocks

/* --------------------------------------------------------------------------
 * ANSI
 * -------------------------------------------------------------------------- */

#define BLOP_ANSI_ESC  "\x1b["

#ifdef BLOP_WINDOWS
  void blop_impl_ansi_enable();
  void blop_impl_ansi_disable();

  #define BLOP_ANSI_ENABLE()  blop_impl_ansi_enable()
  #define BLOP_ANSI_DISABLE() blop_impl_ansi_disable()
#else
  #define BLOP_ANSI_ENABLE()  ((void)0)
  #define BLOP_ANSI_DISABLE() ((void)0)
#endif

#define BLOP_ANSI_COLOR(color, text)  color text BLOP_ANSI_RESET

#define BLOP_ANSI_FG_BLACK            BLOP_ANSI_ESC "30m"
#define BLOP_ANSI_FG_RED              BLOP_ANSI_ESC "31m"
#define BLOP_ANSI_FG_GREEN            BLOP_ANSI_ESC "32m"
#define BLOP_ANSI_FG_YELLOW           BLOP_ANSI_ESC "33m"
#define BLOP_ANSI_FG_BLUE             BLOP_ANSI_ESC "34m"
#define BLOP_ANSI_FG_MAGENTA          BLOP_ANSI_ESC "35m"
#define BLOP_ANSI_FG_CYAN             BLOP_ANSI_ESC "36m"
#define BLOP_ANSI_FG_WHITE            BLOP_ANSI_ESC "37m"

#define BLOP_ANSI_BG_BLACK            BLOP_ANSI_ESC "40m"
#define BLOP_ANSI_BG_RED              BLOP_ANSI_ESC "41m"
#define BLOP_ANSI_BG_GREEN            BLOP_ANSI_ESC "42m"
#define BLOP_ANSI_BG_YELLOW           BLOP_ANSI_ESC "43m"
#define BLOP_ANSI_BG_BLUE             BLOP_ANSI_ESC "44m"
#define BLOP_ANSI_BG_MAGENTA          BLOP_ANSI_ESC "45m"
#define BLOP_ANSI_BG_CYAN             BLOP_ANSI_ESC "46m"
#define BLOP_ANSI_BG_WHITE            BLOP_ANSI_ESC "47m"

#define BLOP_ANSI_RESET_FULL          BLOP_ANSI_ESC "c"
#define BLOP_ANSI_RESET               BLOP_ANSI_ESC "0m"
#define BLOP_ANSI_BOLD                BLOP_ANSI_ESC "1m"
#define BLOP_ANSI_DIM                 BLOP_ANSI_ESC "2m"
#define BLOP_ANSI_ITALIC              BLOP_ANSI_ESC "3m"
#define BLOP_ANSI_UNDERLINE           BLOP_ANSI_ESC "4m"
#define BLOP_ANSI_BLINK               BLOP_ANSI_ESC "5m"
#define BLOP_ANSI_REVERSE             BLOP_ANSI_ESC "7m"
#define BLOP_ANSI_HIDDEN              BLOP_ANSI_ESC "8m"
#define BLOP_ANSI_STRIKETHROUGH       BLOP_ANSI_ESC "9m"
#define BLOP_ANSI_DOUBLE_UNDER        BLOP_ANSI_ESC "21m"
#define BLOP_ANSI_OVERLINE            BLOP_ANSI_ESC "53m"

#define BLOP_ANSI_FG_256(n)           BLOP_ANSI_ESC "38;5;" #n "m"
#define BLOP_ANSI_BG_256(n)           BLOP_ANSI_ESC "48;5;" #n "m"
#define BLOP_ANSI_FG_RGB(r, g, b)     BLOP_ANSI_ESC "38;2;" #r ";" #g ";" #b "m"
#define BLOP_ANSI_BG_RGB(r, g, b)     BLOP_ANSI_ESC "48;2;" #r ";" #g ";" #b "m"

#define BLOP_ANSI_CURSOR_UP(n)        BLOP_ANSI_ESC #n "A"
#define BLOP_ANSI_CURSOR_DOWN(n)      BLOP_ANSI_ESC #n "B"
#define BLOP_ANSI_CURSOR_FORWARD(n)   BLOP_ANSI_ESC #n "C"
#define BLOP_ANSI_CURSOR_BACK(n)      BLOP_ANSI_ESC #n "D"
#define BLOP_ANSI_CURSOR_POS(x, y)    BLOP_ANSI_ESC #y ";" #x "H"
#define BLOP_ANSI_CURSOR_HOME         BLOP_ANSI_ESC "H"

char*   blop_ansi_fg_256        (char* buffer, uint8_t n);
char*   blop_ansi_bg_256        (char* buffer, uint8_t n);
char*   blop_ansi_fg_rgb        (char* buffer, uint8_t r, uint8_t g, uint8_t b);
char*   blop_ansi_bg_rgb        (char* buffer, uint8_t r, uint8_t g, uint8_t b);

char*   blop_ansi_cursor_up     (char* buffer, uint16_t n);
char*   blop_ansi_cursor_down   (char* buffer, uint16_t n);
char*   blop_ansi_cursor_forward(char* buffer, uint16_t n);
char*   blop_ansi_cursor_back   (char* buffer, uint16_t n);
char*   blop_ansi_cursor_pos    (char* buffer, uint16_t x, uint16_t y);

#define BLOP_ANSI_CURSOR_SAVE             BLOP_ANSI_ESC "s"
#define BLOP_ANSI_CURSOR_RESTORE          BLOP_ANSI_ESC "u"
#define BLOP_ANSI_CURSOR_HIDE             BLOP_ANSI_ESC "?25l"
#define BLOP_ANSI_CURSOR_SHOW             BLOP_ANSI_ESC "?25h"

#define BLOP_ANSI_CLEAR_SCREEN            BLOP_ANSI_ESC "2J"
#define BLOP_ANSI_CLEAR_LINE              BLOP_ANSI_ESC "2K"
#define BLOP_ANSI_CLEAR_TO_END            BLOP_ANSI_ESC "0J"
#define BLOP_ANSI_CLEAR_TO_START          BLOP_ANSI_ESC "1J"
#define BLOP_ANSI_CLEAR_LINE_END          BLOP_ANSI_ESC "0K"
#define BLOP_ANSI_CLEAR_LINE_START        BLOP_ANSI_ESC "1K"

#ifdef BLOP_ANSI_SHORT
  #define ANSI_ENABLE()  BLOP_ANSI_ENABLE()
  #define ANSI_DISABLE() BLOP_ANSI_DISABLE()

  #define ANSI_COLOR(color, text) BLOP_ANSI_COLOR(color, text)

  #define ANSI_FG_BLACK                   BLOP_ANSI_FG_BLACK          
  #define ANSI_FG_RED                     BLOP_ANSI_FG_RED            
  #define ANSI_FG_GREEN                   BLOP_ANSI_FG_GREEN          
  #define ANSI_FG_YELLOW                  BLOP_ANSI_FG_YELLOW         
  #define ANSI_FG_BLUE                    BLOP_ANSI_FG_BLUE           
  #define ANSI_FG_MAGENTA                 BLOP_ANSI_FG_MAGENTA        
  #define ANSI_FG_CYAN                    BLOP_ANSI_FG_CYAN           
  #define ANSI_FG_WHITE                   BLOP_ANSI_FG_WHITE         

  #define ANSI_BG_BLACK                   BLOP_ANSI_BG_BLACK          
  #define ANSI_BG_RED                     BLOP_ANSI_BG_RED            
  #define ANSI_BG_GREEN                   BLOP_ANSI_BG_GREEN          
  #define ANSI_BG_YELLOW                  BLOP_ANSI_BG_YELLOW         
  #define ANSI_BG_BLUE                    BLOP_ANSI_BG_BLUE           
  #define ANSI_BG_MAGENTA                 BLOP_ANSI_BG_MAGENTA        
  #define ANSI_BG_CYAN                    BLOP_ANSI_BG_CYAN           
  #define ANSI_BG_WHITE                   BLOP_ANSI_BG_WHITE        

  #define ANSI_RESET_FULL                 BLOP_ANSI_RESET_FULL        
  #define ANSI_RESET                      BLOP_ANSI_RESET             
  #define ANSI_BOLD                       BLOP_ANSI_BOLD              
  #define ANSI_DIM                        BLOP_ANSI_DIM               
  #define ANSI_ITALIC                     BLOP_ANSI_ITALIC            
  #define ANSI_UNDERLINE                  BLOP_ANSI_UNDERLINE         
  #define ANSI_BLINK                      BLOP_ANSI_BLINK             
  #define ANSI_REVERSE                    BLOP_ANSI_REVERSE           
  #define ANSI_HIDDEN                     BLOP_ANSI_HIDDEN            
  #define ANSI_STRIKETHROUGH              BLOP_ANSI_STRIKETHROUGH     
  #define ANSI_DOUBLE_UNDER               BLOP_ANSI_DOUBLE_UNDER      
  #define ANSI_OVERLINE                   BLOP_ANSI_OVERLINE       

  #define ANSI_FG_256(n)                  BLOP_ANSI_FG_256(n)         
  #define ANSI_BG_256(n)                  BLOP_ANSI_BG_256(n)         
  #define ANSI_FG_RGB(r, g, b)            BLOP_ANSI_FG_RGB(r, g, b)   
  #define ANSI_BG_RGB(r, g, b)            BLOP_ANSI_BG_RGB(r, g, b)   

  #define ANSI_CURSOR_UP(n)               BLOP_ANSI_CURSOR_UP(n)      
  #define ANSI_CURSOR_DOWN(n)             BLOP_ANSI_CURSOR_DOWN(n)    
  #define ANSI_CURSOR_FORWARD(n)          BLOP_ANSI_CURSOR_FORWARD(n) 
  #define ANSI_CURSOR_BACK(n)             BLOP_ANSI_CURSOR_BACK(n)    
  #define ANSI_CURSOR_POS(x, y)           BLOP_ANSI_CURSOR_POS(x, y)
  #define ANSI_CURSOR_HOME                BLOP_ANSI_CURSOR_HOME

  #define ansi_fg_256(buffer, n)          blop_ansi_fg_256(buffer, n)
  #define ansi_bg_256(buffer, n)          blop_ansi_bg_256(buffer, n)
  #define ansi_fg_rgb(buffer, r, g, b)    blop_ansi_fg_rgb(buffer, r, g, b)
  #define ansi_bg_rgb(buffer, r, g, b)    blop_ansi_bg_rgb(buffer, r, g, b)

  #define ansi_cursor_up(buffer, n)       blop_ansi_cursor_up(buffer, n)
  #define ansi_cursor_down(buffer, n)     blop_ansi_cursor_down(buffer, n)
  #define ansi_cursor_forward(buffer, n)  blop_ansi_cursor_forward(buffer, n)
  #define ansi_cursor_back(buffer, n)     blop_ansi_cursor_back(buffer, n)
  #define ansi_cursor_pos(buffer, x, y)   blop_ansi_cursor_pos(buffer, x, y)

  #define ANSI_CURSOR_SAVE                BLOP_ANSI_CURSOR_SAVE       
  #define ANSI_CURSOR_RESTORE             BLOP_ANSI_CURSOR_RESTORE    
  #define ANSI_CURSOR_HIDE                BLOP_ANSI_CURSOR_HIDE       
  #define ANSI_CURSOR_SHOW                BLOP_ANSI_CURSOR_SHOW       

  #define ANSI_CLEAR_SCREEN               BLOP_ANSI_CLEAR_SCREEN      
  #define ANSI_CLEAR_LINE                 BLOP_ANSI_CLEAR_LINE        
  #define ANSI_CLEAR_TO_END               BLOP_ANSI_CLEAR_TO_END      
  #define ANSI_CLEAR_TO_START             BLOP_ANSI_CLEAR_TO_START    
  #define ANSI_CLEAR_LINE_END             BLOP_ANSI_CLEAR_LINE_END    
  #define ANSI_CLEAR_LINE_START           BLOP_ANSI_CLEAR_LINE_START  
#endif /* BLOP_ANSI_SHORT */

/* --------------------------------------------------------------------------
 * LOG SYSTEM
 * -------------------------------------------------------------------------- */

typedef struct blop_context {
  const char* alias;
  const char* file;
  const char* func;
  uint32_t    line;
} blop_context;

#define BLOP_CONTEXT(alias) ((blop_context){(alias), BLOP_FILE_PATH, __func__, __LINE__})

#ifndef BLOP_LOG_LEVEL1
  #define BLOP_LOG_LEVEL1(format, ...) fprintf(stdout, format, __VA_ARGS__)
#endif
#ifndef BLOP_LOG_LEVEL2
  #define BLOP_LOG_LEVEL2(format, ...) fprintf(stderr, format, __VA_ARGS__)
#endif

#ifdef BLOP_LOG_COLOURED
  #ifndef BLOP_LOG_LCOLOR
    #define BLOP_LOG_LCOLOR(msg)  BLOP_ANSI_COLOR(BLOP_ANSI_FG_256(212), msg)
  #endif
  #ifndef BLOP_LOG_DCOLOR
    #define BLOP_LOG_DCOLOR(msg)  BLOP_ANSI_COLOR(BLOP_ANSI_FG_256(87),  msg)
  #endif
  #ifndef BLOP_LOG_SCOLOR
    #define BLOP_LOG_SCOLOR(msg)  BLOP_ANSI_COLOR(BLOP_ANSI_FG_256(46),  msg)
  #endif
  #ifndef BLOP_LOG_WCOLOR
    #define BLOP_LOG_WCOLOR(msg)  BLOP_ANSI_COLOR(BLOP_ANSI_FG_256(214), msg)
  #endif
  #ifndef BLOP_LOG_ECOLOR
    #define BLOP_LOG_ECOLOR(msg)  BLOP_ANSI_COLOR(BLOP_ANSI_FG_256(196), msg)
  #endif
  #ifndef BLOP_LOG_FCOLOR
    #define BLOP_LOG_FCOLOR(msg)  BLOP_ANSI_COLOR(BLOP_ANSI_FG_256(165), msg)
  #endif
  #ifndef BLOP_LOG_BCOLOR
    #define BLOP_LOG_BCOLOR(msg)  BLOP_ANSI_COLOR(BLOP_ANSI_FG_256(165), msg)
  #endif
#else
  #define BLOP_LOG_LCOLOR(msg)  msg
  #define BLOP_LOG_DCOLOR(msg)  msg
  #define BLOP_LOG_SCOLOR(msg)  msg
  #define BLOP_LOG_WCOLOR(msg)  msg
  #define BLOP_LOG_ECOLOR(msg)  msg
  #define BLOP_LOG_FCOLOR(msg)  msg
  #define BLOP_LOG_BCOLOR(msg)  msg
#endif

#ifndef BLOP_LOG_LTITLE
  #define BLOP_LOG_LTITLE     "[LOG]"
#endif /* BLOP_LOG_LTITLE */
#ifndef BLOP_LOG_DTITLE
  #define BLOP_LOG_DTITLE     "[DEBUG]"
#endif /* BLOP_LOG_DTITLE */
#ifndef BLOP_LOG_STITLE
  #define BLOP_LOG_STITLE     "[SUCCESS]"
#endif /* BLOP_LOG_STITLE */
#ifndef BLOP_LOG_WTITLE
  #define BLOP_LOG_WTITLE     "[WARNING]"
#endif /* BLOP_LOG_WTITLE */
#ifndef BLOP_LOG_ETITLE
  #define BLOP_LOG_ETITLE     "[ERROR]"
#endif /* BLOP_LOG_ETITLE */
#ifndef BLOP_LOG_FTITLE
  #define BLOP_LOG_FTITLE     "[FATAL]"
#endif /* BLOP_LOG_FTITLE */
#ifndef BLOP_LOG_BTITLE
  #define BLOP_LOG_BTITLE     "[BOUNDS]"
#endif /* BLOP_LOG_BTITLE */

#define BLOP_LOG(msg)               do { BLOP_LOG_LEVEL1(BLOP_LOG_LCOLOR(BLOP_LOG_LTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)            
#define BLOP_DEBUG(msg)             do { BLOP_LOG_LEVEL1(BLOP_LOG_DCOLOR(BLOP_LOG_DTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)      
#define BLOP_SUCCESS(msg)           do { BLOP_LOG_LEVEL1(BLOP_LOG_SCOLOR(BLOP_LOG_STITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
#define BLOP_WARNING(msg)           do { BLOP_LOG_LEVEL2(BLOP_LOG_WCOLOR(BLOP_LOG_WTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
#define BLOP_ERROR(msg)             do { BLOP_LOG_LEVEL2(BLOP_LOG_ECOLOR(BLOP_LOG_ETITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
#define BLOP_FATAL(msg)             do { BLOP_LOG_LEVEL2(BLOP_LOG_FCOLOR(BLOP_LOG_FTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
#define BLOP_BOUNDS(idx, bound)     do { BLOP_LOG_LEVEL2(BLOP_LOG_FCOLOR(BLOP_LOG_BTITLE) "\n Context: %s:%u (%s)\n Message: Index (%zu) out of bounds (%zu)\n\n", BLOP_FILE_PATH, __LINE__, __func__, idx, bound); } while (0)

#define BLOP_LOGF(format, ...)      do { char __blop_log_buffer[1024] = {0}; snprintf(__blop_log_buffer, 1024, format, __VA_ARGS__); BLOP_LOG_LEVEL1(BLOP_LOG_LCOLOR(BLOP_LOG_LTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, __blop_log_buffer); } while (0)            
#define BLOP_DEBUGF(format, ...)    do { char __blop_log_buffer[1024] = {0}; snprintf(__blop_log_buffer, 1024, format, __VA_ARGS__); BLOP_LOG_LEVEL1(BLOP_LOG_DCOLOR(BLOP_LOG_DTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, __blop_log_buffer); } while (0)      
#define BLOP_SUCCESSF(format, ...)  do { char __blop_log_buffer[1024] = {0}; snprintf(__blop_log_buffer, 1024, format, __VA_ARGS__); BLOP_LOG_LEVEL1(BLOP_LOG_SCOLOR(BLOP_LOG_STITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, __blop_log_buffer); } while (0)
#define BLOP_WARNINGF(format, ...)  do { char __blop_log_buffer[1024] = {0}; snprintf(__blop_log_buffer, 1024, format, __VA_ARGS__); BLOP_LOG_LEVEL2(BLOP_LOG_WCOLOR(BLOP_LOG_WTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, __blop_log_buffer); } while (0)
#define BLOP_ERRORF(format, ...)    do { char __blop_log_buffer[1024] = {0}; snprintf(__blop_log_buffer, 1024, format, __VA_ARGS__); BLOP_LOG_LEVEL2(BLOP_LOG_ECOLOR(BLOP_LOG_ETITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, __blop_log_buffer); } while (0)
#define BLOP_FATALF(format, ...)    do { char __blop_log_buffer[1024] = {0}; snprintf(__blop_log_buffer, 1024, format, __VA_ARGS__); BLOP_LOG_LEVEL2(BLOP_LOG_FCOLOR(BLOP_LOG_FTITLE) "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, __blop_log_buffer); } while (0)

#define BLOP_INTERNAL_LOG(msg)          ((void)0)            
#define BLOP_INTERNAL_DEBUG(msg)        ((void)0)      
#define BLOP_INTERNAL_SUCCESS(msg)      ((void)0)
#define BLOP_INTERNAL_WARNING(msg)      ((void)0)
#define BLOP_INTERNAL_ERROR(msg)        ((void)0)
#define BLOP_INTERNAL_FATAL(msg)        do { BLOP_LOG_LEVEL2(BLOP_LOG_FCOLOR("[BLOP FATAL]")  "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
#define BLOP_INTERNAL_BOUNDS(idx, bound)do { BLOP_LOG_LEVEL2(BLOP_LOG_FCOLOR("[BLOP FATAL]")  "\n Context: %s:%u (%s)\n Message: Index (%zu) out of bounds (%zu)\n\n", BLOP_FILE_PATH, __LINE__, __func__, idx, bound); } while (0)

#ifdef BLOP_ENABLE_INTERNAL_LOGS
  #undef  BLOP_INTERNAL_LOG
  #undef  BLOP_INTERNAL_DEBUG
  #undef  BLOP_INTERNAL_SUCCESS
  #undef  BLOP_INTERNAL_WARNING
  #undef  BLOP_INTERNAL_ERROR

  #define BLOP_INTERNAL_LOG(msg)        do { BLOP_LOG_LEVEL1(BLOP_LOG_LCOLOR("[BLOP LOG]")    "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)            
  #define BLOP_INTERNAL_DEBUG(msg)      do { BLOP_LOG_LEVEL1(BLOP_LOG_DCOLOR("[BLOP DEBUG]")  "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)      
  #define BLOP_INTERNAL_SUCCESS(msg)    do { BLOP_LOG_LEVEL1(BLOP_LOG_SCOLOR("[BLOP SUCCESS]")"\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
  #define BLOP_INTERNAL_WARNING(msg)    do { BLOP_LOG_LEVEL2(BLOP_LOG_WCOLOR("[BLOP WARNING]")"\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
  #define BLOP_INTERNAL_ERROR(msg)      do { BLOP_LOG_LEVEL2(BLOP_LOG_ECOLOR("[BLOP ERROR]")  "\n Context: %s:%u (%s)\n Message: %s\n\n", BLOP_FILE_PATH, __LINE__, __func__, msg); } while (0)
#endif

#ifdef BLOP_LOG_DISABLE_ALL
  #undef BLOP_LOG       
  #undef BLOP_DEBUG     
  #undef BLOP_SUCCESS   
  #undef BLOP_WARNING   
  #undef BLOP_ERROR     
  #undef BLOP_FATAL     
  #undef BLOP_BOUNDS  
  #undef BLOP_LOGF       
  #undef BLOP_DEBUGF     
  #undef BLOP_SUCCESSF   
  #undef BLOP_WARNINGF   
  #undef BLOP_ERRORF     
  #undef BLOP_FATALF     

  #define BLOP_LOG(msg)               ((void)0)
  #define BLOP_DEBUG(msg)             ((void)0)
  #define BLOP_SUCCESS(msg)           ((void)0)
  #define BLOP_WARNING(msg)           ((void)0)
  #define BLOP_ERROR(msg)             ((void)0)
  #define BLOP_FATAL(msg)             ((void)0)
  #define BLOP_BOUNDS(idx, bound)     ((void)0)
  #define BLOP_LOGF(format, ...)      ((void)0)
  #define BLOP_DEBUGF(format, ...)    ((void)0)
  #define BLOP_SUCCESSF(format, ...)  ((void)0)
  #define BLOP_WARNINGF(format, ...)  ((void)0)
  #define BLOP_ERRORF(format, ...)    ((void)0)
  #define BLOP_FATALF(format, ...)    ((void)0)

#else
  #ifdef BLOP_LOG_DISABLE_LOG
    #undef  BLOP_LOG
    #undef  BLOP_LOGF
    #define BLOP_LOG(msg)             ((void)0)
    #define BLOP_LOGF(format, ...)    ((void)0)
  #endif
  #ifdef BLOP_LOG_DISABLE_DEBUG
    #undef  BLOP_DEBUG
    #undef  BLOP_DEBUGF
    #define BLOP_DEBUG(msg)           ((void)0)
    #define BLOP_DEBUGF(format, ...)  ((void)0)
  #endif
  #ifdef BLOP_LOG_DISABLE_SUCCESS
    #undef  BLOP_SUCCESS
    #undef  BLOP_SUCCESSF
    #define BLOP_SUCCESS(msg)         ((void)0)
    #define BLOP_SUCCESSF(format, ...)((void)0)
  #endif
  #ifdef BLOP_LOG_DISABLE_WARNING
    #undef  BLOP_WARNING
    #undef  BLOP_WARNINGF
    #define BLOP_WARNING(msg)         ((void)0)
    #define BLOP_WARNINGF(format, ...)((void)0)
  #endif
  #ifdef BLOP_LOG_DISABLE_ERROR
    #undef  BLOP_ERROR
    #undef  BLOP_ERRORF
    #define BLOP_ERROR(msg)           ((void)0)
    #define BLOP_ERRORF(format, ...)  ((void)0)
  #endif
  #ifdef BLOP_LOG_DISABLE_FATAL
    #undef  BLOP_FATAL
    #undef  BLOP_FATALF
    #define BLOP_FATAL(msg)           ((void)0)
    #define BLOP_FATALF(format, ...)  ((void)0)
  #endif
  #ifdef BLOP_LOG_DISABLE_BOUNDS
    #undef  BLOP_BOUNDS
    #define BLOP_BOUNDS(idx, bound)   ((void)0)
  #endif
#endif

#ifdef BLOP_LOG_SHORT
  #define LOG(msg)                  BLOP_LOG(msg)       
  #define LOG_DEBUG(msg)            BLOP_DEBUG(msg)     
  #define LOG_SUCCESS(msg)          BLOP_SUCCESS(msg)   
  #define LOG_WARNING(msg)          BLOP_WARNING(msg)   
  #define LOG_ERROR(msg)            BLOP_ERROR(msg)     
  #define LOG_FATAL(msg)            BLOP_FATAL(msg)
  #define LOG_BOUNDS(idx, bound)    BLOP_BOUNDS(idx, bound)
  #define LOGF(format, ...)         BLOP_LOGF(format, __VA_ARGS__)       
  #define LOG_DEBUGF(format, ...)   BLOP_DEBUGF(format, __VA_ARGS__)     
  #define LOG_SUCCESSF(format, ...) BLOP_SUCCESSF(format, __VA_ARGS__)   
  #define LOG_WARNINGF(format, ...) BLOP_WARNINGF(format, __VA_ARGS__)   
  #define LOG_ERRORF(format, ...)   BLOP_ERRORF(format, __VA_ARGS__)     
  #define LOG_FATALF(format, ...)   BLOP_FATALF(format, __VA_ARGS__)
#endif

/* --------------------------------------------------------------------------
 * ASSERT SYSTEM
 * -------------------------------------------------------------------------- */

BLOP_NORETURN void BLOP_ABORT();

//! Enable Empty Popping
#ifndef BLOP_ENABLE_EMPTY_POPPING
  #define BLOP_EMPTY_POPPING()                        do { BLOP_INTERNAL_FATAL("To enable empty popping and avoid this error define BLOP_ENABLE_EMPTY_POPPING");            BLOP_ABORT(); } while(0)
#else
  #define BLOP_EMPTY_POPPING() ((void)0)
#endif /* BLOP_ENABLE_EMPTY_POPPING */
//! Enable Empty Popping

#define BLOP_ASSERT(cnd, msg)                         do { if (!(cnd))            { BLOP_FATAL(msg);                                                                        BLOP_ABORT(); } } while(0)
#define BLOP_ASSERT_PTR(ptr)                          do { if ((ptr) == NULL)     { BLOP_FATAL(#ptr " is NULL");                                                            BLOP_ABORT(); } } while(0)
#define BLOP_ASSERT_BOUNDS(idx, bound)                do { if ((idx) >= (bound))  { BLOP_BOUNDS(idx, bound);                                                                BLOP_ABORT(); } } while(0)
#define BLOP_ASSERT_FORCED(cnd, msg)                  do { if (!(cnd))            { BLOP_FATAL(msg);                                                                        BLOP_ABORT(); } } while(0)
#define BLOP_ASSERT_MALLOC(ptr, type, size)           do { if (ptr == NULL)       { BLOP_FATAL("Malloc failed for "  #ptr " of type " #type " and size " #size);            BLOP_ABORT(); } } while(0)
#define BLOP_ASSERT_CALLOC(ptr, type, count)          do { if (ptr == NULL)       { BLOP_FATAL("Calloc failed for "  #ptr " of type " #type " and count " #count);          BLOP_ABORT(); } } while(0)
#define BLOP_ASSERT_REALLOC(ptr, type, size)          do { if (ptr == NULL)       { BLOP_FATAL("Realloc failed for " #ptr " of type " #type " and size " #size);            BLOP_ABORT(); } } while(0)

#define BLOP_INTERNAL_ASSERT(cnd, msg)                do { if (!(cnd))            { BLOP_INTERNAL_FATAL(msg);                                                               BLOP_ABORT(); } } while(0)
#define BLOP_INTERNAL_ASSERT_PTR(ptr)                 do { if ((ptr) == NULL)     { BLOP_INTERNAL_FATAL(#ptr " parameter is NULL");                                         BLOP_ABORT(); } } while(0)
#define BLOP_INTERNAL_ASSERT_BOUNDS(idx, bound)       do { if ((idx) >= (bound))  { BLOP_INTERNAL_BOUNDS(idx, bound);                                                       BLOP_ABORT(); } } while(0)
#define BLOP_INTERNAL_ASSERT_FORCED(cnd, msg)         do { if (!(cnd))            { BLOP_INTERNAL_FATAL(msg);                                                               BLOP_ABORT(); } } while(0)
#define BLOP_INTERNAL_ASSERT_MALLOC(ptr, type, size)  do { if ((ptr) == NULL)     { BLOP_INTERNAL_FATAL("Malloc failed for "  #ptr " of type " #type " and size " #size);   BLOP_ABORT(); } } while(0)
#define BLOP_INTERNAL_ASSERT_CALLOC(ptr, type, count) do { if ((ptr) == NULL)     { BLOP_INTERNAL_FATAL("Calloc failed for "  #ptr " of type " #type " and count " #count); BLOP_ABORT(); } } while(0)
#define BLOP_INTERNAL_ASSERT_REALLOC(ptr, type, size) do { if ((ptr) == NULL)     { BLOP_INTERNAL_FATAL("Realloc failed for " #ptr " of type " #type " and size " #size);   BLOP_ABORT(); } } while(0)

#ifdef BLOP_DISABLE_ASSERTIONS
  #undef BLOP_INTERNAL_ASSERT
  #undef BLOP_INTERNAL_ASSERT_PTR

  #define BLOP_INTERNAL_ASSERT(cnd, msg)  ((void)0)
  #define BLOP_INTERNAL_ASSERT_PTR(ptr)   ((void)0)
#endif /* BLOP_DISABLE_ASSERTIONS */
  
//! Disable Assertions
#ifdef BLOP_ASSERT_DISABLE_ALL
  #undef  BLOP_ASSERT
  #undef  BLOP_ASSERT_PTR
  #undef  BLOP_ASSERT_BOUNDS

  #define BLOP_ASSERT(cnd, msg)           ((void)0)
  #define BLOP_ASSERT_PTR(ptr)            ((void)0)
  #define BLOP_ASSERT_BOUNDS(idx, bond)   ((void)0)
#else 
  #ifdef BLOP_ASSERT_DISABLE_ASSERT
    #undef  BLOP_ASSERT
    #define BLOP_ASSERT(cnd, msg)         ((void)0)
  #endif
  #ifdef BLOP_ASSERT_DISABLE_PTR
    #undef  BLOP_ASSERT_PTR
    #define BLOP_ASSERT_PTR(ptr)          ((void)0)
  #endif
  #ifdef BLOP_ASSERT_DISABLE_BOUNDS
    #undef  BLOP_ASSERT_BOUNDS
    #define BLOP_ASSERT_BOUNDS(idx, bound)((void)0)
  #endif
#endif /* BLOP_ASSERT_DISABLE_ALL */
//! Disable Assertions

#ifdef BLOP_ASSERT_SHORT
  #define ASSERT(cnd, msg)          BLOP_ASSERT(cnd, msg)
  #define ASSERT_PTR(ptr)           BLOP_ASSERT_PTR(ptr)
  #define ASSERT_BOUNDS(idx, bound) BLOP_ASSERT_BOUNDS(idx, bound)
  #define ASSERT_FORCED(cnd, msg)   BLOP_ASSERT_FORCED(cnd, msg)
#endif /* BLOP_ASSERT_SHORT */

/* --------------------------------------------------------------------------
 * WRAPPERS
 * -------------------------------------------------------------------------- */

#define BLOP_CONCAT2_IMPL(a, b)     a##b
#define BLOP_CONCAT3_IMPL(a, b, c)  a##b##c

#define BLOP_CONCAT2(a, b)          BLOP_CONCAT2_IMPL(a, b)
#define BLOP_CONCAT3(a, b, c)       BLOP_CONCAT3_IMPL(a, b, c)

#define BLOP_FREE(ptr)                   do { free ((void*)(ptr)); ptr = NULL; } while(0)
#define BLOP_MALLOC(v, type, size)       do { (v) = (type*)malloc((size));                BLOP_INTERNAL_ASSERT_MALLOC((v), type, (size)); memset(v, 0, (size)); } while(0)
#define BLOP_CALLOC(v, type, count)      do { (v) = (type*)calloc((count), sizeof(type)); BLOP_INTERNAL_ASSERT_CALLOC((v), type, (count));                      } while(0)
#define BLOP_REALLOC(v, type, ptr, size) do { (v) = (type*)realloc((void*)(ptr), (size)); BLOP_INTERNAL_ASSERT_REALLOC((v), type, (size));                      } while(0)

#define CAST(type, tocast) ((type)(tocast))
#define TERNARY(cnd, x, y) ((cnd) ? (x) : (y))

#define PTR_ADD(ptr, value) ((void*)((uint8_t*)(ptr) + (value)))
#define PTR_SUB(ptr, value) ((void*)((uint8_t*)(ptr) - (value)))

#define MIN(x, y)     TERNARY(x > y, y, x)
#define MAX(x, y)          TERNARY(x > y, x, y)
#define DISTANCE(x, y)    (TERNARY(x > y, x - y, y - x))
#define BLOP_DIFFERENCE(x, y)  ((ptrdiff_t)(x) - (ptrdiff_t)(y))

/* --------------------------------------------------------------------------
 * DOXYGEN SETTINGS
 * -------------------------------------------------------------------------- */

#ifdef _DOXYGEN_
  #define BLOP_ABORT_ON_ERROR
  #define BLOP_ENABLE_SAFETY
  #define BLOP_ENABLE_THREADSAFE
  #define BLOP_ENABLE_EMPTY_POPPING
  #define BLOP_DISABLE_ASSERTIONS

  #undef  BLOP_ABORT_ON_ERROR
  #undef  BLOP_ENABLE_SAFETY
  #undef  BLOP_ENABLE_THREADSAFE
  #undef  BLOP_ENABLE_EMPTY_POPPING
  #undef  BLOP_DISABLE_ASSERTIONS
#endif

/* --------------------------------------------------------------------------
 * IMPLEMENTATION
 * -------------------------------------------------------------------------- */

#ifdef BLOP_IMPLEMENTATION
  
/* --------------------------------------------------------------------------
 * ANSI SYSTEM
 * -------------------------------------------------------------------------- */

#ifdef BLOP_WINDOWS
#include <windows.h>

void  blop_impl_ansi_enable() {
  HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  BLOP_INTERNAL_ASSERT_FORCED(err != INVALID_HANDLE_VALUE, "Failed to get stderr handle (windows.h)");
  BLOP_INTERNAL_ASSERT_FORCED(out != INVALID_HANDLE_VALUE, "Failed to get stdout handle (windows.h)");

  DWORD err_mode = 0;
  DWORD out_mode = 0;
  BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(err, &err_mode), "Failed to get console err_mode (windows.h)");
  BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(out, &out_mode), "Failed to get console out_mode (windows.h)");

  if (!(err_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
    DWORD err_nmode = err_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
    BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(err, err_nmode) != 0, "Failed to set console err_mode (windows.h)");
  }

  if (!(out_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
    DWORD out_nmode = out_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
    BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(out, out_nmode) != 0, "Failed to set console out_mode (windows.h)");
  }
}
void  blop_impl_ansi_disable() {
  HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  BLOP_INTERNAL_ASSERT_FORCED(err != INVALID_HANDLE_VALUE, "Failed to get std handle (windows.h)");
  BLOP_INTERNAL_ASSERT_FORCED(out != INVALID_HANDLE_VALUE, "Failed to get std handle (windows.h)");

  DWORD err_mode = 0;
  DWORD out_mode = 0;
  BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(err, &err_mode), "Failed to get console mode (windows.h)");
  BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(out, &out_mode), "Failed to get console mode (windows.h)");

  DWORD err_nmode = err_mode & ~(ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
  DWORD out_nmode = out_mode & ~(ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
  
  BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(err, err_nmode) != 0, "Failed to set console mode (windows.h)");
  BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(out, out_nmode) != 0, "Failed to set console mode (windows.h)");
}

#endif /* BLOP_WINDOWS */

char* blop_ansi_fg_256        (char* buffer, uint8_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "38;5;%hhum", n);
  return buffer;
}
char* blop_ansi_bg_256        (char* buffer, uint8_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "48;5;%hhum", n);
  return buffer;
}
char* blop_ansi_fg_rgb        (char* buffer, uint8_t r, uint8_t g, uint8_t b) {
  sprintf(buffer, BLOP_ANSI_ESC "38;2;%hhu;%hhu;%hhum", r, g, b);
  return buffer;
}
char* blop_ansi_bg_rgb        (char* buffer, uint8_t r, uint8_t g, uint8_t b) {
  sprintf(buffer, BLOP_ANSI_ESC "48;2;%hhu;%hhu;%hhum", r, g, b);
  return buffer;
}

char* blop_ansi_cursor_up     (char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huA", n);
  return buffer;
}
char* blop_ansi_cursor_down   (char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huB", n);
  return buffer;
}
char* blop_ansi_cursor_forward(char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huC", n);
  return buffer;
}
char* blop_ansi_cursor_back   (char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huD", n);
  return buffer;
}
char* blop_ansi_cursor_pos    (char* buffer, uint16_t x, uint16_t y) {
  sprintf(buffer, BLOP_ANSI_ESC "%hu;%huH", y, x);
  return buffer;
}

/* --------------------------------------------------------------------------
 * ASSERT SYSTEM
 * -------------------------------------------------------------------------- */

BLOP_NORETURN void BLOP_ABORT() {
  #ifndef BLOP_ABORT_ON_ERROR
    exit(-1);
  #else
    abort();
  #endif
}

#endif

#endif /* __BLOP_H__ */
