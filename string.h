#ifndef __BLOP_SRING_H__
#define __BLOP_SRING_H__

#include <blop/blop.h>

#ifndef BLOP_STRING_RESIZE_POLICIE
  #define BLOP_STRING_RESIZE_POLICIE(size) (size * 2)
#endif /* BLOP_STRING_RESIZE_POLICIE */

#ifndef BLOP_STRING_SHRINK_POLICIE
  #define BLOP_STRING_SHRINK_POLICIE(capacity) (capacity / 4)
#endif

#if !defined(BLOP_STRING_INITIAL_SIZE) || BLOP_STRING_INITIAL_SIZE <= 0
  #define BLOP_STRING_INITIAL_SIZE 10
#endif /* BLOP_STRING_INITIAL_SIZE */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct string string;

string* string_create     (string* str);
void    string_destroy    (string* str);

void    string_rdlock     (string* str);
void    string_wrlock     (string* str);
void    string_rdunlock   (string* str);
void    string_wrunlock   (string* str);

char*   string_cstr       (string* str);
size_t  string_size       (string* str);

void    string_set        (string* str, size_t idx, char c);
char    string_get        (string* str, size_t idx);
void    string_resize     (string* str, size_t size);
void    string_shrink     (string* str);

void    string_clear      (string* str);
void    string_erase      (string* str, size_t idx);
void    string_pop_back   (string* str);
void    string_pop_front  (string* str);

void    string_insert     (string* str, size_t idx, char c);
void    string_push_back  (string* str, char c);
void    string_push_front (string* str, char c);

void    string_strcpy     (string* str, size_t idx, const char* src, size_t count);

#if (defined(BLOP_STRING_STRUCT) || defined(BLOP_STRING_IMPLEMENTATION)) && !defined(BLOP_STRING_NOT_STRUCT)
struct string {
  int               allocated;
  char*             data;
  size_t            size;
  size_t            capacity;
  BLOP_RWLOCK_TYPE  lock;
};
#endif

#ifdef BLOP_STRING_IMPLEMENTATION

string* string_create(string* str) {
  if (!str) {
    BLOP_CALLOC(str, string, 1);
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
void    string_destroy(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_FREE(str->data);
  BLOP_RWLOCK_DESTROY(str->lock);

  if (str->allocated) {
    BLOP_FREE(str);
  }
}

void    string_rdlock(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_RDLOCK(str->lock);
}
void    string_wrlock(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_WRLOCK(str->lock);
}
void    string_rdunlock(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_RDUNLOCK(str->lock);
}
void    string_wrunlock(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  BLOP_RWLOCK_WRUNLOCK(str->lock);
}

char*   string_cstr(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  return str->data;
}
size_t  string_size(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);
  return str->size;
}

void    string_set(string* str, size_t idx, char c) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size);

  str->data[idx] = c;
}
char    string_get(string* str, size_t idx) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size);

  return str->data[idx];
}
void    string_resize(string* str, size_t size) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  if (size == str->size) {
    return;
  }

  if (size == 0) {
    string_clear(str);
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
void    string_shrink(string* str) {
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

void    string_clear(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  str->size = 0;
  str->capacity = BLOP_STRING_INITIAL_SIZE;
  BLOP_FREE(str->data);
  BLOP_CALLOC(str->data, char, str->capacity + 1);
}
void    string_erase(string* str, size_t idx) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, str->size);      

  if (idx != str->size - 1) {
    memmove(&str->data[idx], &str->data[idx + 1], (str->size - idx - 1));
  }
  
  str->size--;
  str->data[str->size] = '\0';
  string_shrink(str);
}
void    string_pop_back(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  if (str->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  string_erase(str, str->size - 1);
}
void    string_pop_front(string* str) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  if (str->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  string_erase(str, 0);
}

void    string_insert(string* str, size_t idx, char c) {
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
void    string_push_back(string* str, char c) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  string_insert(str, str->size, c);
}
void    string_push_front(string* str, char c) {
  BLOP_INTERNAL_ASSERT_PTR(str);

  string_insert(str, 0, c);
}

void    string_strcpy(string* str, size_t idx, const char* src, size_t count) {
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