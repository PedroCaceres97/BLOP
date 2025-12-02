#include <blop/blop.h>

#ifndef BLOP_POOL_NAME
    #define BLOP_POOL_NAME poolb
#endif /* BLOP_POOL_NAME */

#define blopt_pool                  BLOP_POOL_NAME
#define blopt_ptrhdr                BLOP_CONCAT2(blopt_pool, _ptrhdr)

#define blops_pool                  BLOP_CONCAT3(_, blopt_pool, _t)
#define blops_ptrhdr                BLOP_CONCAT3(_, blopt_pool, _ptrhdr_t)

#define blopfn_pool_create          BLOP_CONCAT2(blopt_pool, _create)
#define blopfn_pool_destroy         BLOP_CONCAT2(blopt_pool, _destroy)

#define blopfn_pool_free            BLOP_CONCAT2(blopt_pool, _free)
#define blopfn_pool_free_all        BLOP_CONCAT2(blopt_pool, _free_all)
#define blopfn_pool_malloc          BLOP_CONCAT2(blopt_pool, _malloc)
#define blopfn_pool_calloc          BLOP_CONCAT2(blopt_pool, _calloc)
#define blopfn_pool_realloc         BLOP_CONCAT2(blopt_pool, _realloc)
#define blopfn_pool_duplicate       BLOP_CONCAT2(blopt_pool, _duplicate)

#define blopfn_pool_print           BLOP_CONCAT2(blopt_pool, _print)
#define blopfn_pool_get_bytes       BLOP_CONCAT2(blopt_pool, _get_bytes)
#define blopfn_pool_get_count       BLOP_CONCAT2(blopt_pool, _get_count)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_pool*      blopt_pool;
typedef struct blops_ptrhdr*    blopt_ptrhdr;

blopt_pool  blopfn_pool_create      ();
void        blopfn_pool_destroy     (blopt_pool pool);

void        blopfn_pool_free        (blopt_pool pool, void* ptr);
void        blopfn_pool_free_all    (blopt_pool pool);
void*       blopfn_pool_malloc      (blopt_pool pool, size_t size);
void*       blopfn_pool_calloc      (blopt_pool pool, size_t count, size_t size);
void*       blopfn_pool_realloc     (blopt_pool pool, void* ptr, size_t size);
void*       blopfn_pool_duplicate   (blopt_pool pool, void* ptr, size_t size);

void        blopfn_pool_print       (blopt_pool pool);
size_t      blopfn_pool_get_bytes   (blopt_pool pool);
size_t      blopfn_pool_get_count   (blopt_pool pool);

#ifdef BLOP_POOL_IMPLEMENTATION

#define blopt_plist               BLOP_CONCAT2(blopt_pool, _ptrs)
#define blopt_pnode               BLOP_CONCAT2(blopt_plist, _node)

#define blops_plist               BLOP_CONCAT3(_, blopt_plist, _t)
#define blops_pnode               BLOP_CONCAT3(_, blopt_pnode, _t)

#define blopfn_plist_create       BLOP_CONCAT2(blopt_plist, _create)
#define blopfn_plist_destroy      BLOP_CONCAT2(blopt_plist, _destroy)

#define blopfn_plist_get_front    BLOP_CONCAT2(blopt_plist, _get_front)

#define blopfn_plist_erase        BLOP_CONCAT2(blopt_plist, _erase)
#define blopfn_plist_pop_front    BLOP_CONCAT2(blopt_plist, _pop_front)

#define blopfn_plist_push_back    BLOP_CONCAT2(blopt_plist, _push_back)
#define blopfn_plist_insert_next  BLOP_CONCAT2(blopt_plist, _insert_next)

#define blopfn_pnode_create       BLOP_CONCAT2(blopt_pnode, _create)
#define blopfn_pnode_destroy      BLOP_CONCAT2(blopt_pnode, _destroy)

#define BLOP_LIST_NAME            blopt_plist
#define BLOP_LIST_DATA_TYPE       blopt_ptrhdr
#define BLOP_LIST_IMPLEMENTATION
#include <blop/list.h>

struct blops_ptrhdr {
    size_t                  size;
    blopt_pool              pool;
    blopt_pnode    node;
};
struct blops_pool {
    size_t                  bytes;
    blopt_plist         ptrs;
};

blopt_pool  blopfn_pool_create() {
  blopt_pool pool = BLOP_MALLOC(struct blops_pool, 1);
  BLOP_ASSERT_FORCED(pool != NULL, NULL, "Failed to allocate memory for pool");

  pool->bytes = 0;
  pool->ptrs  = blopfn_plist_create();

  return pool;
}
void        blopfn_pool_destroy(blopt_pool pool) {
  BLOP_ASSERT_PTR_VOID(pool);

  BLOP_ASSERT_VOID(pool->ptrs->size > 0, "Destroying non empty pool (HINT: Call free all)");

  blopfn_plist_destroy(pool->ptrs);
  BLOP_FREE(pool);
}

void        blopfn_pool_free(blopt_pool pool, void* ptr) {
  BLOP_ASSERT_PTR_VOID(pool);
  BLOP_ASSERT_PTR_VOID(ptr);

  blopt_ptrhdr hdr = (blopt_ptrhdr)BLOP_PTR_SUB(ptr, sizeof(struct blops_ptrhdr));
  BLOP_ASSERT_VOID(hdr->pool == pool, "Freeing a foreign ptr");

  blopfn_plist_erase(pool->ptrs, hdr->node, true);
  pool->bytes -= hdr->size;

  BLOP_FREE(hdr);
}
void        blopfn_pool_free_all(blopt_pool pool) {
  BLOP_ASSERT_PTR_VOID(pool);

  blopt_pnode current = blopfn_plist_get_front(pool->ptrs);
  while (current != NULL) {
    blopfn_plist_pop_front(pool->ptrs, true);
    blopt_ptrhdr hdr = current->data;
    pool->bytes -= hdr->size;
    BLOP_FREE(hdr);
    current = blopfn_plist_get_front(pool->ptrs);
  }
}
void*       blopfn_pool_malloc(blopt_pool pool, size_t size) {
  BLOP_ASSERT_PTR(pool, NULL);

  BLOP_ASSERT(size != 0, NULL, "Requested allocation size is 0");

  blopt_ptrhdr hdr = BLOP_MALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
  BLOP_ASSERT_FORCED(hdr != NULL, NULL, "Failed to allocate memory for pointer header");

  memset(hdr, 0, size + sizeof(struct blops_ptrhdr));
  hdr->size = size;
  hdr->pool = pool;
  hdr->node = blopfn_pnode_create();
  hdr->node->data = hdr;

  blopfn_plist_push_back(pool->ptrs, hdr->node);

  pool->bytes += size;
  void*  ptr   = BLOP_PTR_ADD(hdr, sizeof(struct blops_ptrhdr));
  return ptr;
}
void*       blopfn_pool_calloc(blopt_pool pool, size_t count, size_t size) {
  BLOP_ASSERT_PTR(pool, NULL);

  BLOP_ASSERT(size != 0, NULL, "Requested allocation size is 0");

  size *= count;
  blopt_ptrhdr hdr = BLOP_MALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
  BLOP_ASSERT_FORCED(hdr != NULL, NULL, "Failed to allocate memory for pointer header");

  memset(hdr, 0, size + sizeof(struct blops_ptrhdr));
  hdr->size = size;
  hdr->pool = pool;
  hdr->node = blopfn_pnode_create();
  hdr->node->data = hdr;

  blopfn_plist_push_back(pool->ptrs, hdr->node);

  pool->bytes += size;
  void*  ptr   = BLOP_PTR_ADD(hdr, sizeof(struct blops_ptrhdr));
  return ptr;
}
void*       blopfn_pool_realloc(blopt_pool pool, void* ptr, size_t size) {
  BLOP_ASSERT_PTR(pool, NULL);
  BLOP_ASSERT_PTR(ptr, NULL);

  BLOP_ASSERT(size != 0, NULL, "Requested allocation size is 0");

  blopt_ptrhdr srchdr = (blopt_ptrhdr)BLOP_PTR_SUB(ptr, sizeof(struct blops_ptrhdr));
  BLOP_ASSERT(srchdr->pool != pool, NULL, "Reallocating a foreign ptr");

  blopt_ptrhdr newhdr = BLOP_MALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
  BLOP_ASSERT_FORCED(newhdr != NULL, NULL, "Failed to allocate memory for pointer");

  newhdr->size = size;
  newhdr->pool = pool;
  newhdr->node = blopfn_pnode_create();
  newhdr->node->data = newhdr;
  
  blopfn_plist_insert_next(pool->ptrs, srchdr->node, newhdr->node);
  
  pool->bytes += size;
  void*  newptr = BLOP_PTR_ADD(newhdr, sizeof(struct blops_ptrhdr));
  memcpy(newptr, ptr, BLOP_MIN(size, srchdr->size));
  
  if (size > srchdr->size) {
      memset(BLOP_PTR_ADD(newptr, srchdr->size), 0, size - srchdr->size);
  }

  blopfn_plist_erase(pool->ptrs, srchdr->node, true);
  pool->bytes -= srchdr->size;
  BLOP_FREE(srchdr);

  return newptr;
}
void*       blopfn_pool_duplicate(blopt_pool pool, void* ptr, size_t size) {
  BLOP_ASSERT_PTR(pool, NULL);
  BLOP_ASSERT_PTR(ptr, NULL);

  BLOP_ASSERT(size != 0, NULL, "Requested allocation size is 0");

  blopt_ptrhdr srchdr = (blopt_ptrhdr)BLOP_PTR_SUB(ptr, sizeof(struct blops_ptrhdr));
  BLOP_ASSERT(srchdr->pool != pool, NULL, "Reallocating a foreign ptr");

  blopt_ptrhdr newhdr = BLOP_MALLOC(struct blops_ptrhdr, size + sizeof(struct blops_ptrhdr));
  BLOP_ASSERT_FORCED(newhdr != NULL, NULL, "Failed to allocate memory for pointer");

  memset(newhdr, 0, size + sizeof(struct blops_ptrhdr));
  newhdr->size = size;
  newhdr->pool = pool;
  newhdr->node = blopfn_pnode_create();
  newhdr->node->data = newhdr;

  blopfn_plist_insert_next(pool->ptrs, srchdr->node, newhdr->node);

  pool->bytes += size;
  void*  newptr = BLOP_PTR_ADD(newhdr, sizeof(struct blops_ptrhdr));
  memcpy(newptr, ptr, BLOP_MIN(size, srchdr->size));
  return newptr;
}

void        blopfn_pool_print(blopt_pool pool) {
  BLOP_ASSERT_PTR_VOID(pool);

  printf("Pool at %p\n", (void*)pool);
  printf("  Total bytes allocated: %zu\n", pool->bytes);
  printf("  Allocated pointers (%zu):\n", pool->ptrs->size);

  blopt_pnode current = blopfn_plist_get_front(pool->ptrs);
  size_t index = 0;
  while (current != NULL) {
    blopt_ptrhdr hdr = current->data;
    printf("    [%zu] Pointer at %p - Size: %zu bytes\n", index, BLOP_PTR_ADD(hdr, sizeof(struct blops_ptrhdr)), hdr->size);
    current = current->next;
    index++;
  }
}

size_t      blopfn_pool_get_bytes(blopt_pool pool) {
    BLOP_ASSERT_PTR(pool, 0);
    return pool->bytes;
}
size_t      blopfn_pool_get_count(blopt_pool pool) {
    BLOP_ASSERT_PTR(pool, 0);
    return pool->ptrs->size;
}

#endif /* BLOP_POOL_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#undef BLOP_POOL_NAME
#undef BLOP_POOL_IMPLEMENTATION

#undef blopt_pool
#undef blopt_ptrhdr

#undef blops_pool
#undef blops_ptrhdr

#undef blopfn_pool_create
#undef blopfn_pool_destroy

#undef blopfn_pool_free
#undef blopfn_pool_free_all  
#undef blopfn_pool_malloc    
#undef blopfn_pool_calloc    
#undef blopfn_pool_realloc   
#undef blopfn_pool_duplicate 

#undef blopfn_pool_print     
#undef blopfn_pool_get_bytes 
#undef blopfn_pool_get_count 

#undef blopt_plist              
#undef blopt_pnode              

#undef blops_plist              
#undef blops_pnode              

#undef blopfn_plist_create      
#undef blopfn_plist_destroy     

#undef blopfn_plist_get_front   

#undef blopfn_plist_erase       
#undef blopfn_plist_pop_front   

#undef blopfn_plist_push_back   
#undef blopfn_plist_insert_next 

#undef blopfn_pnode_create      
#undef blopfn_pnode_destroy     