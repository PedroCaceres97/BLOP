#ifndef __BLOP_SRING_H__
#define __BLOP_SRING_H__

#include <blop/blop.h>

#ifndef BLOP_STRING_NAME
  #define BLOP_STRING_NAME blop_string
#endif /* BLOP_STRING_NAME */

#ifndef BLOP_STRING_RESIZE_POLICIE
  #define BLOP_STRING_RESIZE_POLICIE(size) (size * 2)
#endif /* BLOP_STRING_RESIZE_POLICIE */

#ifndef BLOP_STRING_SHRINK_POLICIE
  #define BLOP_STRING_SHRINK_POLICIE(capacity) (capacity / 4)
#endif

#if !defined(BLOP_STRING_INITIAL_SIZE) || BLOP_STRING_INITIAL_SIZE <= 0
  #define BLOP_STRING_INITIAL_SIZE 10
#endif /* BLOP_STRING_INITIAL_SIZE */

/** @cond doxygen_ignore */
#define blopt_string                BLOP_STRING_NAME
#define blops_string                BLOP_CONCAT2(blopt_string, _t)

#define blopfn_string_create        BLOP_CONCAT2(blopt_string, _create)
#define blopfn_string_destroy       BLOP_CONCAT2(blopt_string, _destroy)

#define blopfn_string_rdlock        BLOP_CONCAT2(blopt_string, _rdlock)
#define blopfn_string_wrlock        BLOP_CONCAT2(blopt_string, _wrlock)
#define blopfn_string_rdunlock      BLOP_CONCAT2(blopt_string, _rdunlock)
#define blopfn_string_wrunlock      BLOP_CONCAT2(blopt_string, _wrunlock)

#define blopfn_string_cstr          BLOP_CONCAT2(blopt_string, _cstr)
#define blopfn_string_size          BLOP_CONCAT2(blopt_string, _size)

#define blopfn_string_set           BLOP_CONCAT2(blopt_string, _set)
#define blopfn_string_get           BLOP_CONCAT2(blopt_string, _get)
#define blopfn_string_resize        BLOP_CONCAT2(blopt_string, _resize)
#define blopfn_string_shrink        BLOP_CONCAT2(blopt_string, _shrink)

#define blopfn_string_clear         BLOP_CONCAT2(blopt_string, _clear)
#define blopfn_string_erase         BLOP_CONCAT2(blopt_string, _erase)
#define blopfn_string_pop_back      BLOP_CONCAT2(blopt_string, _pop_back)
#define blopfn_string_pop_front     BLOP_CONCAT2(blopt_string, _pop_front)

#define blopfn_string_insert        BLOP_CONCAT2(blopt_string, _insert)
#define blopfn_string_push_back     BLOP_CONCAT2(blopt_string, _push_back)
#define blopfn_string_push_front    BLOP_CONCAT2(blopt_string, _push_front)

#define blopfn_string_strcpy        BLOP_CONCAT2(blopt_string, _strcpy)
/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_string* blopt_string;

blopt_string  blopfn_string_create     (blopt_string str);
void          blopfn_string_destroy    (blopt_string str);

void          blopfn_string_rdlock     (blopt_string str);
void          blopfn_string_wrlock     (blopt_string str);
void          blopfn_string_rdunlock   (blopt_string str);
void          blopfn_string_wrunlock   (blopt_string str);

char*         blopfn_string_cstr       (blopt_string str);
size_t        blopfn_string_size       (blopt_string str);

void          blopfn_string_set        (blopt_string str, size_t idx, char c);
char          blopfn_string_get        (blopt_string str, size_t idx);
void          blopfn_string_resize     (blopt_string str, size_t size);
void          blopfn_string_shrink     (blopt_string str);

void          blopfn_string_clear      (blopt_string str);
void          blopfn_string_erase      (blopt_string str, size_t idx);
void          blopfn_string_pop_back   (blopt_string str);
void          blopfn_string_pop_front  (blopt_string str);

void          blopfn_string_insert     (blopt_string str, size_t idx, char c);
void          blopfn_string_push_back  (blopt_string str, char c);
void          blopfn_string_push_front (blopt_string str, char c);

void          blopfn_string_strcpy     (blopt_string str, size_t idx, const char* src, size_t count);

#if (defined(BLOP_STRING_STRUCT) || defined(BLOP_STRING_IMPLEMENTATION)) && !defined(BLOP_STRING_NOT_STRUCT)
typedef struct blops_string {
  int               allocated;
  char*             data;
  size_t            size;
  size_t            capacity;
  BLOP_RWLOCK_TYPE  lock;
} blops_string;
#endif

#ifdef BLOP_STRING_IMPLEMENTATION

blopt_string  blopfn_string_create(blopt_string str) {
  if (!str) {
    BLOP_CALLOC(str, struct blops_string, 1);
    str->allocated = true;
  } else {
    str->allocated = false;
  }

  str->size = 0;
  str->capacity = BLOP_STRING_INITIAL_SIZE;
  BLOP_RWLOCK_INIT(str->lock);
  BLOP_CALLOC(str->data, char, str->capacity + 1);

  return str;
}
void          blopfn_string_destroy(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_FREE(str->data);
  BLOP_RWLOCK_DESTROY(str->lock);

  if (str->allocated) {
    BLOP_FREE(str);
  }
}

void          blopfn_string_rdlock(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_RDLOCK(str->lock);
}
void          blopfn_string_wrlock(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_WRLOCK(str->lock);
}
void          blopfn_string_rdunlock(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_RDUNLOCK(str->lock);
}
void          blopfn_string_wrunlock(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_WRUNLOCK(str->lock);
}

char*         blopfn_string_cstr(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  return str->data;
}
size_t        blopfn_string_size(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  return str->size;
}

void          blopfn_string_set(blopt_string str, size_t idx, char c) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size);

  str->data[idx] = c;
}
char          blopfn_string_get(blopt_string str, size_t idx) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size);

  return str->data[idx];
}
void          blopfn_string_resize(blopt_string str, size_t size) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  if (size == str->size) {
    return;
  }

  if (size == 0) {
    blopfn_string_clear(str);
    return;
  }

  str->capacity = TERNARY(
    size < BLOP_STRING_INITIAL_SIZE,
    BLOP_STRING_INITIAL_SIZE,
    BLOP_STRING_RESIZE_POLICIE(size)
  );

  char* data = NULL;
  BLOP_CALLOC(data, char, str->capacity + 1);
  memcpy(data, str->data, MIN(str->size, size));

  BLOP_FREE(str->data);
  str->data = data;
  str->size = size;
  str->data[str->size] = '\0';
}
void          blopfn_string_shrink(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  if (str->size < BLOP_STRING_SHRINK_POLICIE(str->capacity) && str->size < BLOP_STRING_INITIAL_SIZE) {
    str->capacity = BLOP_STRING_RESIZE_POLICIE(str->size);

    char* data = NULL;
    BLOP_CALLOC(data, char, str->capacity + 1);
    memcpy(data, str->data, str->size);

    BLOP_FREE(str->data);
    str->data = data;
    str->data[str->size] = '\0';
  }
}

void          blopfn_string_clear(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  str->size = 0;
  str->capacity = BLOP_STRING_INITIAL_SIZE;
  BLOP_FREE(str->data);
  BLOP_CALLOC(str->data, char, str->capacity + 1);
}
void          blopfn_string_erase(blopt_string str, size_t idx) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size);      

  if (idx != str->size - 1) {
    memmove(&str->data[idx], &str->data[idx + 1], (str->size - idx - 1));
  }
  
  str->size--;
  str->data[str->size] = '\0';
  blopfn_string_shrink(str);
}
void          blopfn_string_pop_back(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  if (str->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  blopfn_string_erase(str, str->size - 1);
}
void          blopfn_string_pop_front(blopt_string str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  if (str->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  blopfn_string_erase(str, 0);
}

void          blopfn_string_insert(blopt_string str, size_t idx, char c) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size + 1);

  if (str->size != str->capacity) {
    if (idx != str->size) {
      memmove(&str->data[idx + 1], &str->data[idx], (str->size - idx));
    }
  } else {
    str->capacity = BLOP_STRING_RESIZE_POLICIE(str->size);
    char* data = NULL;
    BLOP_CALLOC(data, char, str->capacity + 1);

    if (idx != 0) {
      memcpy(data, str->data, idx);
    }
    
    if (idx != str->size) {
      memcpy(&data[idx + 1], &str->data[idx], (str->size - idx));
    }

    BLOP_FREE(str->data);
    str->data = data;
  }

  str->size++;
  str->data[idx] = c;
  str->data[str->size] = '\0';
}
void          blopfn_string_push_back(blopt_string str, char c) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  blopfn_string_insert(str, str->size, c);
}
void          blopfn_string_push_front(blopt_string str, char c) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  blopfn_string_insert(str, 0, c);
}

void          blopfn_string_strcpy(blopt_string str, size_t idx, const char* src, size_t count) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_INTERNAL_ASSERT_PTR(src);

  if (count == 0) { return; }

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size);
  BLOP_INTERNAL_ASSERT_BOUNDS(idx + count, str->size + 1);

  memcpy(&str->data[idx], src, count);
}

#endif /* BLOP_STRING_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#endif /* __BLOP_SRING_H__ */