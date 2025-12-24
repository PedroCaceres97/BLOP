#include <blop/blop.h>

#ifndef BLOP_POOL_NAME
  #define BLOP_POOL_NAME poolb
#endif /* BLOP_POOL_NAME */

#define blopt_pool              BLOP_POOL_NAME
#define blopt_ptrhdr            BLOP_CONCAT2(blopt_pool, _ptrhdr)

#define blops_pool              BLOP_CONCAT3(_, blopt_pool, _t)
#define blops_ptrhdr            BLOP_CONCAT3(_, blopt_pool, _ptrhdr_t)

#define blopfn_pool_create      BLOP_CONCAT2(blopt_pool, _create)
#define blopfn_pool_destroy     BLOP_CONCAT2(blopt_pool, _destroy)

#define blopfn_pool_rdlock      BLOP_CONCAT2(blopt_pool, _rdlock)
#define blopfn_pool_wrlock      BLOP_CONCAT2(blopt_pool, _wrlock)
#define blopfn_pool_rdunlock    BLOP_CONCAT2(blopt_pool, _rdunlock)
#define blopfn_pool_wrunlock    BLOP_CONCAT2(blopt_pool, _wrunlock)

#define blopfn_pool_free        BLOP_CONCAT2(blopt_pool, _free)
#define blopfn_pool_free_all    BLOP_CONCAT2(blopt_pool, _free_all)
#define blopfn_pool_malloc      BLOP_CONCAT2(blopt_pool, _malloc)
#define blopfn_pool_calloc      BLOP_CONCAT2(blopt_pool, _calloc)
#define blopfn_pool_realloc     BLOP_CONCAT2(blopt_pool, _realloc)
#define blopfn_pool_duplicate   BLOP_CONCAT2(blopt_pool, _duplicate)

#define blopfn_pool_print       BLOP_CONCAT2(blopt_pool, _print)
#define blopfn_pool_bytes       BLOP_CONCAT2(blopt_pool, _bytes)
#define blopfn_pool_count       BLOP_CONCAT2(blopt_pool, _count)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_pool*      blopt_pool;
typedef struct blops_ptrhdr*    blopt_ptrhdr;

blopt_pool  blopfn_pool_create      (blop_context context);
void        blopfn_pool_destroy     (blopt_pool pool);

void        blopfn_pool_rdlock      (blopt_pool pool);
void        blopfn_pool_wrlock      (blopt_pool pool);
void        blopfn_pool_rdunlock    (blopt_pool pool);
void        blopfn_pool_wrunlock    (blopt_pool pool);

void        blopfn_pool_free        (blopt_pool pool, void* ptr);
void        blopfn_pool_free_all    (blopt_pool pool);
void*       blopfn_pool_malloc      (blopt_pool pool, size_t size, blop_context context);
void*       blopfn_pool_calloc      (blopt_pool pool, size_t count, size_t size, blop_context context);
void*       blopfn_pool_realloc     (blopt_pool pool, void* ptr, size_t size, blop_context context);
void*       blopfn_pool_duplicate   (blopt_pool pool, void* ptr, size_t size, blop_context context);

void        blopfn_pool_print       (blopt_pool pool);
size_t      blopfn_pool_bytes       (blopt_pool pool);
size_t      blopfn_pool_count       (blopt_pool pool);

#if (defined(BLOP_POOL_STRUCT) || defined(BLOP_POOL_IMPLEMENTATION)) && !defined(BLOP_POOL_NOT_STRUCT)
#define blopt_plist               BLOP_CONCAT2(blopt_pool, _list)
#define blopt_pnode               BLOP_CONCAT2(blopt_plist, _node)

#define blops_plist               BLOP_CONCAT3(_, blopt_plist, _t)
#define blops_pnode               BLOP_CONCAT3(_, blopt_pnode, _t)

#define BLOP_LIST_NAME            blopt_plist
#define BLOP_LIST_DATA_TYPE       blopt_ptrhdr
#define BLOP_LIST_STRUCT
#include <blop/list.h>

struct blops_ptrhdr {
  blopt_pool          pool;
  struct blops_pnode  node;

  size_t              size;
  blop_context        context;
};
struct blops_pool {
  BLOP_RWLOCK_TYPE    lock;
  struct blops_plist  ptrs;
  
  size_t              bytes;
  blop_context        context;
};
#endif

#ifdef BLOP_POOL_IMPLEMENTATION

#ifdef BLOP_POOL_NOT_STRUCT
  #define blopt_plist               BLOP_CONCAT2(blopt_pool, _list)
  #define blopt_pnode               BLOP_CONCAT2(blopt_plist, _node)

  #define blops_plist               BLOP_CONCAT3(_, blopt_plist, _t)
  #define blops_pnode               BLOP_CONCAT3(_, blopt_pnode, _t)
#endif

#define BLOP_LIST_NAME            blopt_plist
#define BLOP_LIST_DATA_TYPE       blopt_ptrhdr
#define BLOP_LIST_NOT_STRUCT
#define BLOP_LIST_IMPLEMENTATION
#include <blop/list.h>

#define blopfn_plist_create       BLOP_CONCAT2(blopt_plist, _create)
#define blopfn_plist_destroy      BLOP_CONCAT2(blopt_plist, _destroy)

#define blopfn_plist_erase        BLOP_CONCAT2(blopt_plist, _erase)
#define blopfn_plist_pop_front    BLOP_CONCAT2(blopt_plist, _pop_front)

#define blopfn_plist_push_back    BLOP_CONCAT2(blopt_plist, _push_back)
#define blopfn_plist_insert_next  BLOP_CONCAT2(blopt_plist, _insert_next)

#define blopfn_pnode_create       BLOP_CONCAT2(blopt_pnode, _create)
#define blopfn_pnode_destroy      BLOP_CONCAT2(blopt_pnode, _destroy)

#define POOL_PTR_TO_HDR(ptr) (blopt_ptrhdr)PTR_SUB(ptr, sizeof(struct blops_ptrhdr))
#define POOL_HDR_TO_PTR(hdr) (void*)PTR_ADD(hdr, sizeof(struct blops_ptrhdr))

blopt_pool  blopfn_pool_create(blop_context context) {
  blopt_pool pool = NULL;
  BLOP_CALLOC(pool, struct blops_pool, 1);

  BLOP_RWLOCK_INIT(pool->lock);
  blopfn_plist_create(&pool->ptrs);

  pool->bytes   = 0;
  pool->context = context;

  return pool;
}
void        blopfn_pool_destroy(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  BLOP_INTERNAL_ASSERT(pool->ptrs.size == 0, "Destroying non empty pool (HINT: Call free all)");

  blopfn_plist_destroy(&pool->ptrs);
  BLOP_FREE(pool);
}

void        blopfn_pool_rdlock(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  BLOP_RWLOCK_RDLOCK(pool->lock);
}
void        blopfn_pool_wrlock(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  BLOP_RWLOCK_WRLOCK(pool->lock);
}
void        blopfn_pool_rdunlock(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  BLOP_RWLOCK_RDUNLOCK(pool->lock);
}
void        blopfn_pool_wrunlock(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  BLOP_RWLOCK_WRUNLOCK(pool->lock);
}

void        blopfn_pool_free(blopt_pool pool, void* ptr) {
  BLOP_INTERNAL_ASSERT_PTR(pool);
  BLOP_INTERNAL_ASSERT_PTR(ptr);

  blopt_ptrhdr hdr = POOL_PTR_TO_HDR(ptr);
  BLOP_INTERNAL_ASSERT(hdr->pool == pool, "Freeing a foreign ptr");

  blopfn_plist_erase(&pool->ptrs, &hdr->node, true);
  pool->bytes -= hdr->size;

  BLOP_FREE(hdr);
}
void        blopfn_pool_free_all(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  blopt_pnode current = pool->ptrs.front;
  while (current != NULL) {
    blopt_ptrhdr hdr = current->data;
    current = current->next;
    blopfn_plist_pop_front(&pool->ptrs, true);
    pool->bytes -= hdr->size;
    BLOP_FREE(hdr);
  }
}
void*       blopfn_pool_malloc(blopt_pool pool, size_t size, blop_context context) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  BLOP_INTERNAL_ASSERT(size != 0, "Requested allocation size is 0");

  blopt_ptrhdr hdr = NULL;
  BLOP_MALLOC(hdr, struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
  memset(hdr, 0, size + sizeof(struct blops_ptrhdr));

  blopfn_pnode_create(&hdr->node);
  hdr->pool       = pool;
  hdr->node.data  = hdr;
  hdr->size       = size;
  hdr->context    = context;
  blopfn_plist_push_back(&pool->ptrs, &hdr->node);

  pool->bytes += size;
  void*  ptr   = POOL_HDR_TO_PTR(hdr);
  return ptr;
}
void*       blopfn_pool_calloc(blopt_pool pool, size_t count, size_t size, blop_context context) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  BLOP_INTERNAL_ASSERT(size != 0, "Requested allocation size is 0");

  size *= count;
  blopt_ptrhdr hdr = NULL;
  BLOP_MALLOC(hdr, struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
  memset(hdr, 0, size + sizeof(struct blops_ptrhdr));

  blopfn_pnode_create(&hdr->node);
  hdr->context    = context;
  hdr->size       = size;
  hdr->pool       = pool;
  hdr->node.data  = hdr;
  blopfn_plist_push_back(&pool->ptrs, &hdr->node);

  pool->bytes += size;
  void*  ptr   = POOL_HDR_TO_PTR(hdr);
  return ptr;
}
void*       blopfn_pool_realloc(blopt_pool pool, void* ptr, size_t size, blop_context context) {
  BLOP_INTERNAL_ASSERT_PTR(pool);
  BLOP_INTERNAL_ASSERT_PTR(ptr);

  BLOP_INTERNAL_ASSERT(size != 0, "Requested allocation size is 0");

  blopt_ptrhdr srchdr = POOL_PTR_TO_HDR(ptr);
  BLOP_INTERNAL_ASSERT(srchdr->pool == pool, "Reallocating a foreign ptr");

  blopt_ptrhdr newhdr = NULL;
  BLOP_MALLOC(newhdr, struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));

  blopfn_pnode_create(&newhdr->node);
  newhdr->pool    = pool;
  newhdr->node.data = newhdr;
  newhdr->size    = size;
  newhdr->context = context;
  blopfn_plist_insert_next(&pool->ptrs, &srchdr->node, &newhdr->node);
  blopfn_plist_erase(&pool->ptrs, &srchdr->node, true);

  pool->bytes += size;
  void*  newptr = POOL_HDR_TO_PTR(newhdr);
  memcpy(newptr, ptr, MIN(size, srchdr->size));
  
  if (size > srchdr->size) {
    memset(PTR_ADD(newptr, srchdr->size), 0, size - srchdr->size);
  }

  pool->bytes -= srchdr->size;
  BLOP_FREE(srchdr);

  return newptr;
}
void*       blopfn_pool_duplicate(blopt_pool pool, void* ptr, size_t size, blop_context context) {
  BLOP_INTERNAL_ASSERT_PTR(pool);
  BLOP_INTERNAL_ASSERT_PTR(ptr);

  BLOP_INTERNAL_ASSERT(size != 0, "Requested allocation size is 0");

  blopt_ptrhdr srchdr = (blopt_ptrhdr)PTR_SUB(ptr, sizeof(struct blops_ptrhdr));
  BLOP_INTERNAL_ASSERT(srchdr->pool == pool, "Duplicating a foreign ptr");

  blopt_ptrhdr newhdr = NULL;
  BLOP_MALLOC(newhdr, struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
  memset(newhdr, 0, size + sizeof(struct blops_ptrhdr));

  blopfn_pnode_create(&newhdr->node);
  newhdr->context = context;
  newhdr->size    = size;
  newhdr->pool    = pool;
  newhdr->node.data = newhdr;
  blopfn_plist_insert_next(&pool->ptrs, &srchdr->node, &newhdr->node);

  pool->bytes += size;
  void*  newptr = PTR_ADD(newhdr, sizeof(struct blops_ptrhdr));
  memcpy(newptr, ptr, MIN(size, srchdr->size));
  return newptr;
}

void        blopfn_pool_print(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);

  printf("Pool Information:\n");
  printf(" Alias: %s\n", pool->context.alias);
  printf(" Origin context: %s:%u (%s)\n", pool->context.file, pool->context.line, pool->context.func);
  printf(" Allocated pointers: %zu\n", pool->ptrs.size);
  printf(" Total bytes allocated: %zu\n\n", pool->bytes);

  if (pool->bytes == 0) {
    return;
  }
  
  blopt_pnode current = pool->ptrs.front;
  printf("Individual Pointer Information\n");
  size_t index = 0;
  while (current != NULL) {
    blopt_ptrhdr hdr = current->data;
    printf(" Pointer [%zu]\n", index);
    printf("  Size: %zu bytes\n", hdr->size);
    printf("  Alias: %s\n", hdr->context.alias);
    printf("  Addres: %p\n", POOL_HDR_TO_PTR(hdr));
    printf("  Origin context: %s:%u (%s)\n\n", hdr->context.file, hdr->context.line, hdr->context.func);
    current = current->next;
    index++;
  }
}
size_t      blopfn_pool_bytes(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);
  return pool->bytes;
}
size_t      blopfn_pool_count(blopt_pool pool) {
  BLOP_INTERNAL_ASSERT_PTR(pool);
  return pool->ptrs.size;
}

#endif /* BLOP_POOL_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#undef BLOP_POOL_NAME

#undef BLOP_POOL_STRUCT
#undef BLOP_POOL_NOT_STRUCT
#undef BLOP_POOL_IMPLEMENTATION

#undef POOL_PTR_TO_HDR
#undef POOL_HDR_TO_PTR

#undef blopt_pool
#undef blopt_ptrhdr

#undef blops_pool
#undef blops_ptrhdr

#undef blopfn_pool_create
#undef blopfn_pool_destroy

#undef blopfn_pool_rdlock
#undef blopfn_pool_wrlock
#undef blopfn_pool_rdunlock
#undef blopfn_pool_wrunlock

#undef blopfn_pool_free
#undef blopfn_pool_free_all
#undef blopfn_pool_malloc
#undef blopfn_pool_calloc
#undef blopfn_pool_realloc
#undef blopfn_pool_duplicate

#undef blopfn_pool_print
#undef blopfn_pool_bytes
#undef blopfn_pool_count

#undef blopt_plist
#undef blopt_pnode

#undef blops_plist
#undef blops_pnode

#undef blopfn_plist_create
#undef blopfn_plist_destroy

#undef blopfn_plist_erase
#undef blopfn_plist_pop_front

#undef blopfn_plist_push_back
#undef blopfn_plist_insert_next

#undef blopfn_pnode_create
#undef blopfn_pnode_destroy