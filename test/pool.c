/*
#define BLOP_LOG_SHORT
#define BLOP_LOG_COLOURED
#define BLOP_ENABLE_SAFETY
#define BLOP_POOL_NAME Pool
#define BLOP_POOL_IMPLEMENTATION
#include <blop/pool.h>

Pool create_pool() {
  return Pool_create(BLOP_CONTEXT("Test Pool"));
}

int main() {
  Pool pool = create_pool();
  LOG_SUCCESS("Pool created");

  char* ptr1 = Pool_malloc(pool, 64, BLOP_CONTEXT("PTR 1"));
  LOG_SUCCESS("Ptr allocated");
  char* ptr2 = Pool_calloc(pool, 2, 64, BLOP_CONTEXT("PTR 2"));
  LOG_SUCCESS("Ptr allocated");
  char* ptr3 = Pool_malloc(pool, 256, BLOP_CONTEXT("PTR 3"));
  LOG_SUCCESS("Ptr allocated");

  strcpy(ptr1, "Hola mundo!\n");
  strcpy(ptr2, "Hola blop pool sos lo mas!\n");
  strcpy(ptr3, "He aqui el mensaje mas largo de todos los testeos!\n");

  printf("%s", ptr1);
  printf("%s", ptr2);
  printf("%s", ptr3);

  char* ptr4 = Pool_duplicate(pool, ptr1, 64, BLOP_CONTEXT("PTR 4"));
  LOG_SUCCESS("Ptr dup");
  char* ptr5 = Pool_duplicate(pool, ptr2, 256, BLOP_CONTEXT("PTR 5"));
  LOG_SUCCESS("Ptr dup");

  Pool_print(pool);

  char* ptr6 = Pool_realloc(pool, ptr1, 20, BLOP_CONTEXT("PTR 6"));
  LOG_SUCCESS("Ptr realloc");

  printf("%s", ptr1);

  Pool_print(pool);

  Pool_free(pool, ptr2);
  LOG_SUCCESS("Ptr freed");
  Pool_free(pool, ptr3);
  LOG_SUCCESS("Ptr freed");

  Pool_print(pool);

  Pool_free_all(pool);
  LOG_SUCCESS("Ptr freed");

  Pool_print(pool);

  Pool_destroy(pool);
  LOG_SUCCESS("Pool destroyed");
}
*/