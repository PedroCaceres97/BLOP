#include <blop/blop.h>

#ifndef BLOP_LIST_NAME
  #define BLOP_LIST_NAME blop_list
#endif /* BLOP_LIST_NAME */

#ifndef BLOP_LIST_DATA_TYPE
  #define BLOP_LIST_DATA_TYPE uint8_t
#endif /* BLOP_LIST_DATA_TYPE */

#define BLOP_TYPE BLOP_LIST_DATA_TYPE

/** @cond doxygen_ignore */
#define blopt_list              BLOP_LIST_NAME
#define blopt_node              BLOP_CONCAT2(blopt_list, _node)

#define blops_list              BLOP_CONCAT2(blopt_list, _t)
#define blops_node              BLOP_CONCAT2(blopt_node, _t)

#define blopfn_list_create      BLOP_CONCAT2(blopt_list, _create)
#define blopfn_list_destroy     BLOP_CONCAT2(blopt_list, _destroy)

#define blopfn_list_rdlock      BLOP_CONCAT2(blopt_list, _rdlock)
#define blopfn_list_wrlock      BLOP_CONCAT2(blopt_list, _wrlock)
#define blopfn_list_rdunlock    BLOP_CONCAT2(blopt_list, _rdunlock)
#define blopfn_list_wrunlock    BLOP_CONCAT2(blopt_list, _wrunlock)

#define blopfn_list_get         BLOP_CONCAT2(blopt_list, _get)
#define blopfn_list_size        BLOP_CONCAT2(blopt_list, _size)
#define blopfn_list_back        BLOP_CONCAT2(blopt_list, _back)
#define blopfn_list_front       BLOP_CONCAT2(blopt_list, _front)

#define blopfn_list_clear       BLOP_CONCAT2(blopt_list, _clear)
#define blopfn_list_erase       BLOP_CONCAT2(blopt_list, _erase)
#define blopfn_list_pop_back    BLOP_CONCAT2(blopt_list, _pop_back)
#define blopfn_list_pop_front   BLOP_CONCAT2(blopt_list, _pop_front)

#define blopfn_list_push_back   BLOP_CONCAT2(blopt_list, _push_back)
#define blopfn_list_push_front  BLOP_CONCAT2(blopt_list, _push_front)
#define blopfn_list_insert_next BLOP_CONCAT2(blopt_list, _insert_next)
#define blopfn_list_insert_prev BLOP_CONCAT2(blopt_list, _insert_prev)

#define blopfn_node_create      BLOP_CONCAT2(blopt_node, _create)
#define blopfn_node_duplicate   BLOP_CONCAT2(blopt_node, _duplicate)
#define blopfn_node_destroy     BLOP_CONCAT2(blopt_node, _destroy)

#define blopfn_node_data        BLOP_CONCAT2(blopt_node, _data)
#define blopfn_node_next        BLOP_CONCAT2(blopt_node, _next)
#define blopfn_node_prev        BLOP_CONCAT2(blopt_node, _prev)
#define blopfn_node_list        BLOP_CONCAT2(blopt_node, _list)

/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_list  blops_list;
typedef struct blops_node  blops_node;

typedef struct blops_list* blopt_list;
typedef struct blops_node* blopt_node;

blopt_list          blopfn_list_create      (blopt_list list);
void                blopfn_list_destroy     (blopt_list list);

void                blopfn_list_rdlock      (blopt_list list);
void                blopfn_list_wrlock      (blopt_list list);
void                blopfn_list_rdunlock    (blopt_list list);
void                blopfn_list_wrunlock    (blopt_list list);

blopt_node          blopfn_list_get         (blopt_list list, size_t index);
size_t              blopfn_list_size        (blopt_list list);
blopt_node          blopfn_list_back        (blopt_list list);
blopt_node          blopfn_list_front       (blopt_list list);

void                blopfn_list_clear       (blopt_list list, int deallocate);
void                blopfn_list_erase       (blopt_list list, blopt_node node, int deallocate);
void                blopfn_list_pop_back    (blopt_list list, int deallocate);
void                blopfn_list_pop_front   (blopt_list list, int deallocate);

void                blopfn_list_push_back   (blopt_list list, blopt_node node);
void                blopfn_list_push_front  (blopt_list list, blopt_node node);
void                blopfn_list_insert_next (blopt_list list, blopt_node pivot, blopt_node node);
void                blopfn_list_insert_prev (blopt_list list, blopt_node pivot, blopt_node node);

blopt_node          blopfn_node_create      (blopt_node node);
blopt_node          blopfn_node_duplicate   (blopt_node src, blopt_node dst);
void                blopfn_node_destroy     (blopt_node node);

BLOP_TYPE           blopfn_node_data        (blopt_node node);
blopt_node          blopfn_node_next        (blopt_node node);
blopt_node          blopfn_node_prev        (blopt_node node);
blopt_list          blopfn_node_list        (blopt_node node);

#if (defined(BLOP_LIST_STRUCT) || defined(BLOP_LIST_IMPLEMENTATION)) && !defined(BLOP_LIST_NOT_STRUCT)
struct blops_node {
  BLOP_TYPE         data;  
  blopt_node        next;  
  blopt_node        prev;  
  blopt_list        list;
  int               allocated;
};

struct blops_list {
  size_t            size;
  blopt_node        front;
  blopt_node        back;
  int               allocated;
  BLOP_RWLOCK_TYPE  lock;
};
#endif

#ifdef BLOP_LIST_IMPLEMENTATION

blopt_list          blopfn_list_create(blopt_list list) {
  if (!list) {
    BLOP_CALLOC(list, struct blops_list, 1);
    list->allocated = true;
  } else {
    list->allocated = false;
  }

  list->size  = 0;
  list->front = NULL;
  list->back  = NULL;
  BLOP_RWLOCK_INIT(list->lock);

  return list;
}
void                blopfn_list_destroy(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  BLOP_INTERNAL_ASSERT(list->size == 0, "Destroying non empty list (HINT: Clear the list)");

  BLOP_RWLOCK_DESTROY(list->lock);

  if (list->allocated) {
    BLOP_FREE(list);
  }
}

void                blopfn_list_rdlock(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  BLOP_RWLOCK_RDLOCK(list->lock);
}
void                blopfn_list_wrlock(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  BLOP_RWLOCK_WRLOCK(list->lock);
}
void                blopfn_list_rdunlock(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  BLOP_RWLOCK_RDUNLOCK(list->lock);
}
void                blopfn_list_wrunlock(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  BLOP_RWLOCK_WRUNLOCK(list->lock);
}

size_t              blopfn_list_size(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  return list->size;
}
blopt_node          blopfn_list_back(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  return list->back;
}
blopt_node          blopfn_list_front(blopt_list list) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  return list->front;
}

blopt_node          blopfn_list_get(blopt_list list, size_t idx) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  BLOP_INTERNAL_ASSERT_BOUNDS(idx, list->size);

  blopt_node current = NULL;
  if (idx < list->size / 2) {
    current = list->front;
    for (size_t i = 0; i < idx; i++) {
      current = current->next;
    }
  } else {
    current = list->back;
    for (size_t i = list->size - 1; i > idx; i--) {
      current = current->prev;
    }
  }
  return current;
}

void                blopfn_list_clear(blopt_list list, int deallocate) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  if (list->size == 0) {
    return;
  }

  blopt_node current = list->front;
  blopt_node next = NULL;
  while (current) {
    next = current->next;
    current->list = NULL;
    current->next = NULL;
    current->prev = NULL;
    if (deallocate) {
      blopfn_node_destroy(current);
    }
    current = next;
  }

  list->size = 0;
  list->front = NULL;
  list->back = NULL;
}
void                blopfn_list_erase(blopt_list list, blopt_node node, int deallocate) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  BLOP_INTERNAL_ASSERT_PTR(node);

  BLOP_INTERNAL_ASSERT(node->list == list, "Erasing a foreign node");

  if (node->prev) {
    node->prev->next = node->next;
  } else {
    list->front = node->next;
  }

  if (node->next) {
    node->next->prev = node->prev;
  } else {
    list->back = node->prev;
  }

  list->size--;
  node->list = NULL;
  node->next = NULL;
  node->prev = NULL;

  if (deallocate) {
    blopfn_node_destroy(node);
  }
}
void                blopfn_list_pop_back(blopt_list list, int deallocate) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  if (list->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  blopt_node back = list->back;

  if (list->back->prev) {
    list->back->prev->next = NULL;
    list->back = list->back->prev;
  } else {
    list->front = NULL;
    list->back = NULL;
  }

  list->size--;

  back->list = NULL;
  back->next = NULL;
  back->prev = NULL;
  if (deallocate) {
    blopfn_node_destroy(back);
  }
}
void                blopfn_list_pop_front(blopt_list list, int deallocate) {
  BLOP_INTERNAL_ASSERT_PTR(list);

  if (list->size == 0) {
    BLOP_EMPTY_POPPING();
    return;
  }

  blopt_node front = list->front;

  if (list->front->next) {
    list->front->next->prev = NULL;
    list->front = list->front->next;
  } else {
    list->front = NULL;
    list->back = NULL;
  }

  list->size--;

  front->list = NULL;
  front->next = NULL;
  front->prev = NULL;
  if (deallocate) {
    blopfn_node_destroy(front);
  }
}

void                blopfn_list_push_back(blopt_list list, blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  BLOP_INTERNAL_ASSERT_PTR(node);

  BLOP_INTERNAL_ASSERT(node->list == NULL, "Pushing a foreign node (HINT: Duplicate the node)");

  node->list = list;
  node->next = NULL;
  node->prev = list->back;

  if (list->back) {
    list->back->next = node;
    list->back = node;
  } else {
    list->front = node;
    list->back = node;
  }

  list->size++;
}
void                blopfn_list_push_front(blopt_list list, blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  BLOP_INTERNAL_ASSERT_PTR(node);

  BLOP_INTERNAL_ASSERT(node->list == NULL, "Pushing a foreign node (HINT: Duplicate the node)");

  node->list = list;
  node->next = list->front;
  node->prev = NULL;

  if (list->front) {
    list->front->prev = node;
    list->front = node;
  } else {
    list->front = node;
    list->back = node;
  }

  list->size++;
}
void                blopfn_list_insert_next(blopt_list list, blopt_node pivot, blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  BLOP_INTERNAL_ASSERT_PTR(pivot);
  BLOP_INTERNAL_ASSERT_PTR(node);

  BLOP_INTERNAL_ASSERT(node->list == NULL, "Inserting a foreign node (HINT: Duplicate the node)");
  BLOP_INTERNAL_ASSERT(pivot->list == list, "The pivot is a foreign node");

  if (pivot == list->back) {
    blopfn_list_push_back(list, node);
    return;
  }

  node->list = list;
  node->prev = pivot;
  node->next = pivot->next;

  pivot->next->prev = node;
  pivot->next = node;

  list->size++;
}
void                blopfn_list_insert_prev(blopt_list list, blopt_node pivot, blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(list);
  BLOP_INTERNAL_ASSERT_PTR(pivot);
  BLOP_INTERNAL_ASSERT_PTR(node);

  BLOP_INTERNAL_ASSERT(node->list == NULL, "Inserting a foreign node (HINT: Duplicate the node)");
  BLOP_INTERNAL_ASSERT(pivot->list == list, "The pivot is a foreign node");

  if (pivot == list->front) {
    blopfn_list_push_front(list, node);
    return;
  }

  node->list = list;
  node->next = pivot;
  node->prev = pivot->prev;

  pivot->prev->next = node;
  pivot->prev = node;

  list->size++;
}

blopt_node          blopfn_node_create(blopt_node node) {
  if (!node) {
    BLOP_CALLOC(node, struct blops_node, 1);
    node->allocated = true;
  } else {
    node->allocated = false;
  }


  node->data = (BLOP_TYPE)(0);
  node->next = NULL;
  node->prev = NULL;
  node->list = NULL;
  BLOP_RWLOCK_INIT(node->lock);

  return node;
}
blopt_node          blopfn_node_duplicate(blopt_node src, blopt_node dst) {
  BLOP_INTERNAL_ASSERT_PTR(src);

  dst = blopfn_node_create(dst);
  dst->data = src->data;

  return dst;
}
void                blopfn_node_destroy(blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(node);

  BLOP_INTERNAL_ASSERT(node->list == NULL, "Destroying an unattached node (HINT: Set deallocate to true in any list erasing function)");

  #ifdef BLOP_LIST_DEALLOCATE_DATA
    BLOP_LIST_DEALLOCATE_DATA(node->data);
  #endif
  BLOP_RWLOCK_DESTROY(node->lock);

  if (node->allocated) {
    BLOP_FREE(node);
  }
}

BLOP_TYPE           blopfn_node_data(blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(node);
  return node->data;
}
blopt_node          blopfn_node_next(blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(node);
  return node->next;
}
blopt_node          blopfn_node_prev(blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(node);
  return node->prev;
}
blopt_list          blopfn_node_list(blopt_node node) {
  BLOP_INTERNAL_ASSERT_PTR(node);
  return node->list;
}

#endif /* BLOP_LIST_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#ifdef _DOXYGEN_
  #define BLOP_LIST_DEALLOCATE_DATA(data)
  #define BLOP_LIST_IMPLEMENTATION
    
  #undef  BLOP_LIST_DEALLOCATE_DATA
  #undef  BLOP_LIST_IMPLEMENTATION
#endif

#undef BLOP_TYPE
#undef BLOP_LIST_NAME
#undef BLOP_LIST_DATA_TYPE
#undef BLOP_LIST_DEALLOCATE_DATA

#undef BLOP_LIST_STRUCT
#undef BLOP_LIST_NOT_STRUCT
#undef BLOP_LIST_IMPLEMENTATION

#undef blopt_list             
#undef blopt_node             
#undef blops_list             
#undef blops_node       

#undef blopfn_list_create     
#undef blopfn_list_destroy    

#undef blopfn_list_rdlock     
#undef blopfn_list_wrlock     
#undef blopfn_list_rdunlock   
#undef blopfn_list_wrunlock   

#undef blopfn_list_get        
#undef blopfn_list_size       
#undef blopfn_list_back       
#undef blopfn_list_front     

#undef blopfn_list_clear      
#undef blopfn_list_erase      
#undef blopfn_list_pop_back   
#undef blopfn_list_pop_front  

#undef blopfn_list_push_back  
#undef blopfn_list_push_front 
#undef blopfn_list_insert_next
#undef blopfn_list_insert_prev

#undef blopfn_node_create     
#undef blopfn_node_duplicate  
#undef blopfn_node_destroy    

#undef blopfn_node_data       
#undef blopfn_node_next       
#undef blopfn_node_prev       
#undef blopfn_node_list       