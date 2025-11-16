/**
 * @file list.h
 * @brief Generic intrusive doubly-linked list (header-only, macro-configurable).
 *
 * This header provides a small, configurable, intrusive doubly-linked list
 * implementation. It is intended to be included with different macro
 * configurations to produce typed list variants.
 *
 * Configuration macros:
 * - BLOP_LIST_NAME:       Identifier for the list (defaults to `listb`).
 * - BLOP_LIST_DATA_TYPE:  Type stored in each node (defaults to `uint8_t`).
 * - BLOP_LIST_DEALLOCATE_DATA:
 *     Optional macro/function invoked during node destruction to free user
 *     data. Should be a macro taking one argument: the data value.
 * - BLOP_LIST_IMPLEMENTATION:
 *     Define in one translation unit to emit the implementation bodies.
 *
 * Usage:
 * - Include this header normally to get type/function declarations.
 * - In one .c file define BLOP_LIST_IMPLEMENTATION before including to get
 *   definitions.
 *
 * Example:
 *   #define BLOP_LIST_NAME mylist
 *   #define BLOP_LIST_DATA_TYPE int
 *   #define BLOP_LIST_IMPLEMENTATION
 *   #include "list.h"
 *
 * The API is intrusive: nodes contain the stored data and list pointers.
 *
 * @author Pedro Caceres
 * @date 2025-11-16
 */

#include <blop/blop.h>

#ifndef BLOP_LIST_NAME
    #define BLOP_LIST_NAME listb
#endif /* BLOP_LIST_NAME */

#ifndef BLOP_LIST_DATA_TYPE
    #define BLOP_LIST_DATA_TYPE uint8_t
#endif /* BLOP_LIST_DATA_TYPE */

/** Internal token names derived from configuration macros. */
#define blopt_list              BLOP_LIST_NAME
#define blopt_node              BLOP_CONCAT2(blopt_list, _node)

#define blops_list              BLOP_CONCAT3(_, blopt_list, _t)
#define blops_node              BLOP_CONCAT3(_, blopt_list, _node_t)

/** Public function name tokens derived from list name. */
#define blopfn_list_create      BLOP_CONCAT2(blopt_list, _create)
#define blopfn_list_destroy     BLOP_CONCAT2(blopt_list, _destroy)
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

#define blopfn_node_create      BLOP_CONCAT2(blopt_node, _create)
#define blopfn_node_duplicate   BLOP_CONCAT2(blopt_node, _duplicate)
#define blopfn_node_destroy     BLOP_CONCAT2(blopt_node, _destroy)
#define blopfn_node_set_data    BLOP_CONCAT2(blopt_node, _set_data)
#define blopfn_node_get_data    BLOP_CONCAT2(blopt_node, _get_data)
#define blopfn_node_get_next    BLOP_CONCAT2(blopt_node, _get_next)
#define blopfn_node_get_prev    BLOP_CONCAT2(blopt_node, _get_prev)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef blopt_list
 * @brief Opaque pointer to the configured list type.
 */

/**
 * @typedef blopt_node
 * @brief Opaque pointer to the configured node type.
 */
typedef struct blops_list* blopt_list;
typedef struct blops_node* blopt_node;

/**
 * @name List API
 * @{
 */

/**
 * @brief Allocate and initialize a new empty list.
 * @return Newly allocated list pointer, or NULL on allocation failure.
 */
blopt_list          blopfn_list_create      ();

/**
 * @brief Destroy a list structure.
 *
 * The list must be empty (size == 0) when destroyed. Use blopfn_list_clear
 * to remove and optionally deallocate nodes prior to calling destroy.
 *
 * @param list List to destroy.
 */
void                blopfn_list_destroy     (blopt_list list);

/**
 * @brief Get the front node of the list.
 * @param list List to query.
 * @return Front node pointer or NULL if list is empty or list is NULL.
 */
blopt_node          blopfn_list_get_front   (blopt_list list);

/**
 * @brief Get the back node of the list.
 * @param list List to query.
 * @return Back node pointer or NULL if list is empty or list is NULL.
 */
blopt_node          blopfn_list_get_back    (blopt_list list);

/**
 * @brief Get the number of nodes in the list.
 * @param list List to query.
 * @return Number of nodes, or 0 if list is NULL.
 */
size_t              blopfn_list_get_size    (blopt_list list);

/**
 * @brief Fetch node at zero-based index.
 * @param list List to search.
 * @param index Zero-based index (0..size-1).
 * @return Node pointer at index, or NULL on error (out of bounds or NULL list).
 */
blopt_node          blopfn_list_get_node    (blopt_list list, size_t index);

/**
 * @brief Remove all nodes from a list.
 * @param list List to clear.
 * @param deallocate If non-zero the nodes are destroyed (and their data
 *        deallocated if BLOP_LIST_DEALLOCATE_DATA is provided).
 */
void                blopfn_list_clear       (blopt_list list, int deallocate);

/**
 * @brief Erase a specific node from a list.
 * @param list List containing the node.
 * @param node Node to remove.
 * @param deallocate If non-zero the node is destroyed after unlinking.
 */
void                blopfn_list_erase       (blopt_list list, blopt_node node, int deallocate);

/**
 * @brief Pop the back node.
 * @param list List to operate on.
 * @param deallocate If non-zero the popped node is destroyed.
 */
void                blopfn_list_pop_back    (blopt_list list, int deallocate);

/**
 * @brief Pop the front node.
 * @param list List to operate on.
 * @param deallocate If non-zero the popped node is destroyed.
 */
void                blopfn_list_pop_front   (blopt_list list, int deallocate);

/**
 * @brief Push an existing node at the back of the list.
 * @param list Target list.
 * @param node Node to insert (must not already belong to a list).
 */
void                blopfn_list_push_back   (blopt_list list, blopt_node node);

/**
 * @brief Push an existing node at the front of the list.
 * @param list Target list.
 * @param node Node to insert (must not already belong to a list).
 */
void                blopfn_list_push_front  (blopt_list list, blopt_node node);

/**
 * @brief Insert node after a pivot node.
 * @param list Target list.
 * @param pivot Node after which to insert.
 * @param node Node to insert (must not already belong to a list).
 */
void                blopfn_list_insert_next (blopt_list list, blopt_node pivot, blopt_node node);

/**
 * @brief Insert node before a pivot node.
 * @param list Target list.
 * @param pivot Node before which to insert.
 * @param node Node to insert (must not already belong to a list).
 */
void                blopfn_list_insert_prev (blopt_list list, blopt_node pivot, blopt_node node);

/** @} */ /* end List API */

/**
 * @name Node API
 * @{
 */

/**
 * @brief Allocate a new node initialized to zero.
 * @return Newly allocated node pointer, or NULL on allocation failure.
 */
blopt_node          blopfn_node_create      ();

/**
 * @brief Duplicate a node (shallow copy of @c data only).
 * @param node Node to duplicate (must be non-NULL).
 * @return New node containing a copy of node->data, not attached to any list.
 */
blopt_node          blopfn_node_duplicate   (blopt_node node);

/**
 * @brief Destroy a standalone node.
 *
 * Node must not belong to any list. If BLOP_LIST_DEALLOCATE_DATA is provided
 * it will be invoked on the node data before freeing the node storage.
 *
 * @param node Node to destroy.
 */
void                blopfn_node_destroy     (blopt_node node);

/**
 * @brief Set node data.
 * @param node Target node.
 * @param data Data value to store.
 * @return The same node pointer (for convenience / chaining), or NULL on error.
 */
blopt_node          blopfn_node_set_data    (blopt_node node, BLOP_LIST_DATA_TYPE data);

/**
 * @brief Get node data.
 * @param node Node to query.
 * @return Stored data, or 0 (cast) if node is NULL.
 */
BLOP_LIST_DATA_TYPE blopfn_node_get_data    (blopt_node node);

/**
 * @brief Get next node pointer.
 * @param node Node to query.
 * @return Next node or NULL.
 */
blopt_node          blopfn_node_get_next    (blopt_node node);

/**
 * @brief Get previous node pointer.
 * @param node Node to query.
 * @return Previous node or NULL.
 */
blopt_node          blopfn_node_get_prev    (blopt_node node);

/** @} */ /* end Node API */

#ifdef BLOP_LIST_IMPLEMENTATION

/**
 * @internal
 * @brief Concrete node structure (visible only when IMPLEMENTATION defined).
 */
struct blops_node {
    BLOP_LIST_DATA_TYPE data;  /**< Stored element. */
    blopt_node          next;  /**< Next node in list (NULL if none). */
    blopt_node          prev;  /**< Previous node in list (NULL if none). */
    blopt_list          list;  /**< Owning list (NULL if unlinked). */
};

/**
 * @internal
 * @brief Concrete list structure (visible only when IMPLEMENTATION defined).
 */
struct blops_list {
    size_t              size;   /**< Number of nodes. */
    blopt_node          front;  /**< Front node or NULL. */
    blopt_node          back;   /**< Back node or NULL. */
};

blopt_list          blopfn_list_create() {
  blopt_list list = BLOP_MALLOC(struct blops_list, 1);
    if (!list) {
      BLOP_ERROR_MESSAGE("Failed to allocate memory for list structure");
      BLOP_ABORT();
      return NULL;
    }


  memset(list, 0, sizeof(struct blops_list));
  return list;
}
void                blopfn_list_destroy(blopt_list list) {
  BLOP_ASSERT_PTR_VOID(list);

  BLOP_ASSERT_VOID(list->size == 0, "A non empty list can not be destroyed, clear the list first");
  free(list);
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

  BLOP_ASSERT_BONDS(idx, list->size, NULL);

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

  BLOP_ASSERT_VOID(node->list == list, "The node does not belong to this list");

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
      BLOP_EMPTY_POPPING("To enable empty popping and avoid this error define BLOP_VALID_EMPTY_POPPING");
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
      BLOP_EMPTY_POPPING("To enable empty popping and avoid this error define BLOP_VALID_EMPTY_POPPING");
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

  BLOP_ASSERT_VOID(node->list == NULL, "The node already belongs to a list, duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");

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

  BLOP_ASSERT_VOID(node->list == NULL, "The node already belongs to a list, duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");

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

  BLOP_ASSERT_VOID(node->list == NULL, "The node already belongs to a list, duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
  BLOP_ASSERT_VOID(pivot->list == list, "The pivot does not belong to this list");

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

  BLOP_ASSERT_VOID(node->list == NULL, "The node already belongs to a list, duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
  BLOP_ASSERT_VOID(pivot->list == list, "The pivot does not belong to this list");

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

blopt_node          blopfn_node_create() {
  blopt_node node = BLOP_MALLOC(struct blops_node, 1);
  if (!node) {
    BLOP_ERROR_MESSAGE("Failed to allocate memory for list_node structure");
    BLOP_ABORT();
    return NULL;
  }

  memset(node, 0, sizeof(struct blops_node));
  return node;
}
blopt_node          blopfn_node_duplicate(blopt_node node) {
  BLOP_ASSERT_PTR(node, NULL);

  blopt_node dupnode = BLOP_MALLOC(struct blops_node, 1);
  if (!node) {
    BLOP_ERROR_MESSAGE("Failed to allocate memory for list_node structure");
    BLOP_ABORT();
    return NULL;
  }

  dupnode->next = NULL;
  dupnode->prev = NULL;
  dupnode->list = NULL;
  dupnode->data = node->data;
  return dupnode;
}
void                blopfn_node_destroy(blopt_node node) {
  BLOP_ASSERT_PTR_VOID(node);

  BLOP_ASSERT_VOID(node->list == NULL, "The node belongs to a list and it can not be destroyed, you can deallocate a node at erase time.");

  #ifdef BLOP_LIST_DEALLOCATE_DATA
      BLOP_LIST_DEALLOCATE_DATA(node->data);
  #endif /* BLOP_LIST_DEALLOCATE_DATA */

  free(node);
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

#endif /* BLOP_LIST_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#undef BLOP_LIST_NAME
#undef BLOP_LIST_DATA_TYPE
#undef BLOP_LIST_DEALLOCATE_DATA
#undef BLOP_LIST_IMPLEMENTATION

#undef blopt_list               
#undef blopt_node   

#undef blops_list               
#undef blops_node               

#undef blopfn_list_create        
#undef blopfn_list_destroy       
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

#undef blopfn_node_create        
#undef blopfn_node_duplicate     
#undef blopfn_node_destroy       
#undef blopfn_node_set_data      
#undef blopfn_node_get_data      
#undef blopfn_node_get_next      
#undef blopfn_node_get_prev      