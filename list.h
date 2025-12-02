#include <blop/blop.h>

#ifndef BLOP_LIST_NAME
  #define BLOP_LIST_NAME listb
#endif /* BLOP_LIST_NAME */

#ifndef BLOP_LIST_DATA_TYPE
  #define BLOP_LIST_DATA_TYPE uint8_t
#endif /* BLOP_LIST_DATA_TYPE */

/** @cond doxygen_ignore */
#define blopt_list              BLOP_LIST_NAME
#define blopt_node              BLOP_CONCAT2(blopt_list, _node)

#define blops_list              BLOP_CONCAT3(_, blopt_list, _t)
#define blops_node              BLOP_CONCAT3(_, blopt_list, _node_t)

#define blopfn_list_init        BLOP_CONCAT2(blopt_list, _init)
#define blopfn_list_deinit      BLOP_CONCAT2(blopt_list, _deinit)
#define blopfn_list_create      BLOP_CONCAT2(blopt_list, _create)
#define blopfn_list_destroy     BLOP_CONCAT2(blopt_list, _destroy)

#define blopfn_list_rdlock      BLOP_CONCAT2(blopt_list, _rdlock)
#define blopfn_list_wrlock      BLOP_CONCAT2(blopt_list, _wrlock)
#define blopfn_list_rdunlock    BLOP_CONCAT2(blopt_list, _rdunlock)
#define blopfn_list_wrunlock    BLOP_CONCAT2(blopt_list, _wrunlock)

#define blopfn_list_get_front   BLOP_CONCAT2(blopt_list, _get_front)
#define blopfn_list_get_back    BLOP_CONCAT2(blopt_list, _get_back)
#define blopfn_list_get_size    BLOP_CONCAT2(blopt_list, _get_size)
#define blopfn_list_get_node    BLOP_CONCAT2(blopt_list, _get_node)

#define blopfn_list_clear       BLOP_CONCAT2(blopt_list, _clear)
#define blopfn_list_erase       BLOP_CONCAT2(blopt_list, _erase)
#define blopfn_list_pop_back    BLOP_CONCAT2(blopt_list, _pop_back)
#define blopfn_list_pop_front   BLOP_CONCAT2(blopt_list, _pop_front)

#define blopfn_list_push_back   BLOP_CONCAT2(blopt_list, _push_back)
#define blopfn_list_push_front  BLOP_CONCAT2(blopt_list, _push_front)
#define blopfn_list_insert_next BLOP_CONCAT2(blopt_list, _insert_next)
#define blopfn_list_insert_prev BLOP_CONCAT2(blopt_list, _insert_prev)

#define blopfn_node_init        BLOP_CONCAT2(blopt_node, _init)
#define blopfn_node_deinit      BLOP_CONCAT2(blopt_node, _deinit)
#define blopfn_node_create      BLOP_CONCAT2(blopt_node, _create)
#define blopfn_node_duplicate   BLOP_CONCAT2(blopt_node, _duplicate)
#define blopfn_node_destroy     BLOP_CONCAT2(blopt_node, _destroy)

#define blopfn_node_rdlock      BLOP_CONCAT2(blopt_node, _rdlock)
#define blopfn_node_wrlock      BLOP_CONCAT2(blopt_node, _wrlock)
#define blopfn_node_rdunlock    BLOP_CONCAT2(blopt_node, _rdunlock)
#define blopfn_node_wrunlock    BLOP_CONCAT2(blopt_node, _wrunlock)

#define blopfn_node_set_data    BLOP_CONCAT2(blopt_node, _set_data)
#define blopfn_node_get_data    BLOP_CONCAT2(blopt_node, _get_data)
#define blopfn_node_get_next    BLOP_CONCAT2(blopt_node, _get_next)
#define blopfn_node_get_prev    BLOP_CONCAT2(blopt_node, _get_prev)
#define blopfn_node_get_list    BLOP_CONCAT2(blopt_node, _get_list)
/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blops_list* blopt_list;
typedef struct blops_node* blopt_node;

void                blopfn_list_init        (blopt_list list);
void                blopfn_list_deinit      (blopt_list list);
blopt_list          blopfn_list_create      ();
void                blopfn_list_destroy     (blopt_list list);

blopt_node          blopfn_list_get_front   (blopt_list list);
blopt_node          blopfn_list_get_back    (blopt_list list);
size_t              blopfn_list_get_size    (blopt_list list);
blopt_node          blopfn_list_get_node    (blopt_list list, size_t index);

void                blopfn_list_rdlock      (blopt_list list);
void                blopfn_list_wrlock      (blopt_list list);
void                blopfn_list_rdunlock    (blopt_list list);
void                blopfn_list_wrunlock    (blopt_list list);

void                blopfn_list_clear       (blopt_list list, int deallocate);
void                blopfn_list_erase       (blopt_list list, blopt_node node, int deallocate);
void                blopfn_list_pop_back    (blopt_list list, int deallocate);
void                blopfn_list_pop_front   (blopt_list list, int deallocate);

void                blopfn_list_push_back   (blopt_list list, blopt_node node);
void                blopfn_list_push_front  (blopt_list list, blopt_node node);
void                blopfn_list_insert_next (blopt_list list, blopt_node pivot, blopt_node node);
void                blopfn_list_insert_prev (blopt_list list, blopt_node pivot, blopt_node node);

void                blopfn_node_init        (blopt_node node);
void                blopfn_node_deinit      (blopt_node node);
blopt_node          blopfn_node_create      ();
blopt_node          blopfn_node_duplicate   (blopt_node node);
void                blopfn_node_destroy     (blopt_node node);

void                blopfn_node_rdlock      (blopt_node node);
void                blopfn_node_wrlock      (blopt_node node);
void                blopfn_node_rdunlock    (blopt_node node);
void                blopfn_node_wrunlock    (blopt_node node);

blopt_node          blopfn_node_set_data    (blopt_node node, BLOP_LIST_DATA_TYPE data);
BLOP_LIST_DATA_TYPE blopfn_node_get_data    (blopt_node node);
blopt_node          blopfn_node_get_next    (blopt_node node);
blopt_node          blopfn_node_get_prev    (blopt_node node);
blopt_list          blopfn_node_get_list    (blopt_node node);

#ifdef BLOP_LIST_IMPLEMENTATION

struct blops_node {
  BLOP_LIST_DATA_TYPE data;  
  blopt_node          next;  
  blopt_node          prev;  
  blopt_list          list;
  BLOP_RWLOCK_TYPE    lock;
};

struct blops_list {
  size_t              size;
  blopt_node          front;
  blopt_node          back;
  BLOP_RWLOCK_TYPE    lock;
};

void                blopfn_list_init(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  list->size  = 0;
  list->front = NULL;
  list->back  = NULL;
  BLOP_RWLOCK_INIT(list->lock);
}
void                blopfn_list_deinit(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  BLOP_ASSERT_VOID(list->size == 0, "Deinitializing non empty list (HINT: Clear the list)");
  BLOP_RWLOCK_DESTROY(list->lock);
}
blopt_list          blopfn_list_create() {
  blopt_list list = BLOP_MALLOC(struct blops_list, 1);
  BLOP_ASSERT_FORCED(list != NULL, NULL, "Failed to allocate memory for list structure");

  blopfn_list_init(list);
  return list;
}
void                blopfn_list_destroy(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  BLOP_ASSERT_VOID(list->size == 0, "Destroying non empty list (HINT: Clear the list)");

  BLOP_RWLOCK_DESTROY(list->lock);
  BLOP_FREE(list);
}

void                blopfn_list_rdlock(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  BLOP_RWLOCK_RDLOCK(list->lock);
}
void                blopfn_list_wrlock(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  BLOP_RWLOCK_WRLOCK(list->lock);
}
void                blopfn_list_rdunlock(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  BLOP_RWLOCK_RDUNLOCK(list->lock);
}
void                blopfn_list_wrunlock(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  BLOP_RWLOCK_WRUNLOCK(list->lock);
}

blopt_node          blopfn_list_get_front(blopt_list list) {
  BLOP_ASSERT_PTR(list, NULL);
  return list->front;
}
blopt_node          blopfn_list_get_back(blopt_list list) {
  BLOP_ASSERT_PTR(list, NULL);
  return list->back;
}
size_t              blopfn_list_get_size(blopt_list list) {
  BLOP_ASSERT_PTR(list, 0);
  return list->size;
}
blopt_node          blopfn_list_get_node(blopt_list list, size_t idx) {
  BLOP_ASSERT_PTR(list, NULL);

  BLOP_ASSERT_BOUNDS(idx, list->size, NULL);

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
  BLOP_ASSERT_PTR_VOID(list);

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
  BLOP_ASSERT_PTR_VOID(list);
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == list, "Erasing a foreign node");

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
  BLOP_ASSERT_PTR_VOID(list);

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
  BLOP_ASSERT_PTR_VOID(list);

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
  BLOP_ASSERT_PTR_VOID(list);
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == NULL, "Pushing a foreign node (HINT: Duplicate the node)");

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
  BLOP_ASSERT_PTR_VOID(list);
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == NULL, "Pushing a foreign node (HINT: Duplicate the node)");

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
  BLOP_ASSERT_PTR_VOID(list);
  BLOP_ASSERT_PTR_VOID(pivot);
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == NULL, "Inserting a foreign node (HINT: Duplicate the node)");
  BLOP_ASSERT_VOID(pivot->list == list, "The pivot is a foreign node");

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
  BLOP_ASSERT_PTR_VOID(list);
  BLOP_ASSERT_PTR_VOID(pivot);
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == NULL, "Inserting a foreign node (HINT: Duplicate the node)");
  BLOP_ASSERT_VOID(pivot->list == list, "The pivot is a foreign node");

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

void                blopfn_node_init(blopt_node node) {
  node->data = (BLOP_LIST_DATA_TYPE)(0);
  node->next = NULL;
  node->prev = NULL;
  node->list = NULL;
  BLOP_RWLOCK_INIT(node->lock);
}
void                blopfn_node_deinit(blopt_node node) {
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == NULL, "Deinitializing an unattached node (HINT: Set deallocate to true in any list erasing function)");

  #ifdef BLOP_LIST_DEALLOCATE_DATA
    BLOP_LIST_DEALLOCATE_DATA(node->data);
  #endif
  BLOP_RWLOCK_DESTROY(node->lock);
}
blopt_node          blopfn_node_create() {
  blopt_node node = BLOP_MALLOC(struct blops_node, 1);
  BLOP_ASSERT_FORCED(node != NULL, NULL, "Failed to allocate memory for node structure");

  blopfn_node_init(node);
  return node;
}
blopt_node          blopfn_node_duplicate(blopt_node node) {
  BLOP_ASSERT_PTR(node, NULL);

  blopt_node dupnode = BLOP_MALLOC(struct blops_node, 1);
  BLOP_ASSERT_FORCED(dupnode != NULL, NULL, "Failed to allocate memory for node structure");

  blopfn_node_init(dupnode);
  dupnode->data = node->data;
  return dupnode;
}
void                blopfn_node_destroy(blopt_node node) {
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == NULL, "Destroying an unattached node (HINT: Set deallocate to true in any list erasing function)");

  #ifdef BLOP_LIST_DEALLOCATE_DATA
    BLOP_LIST_DEALLOCATE_DATA(node->data);
  #endif
  BLOP_RWLOCK_DESTROY(node->lock);
  BLOP_FREE(node);
}

void                blopfn_node_rdlock(blopt_node node) {
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_RWLOCK_RDLOCK(node->lock);
}
void                blopfn_node_wrlock(blopt_node node) {
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_RWLOCK_WRLOCK(node->lock);
}
void                blopfn_node_rdunlock(blopt_node node) {
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_RWLOCK_RDUNLOCK(node->lock);
}
void                blopfn_node_wrunlock(blopt_node node) {
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_RWLOCK_WRUNLOCK(node->lock);
}

blopt_node          blopfn_node_set_data(blopt_node node, BLOP_LIST_DATA_TYPE data) {
  BLOP_ASSERT_PTR(node, NULL);
  node->data = data;
  return node;
}
BLOP_LIST_DATA_TYPE blopfn_node_get_data(blopt_node node) {
  BLOP_ASSERT_PTR(node, (BLOP_LIST_DATA_TYPE)0);
  return node->data;
}
blopt_node          blopfn_node_get_next(blopt_node node) {
  BLOP_ASSERT_PTR(node, NULL);
  return node->next;
}
blopt_node          blopfn_node_get_prev(blopt_node node) {
  BLOP_ASSERT_PTR(node, NULL);
  return node->prev;
}
blopt_list          blopfn_node_get_list(blopt_node node) {
  BLOP_ASSERT_PTR(node, NULL);
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

#undef BLOP_LIST_NAME
#undef BLOP_LIST_DATA_TYPE
#undef BLOP_LIST_DEALLOCATE_DATA
#undef BLOP_LIST_IMPLEMENTATION

#undef blopt_list               
#undef blopt_node   

#undef blops_list               
#undef blops_node               

#undef blopfn_list_init
#undef blopfn_list_deinit
#undef blopfn_list_create        
#undef blopfn_list_destroy       
#undef blopfn_list_rdlock  
#undef blopfn_list_wrlock  
#undef blopfn_list_rdunlock
#undef blopfn_list_wrunlock
#undef blopfn_list_get_front     
#undef blopfn_list_get_back      
#undef blopfn_list_get_size      
#undef blopfn_list_get_node      
#undef blopfn_list_clear         
#undef blopfn_list_erase         
#undef blopfn_list_pop_back      
#undef blopfn_list_pop_front     
#undef blopfn_list_push_back     
#undef blopfn_list_push_front    
#undef blopfn_list_insert_next   
#undef blopfn_list_insert_prev   

#undef blopfn_node_init
#undef blopfn_node_deinit
#undef blopfn_node_create
#undef blopfn_node_duplicate
#undef blopfn_node_destroy
#undef blopfn_node_rdlock  
#undef blopfn_node_wrlock  
#undef blopfn_node_rdunlock
#undef blopfn_node_wrunlock
#undef blopfn_node_set_data
#undef blopfn_node_get_data
#undef blopfn_node_get_next
#undef blopfn_node_get_prev
#undef blopfn_node_get_list