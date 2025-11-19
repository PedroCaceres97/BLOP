/**
 * @file list.h
 * @brief Typed intrusive doubly-linked list — header-only, macro-configurable.
 *
 * This header implements a small, efficient, intrusive doubly-linked list
 * intended to be included multiple times with different macro configurations
 * to produce distinct, type-safe list variants.
 *
 * Key features
 *  - Intrusive nodes: the stored element is embedded in the node type (no
 *    opaque void* conversions).
 *  - Header-only: define BLOP_LIST_IMPLEMENTATION once to instantiate symbols.
 *  - Configurable type and name via:
 *      - BLOP_LIST_NAME        — unique suffix/prefix for generated symbols.
 *      - BLOP_LIST_DATA_TYPE  — type stored in each node.
 *      - BLOP_LIST_DEALLOCATE_DATA(ptr) — optional callback to free data.
 *
 * @code
 *   #define BLOP_LIST_NAME mylist
 *   #define BLOP_LIST_DATA_TYPE int
 *   #define BLOP_LIST_IMPLEMENTATION
 *   #include "list.h"
 *
 *   void main() {
 *      mylist list = mylist_create();
 *      mylist_push_back(list, mylist_set_data(mylist_node_create(), 10));
 *      mylist_pop_back(list, 1);
 *      mylist_destroy(list);
 *   }
 * @endcode 
 *
 * @version 1.0.0
 * @author Pedro Caceres
 * @date November 2025
 */

/** @defgroup blop_list List */

/* -------------------------------------------------------------------------- */
/*                               CONFIGURATION                                */
/* -------------------------------------------------------------------------- */

/**
 * @defgroup list_configuration Configuration
 * @ingroup blop_list
 * @brief Optional compile-time macro flags that modify List's behavior.
 * @{
 */

/**
 * @name Configuration
 * @brief Optional compile-time macro flags that modify List's behavior.
 * @{
 */

/**
 * @def BLOP_LIST_NAME
 * @brief Unique Symbols Tag
 *
 * It serves as the suffix of all typedefs and functions,
 * This let you reinclude list.h as many times as you want with
 * unique symbols each time.
 *
 * @note Default value is *listb*
 */
 
/**
 * @def BLOP_LIST_DATA_TYPE
 * @brief The data type of the data stored in the nodes
 *
 * It let you store any kind of data type standard of user defined without
 * the use of void*.
 * This means that you can have heap or stack allocated data.
 * Consider defining ::BLOP_LIST_DEALLOCATE_DATA when using heap allocated data.
 *
 * @note Default value is *uint8_t*
 */

/**
 * @def BLOP_LIST_DEALLOCATE_DATA(ptr)
 * @brief Deallocator callback for stored data at node destroy time
 *
 * It let you define a deallocator callback for your custom data stored in the nodes.
 * The signature of the function
 *
 * @note Default value is undefined
 */

/**
 * @def BLOP_LIST_IMPLEMENTATION 
 * @brief Includes all functions implementations
 * @note Should be included once per unique BLOP_LIST_NAME
 */

/** @} */ /* end of list_configuration (name) */

/** @} */ /* end of list_configuration */

/* -------------------------------------------------------------------------- */
/*                              Unique Symbols                                */
/* -------------------------------------------------------------------------- */

/**
 * @defgroup list_macros Unique Symbols
 * @ingroup blop_list
 *
 * - blopt_list will be expanded to BLOP_LIST_NAME
 * - blopt_node will be expanded to BLOP_LIST_NAME_node
 * - blopfn_* will be expanded to BLOP_LIST_NAME_*
 *
 * - The `t` in blopt means `typedef`
 * - The `s` in blops means `structure`
 * - The `fn` in blopfn means `function`
 * @{
 */

/**
 * @name Unique Symbols
 *
 * - blopt_list will be expanded to BLOP_LIST_NAME
 * - blopt_node will be expanded to BLOP_LIST_NAME_node
 * - blopfn_* will be expanded to BLOP_LIST_NAME_*
 *
 * - The `t` in blopt means `typedef`
 * - The `s` in blops means `structure`
 * - The `fn` in blopfn means `function`
 * @{
 */

/**
 * @def blopt_list
 * @brief Unique symbol for all prefixes
*/

/**
 * @def blopt_node
 * @brief Unique symbol for all node related prefixes
 */

/**
 * @def blops_list
 * @brief Unique symbol for list structure 
 */

/**
 * @def blops_node
 * @brief Unique symbol for node structure 
 */


/**
 * @def blopfn_list_create
 * @brief Unique symbol for *_create 
 */

/**
 * @def blopfn_list_destroy
 * @brief Unique symbol for *_destroy 
 */

/**
 * @def blopfn_list_get_front
 * @brief Unique symbol for *_get_front 
 */

/**
 * @def blopfn_list_get_back
 * @brief Unique symbol for *_get_back 
 */

/**
 * @def blopfn_list_get_size
 * @brief Unique symbol for *_get_size 
 */

/**
 * @def blopfn_list_get_node
 * @brief Unique symbol for *_get_node 
 */

/**
 * @def blopfn_list_clear
 * @brief Unique symbol for *_clear 
 */

/**
 * @def blopfn_list_erase
 * @brief Unique symbol for *_erase 
 */

/**
 * @def blopfn_list_pop_back
 * @brief Unique symbol for *_pop_back 
 */

/**
 * @def blopfn_list_pop_front
 * @brief Unique symbol for *_pop_front 
 */

/**
 * @def blopfn_list_push_back
 * @brief Unique symbol for *_push_back 
 */

/**
 * @def blopfn_list_push_front
 * @brief Unique symbol for *_push_front 
 */

/**
 * @def blopfn_list_insert_next
 * @brief Unique symbol for *_insert_next 
 */

/**
 * @def blopfn_list_insert_prev
 * @brief Unique symbol for *_insert_prev 
 */


/**
 * @def blopfn_node_create
 * @brief Unique symbol for *_create 
 */

/**
 * @def blopfn_node_duplicate
 * @brief Unique symbol for *_duplicate 
 */

/**
 * @def blopfn_node_destroy
 * @brief Unique symbol for *_destroy 
 */

/**
 * @def blopfn_node_set_data
 * @brief Unique symbol for *_set_data 
 */

/**
 * @def blopfn_node_get_data
 * @brief Unique symbol for *_get_data 
 */

/**
 * @def blopfn_node_get_next
 * @brief Unique symbol for *_get_next 
 */

/**
 * @def blopfn_node_get_prev
 * @brief Unique symbol for *_get_prev 
 */


/** @} */ /* end Macros (name) */

/** @} */ /* end Macros */

/* -------------------------------------------------------------------------- */
/*                          STRUCTURES / TYPEDEFS                             */
/* -------------------------------------------------------------------------- */

/** 
 * @defgroup list_structures Structures and Typedefs
 * @ingroup blop_list
 * @{
 */

/**
 * @typedef blopt_list
 * @brief Opaque pointer to the configured list type.
 */

/**
 * @typedef blopt_node
 * @brief Opaque pointer to the configured node type.
 */

/**
 * @struct blops_list
 * @brief Concrete list structure
 * @warning visible only when ::BLOP_LIST_IMPLEMENTATION defined.
 */

/** 
 * @var size_t blops_list::size
 * @brief Number of nodes.
 */

/** 
 * @var blopt_node blops_list::front
 * @brief Front node or NULL.
 */


/** 
 * @var blopt_node blops_list::back
 * @brief Back node or NULL.
 */

/**
 * @struct blops_node
 * @brief Concrete node structure
 * @warning visible only when ::BLOP_LIST_IMPLEMENTATION defined.
 */

/** 
 * @var BLOP_LIST_DATA_TYPE blops_node::data
 * @brief Stored element.
 */

/**
 * @var blopt_node blops_node::next
 * @brief Next node in list (NULL if none).
 */

/**
 * @var blopt_node blops_node::prev
 * @brief Previous node in list (NULL if none).
 */

/**
 * @var blopt_list blops_node::list
 * @brief Owning list (NULL if unlinked).
 */

/** @} */ /* end of list structures */

/* -------------------------------------------------------------------------- */
/*                                 List API                                   */
/* -------------------------------------------------------------------------- */

/**
 * @defgroup list_api List API
 * @ingroup blop_list
 * @brief The guide for list functions
 *
 * - blopt_list will be expanded to BLOP_LIST_NAME
 * - blopt_node will be expanded to BLOP_LIST_NAME_node
 * - blopfn_* will be expanded to BLOP_LIST_NAME_*
 *
 * - The `t` in blopt means `typedef`
 * - The `s` in blops means `structure`
 * - The `fn` in blopfn means `function`
 * @{
 */

/**
 * @name List API
 * @brief The guide for list functions
 *
 * - blopt_list will be expanded to BLOP_LIST_NAME
 * - blopt_node will be expanded to BLOP_LIST_NAME_node
 * - blopfn_* will be expanded to BLOP_LIST_NAME_*
 *
 * - The `t` in blopt means `typedef`
 * - The `s` in blops means `structure`
 * - The `fn` in blopfn means `function`
 * @{
 */

/**
 * @fn blopt_list blopfn_list_create()
 * @brief Allocate and initialize a new empty list.
 * @return Newly allocated list pointer, or NULL on allocation failure.
 */

/**
 * @fn void blopfn_list_destroy(blopt_list list)
 * @brief Destroy a list structure.
 *
 * The list must be empty (size == 0) when destroyed. Use blopfn_list_clear
 * to remove and optionally deallocate nodes prior to calling destroy.
 *
 * @param list List to destroy.
 */

/**
 * @fn blopt_node blopfn_list_get_front(blopt_list list)
 * @brief Get the front node of the list.
 * @param list List to query.
 * @return Front node pointer or NULL if list is empty or list is NULL.
 */

/**
 * @fn blopt_node blopfn_list_get_back(blopt_list list)
 * @brief Get the back node of the list.
 * @param list List to query.
 * @return Back node pointer or NULL if list is empty or list is NULL.
 */

/**
 * @fn size_t blopfn_list_get_size(blopt_list list)
 * @brief Get the number of nodes in the list.
 * @param list List to query.
 * @return Number of nodes, or 0 if list is NULL.
 */

/**
 * @fn blopt_node blopfn_list_get_node(blopt_list list, size_t index)
 * @brief Fetch node at zero-based index.
 * @param list List to search.
 * @param index Zero-based index (0..size-1).
 * @return Node pointer at index, or NULL on error (out of bounds or NULL list).
 */

/**
 * @fn void blopfn_list_clear(blopt_list list, int deallocate)
 * @brief Remove all nodes from a list.
 * @param list List to clear.
 * @param deallocate If non-zero the nodes are destroyed (and their data deallocated if BLOP_LIST_DEALLOCATE_DATA is provided).
 */

/**
 * @fn void blopfn_list_erase(blopt_list list, blopt_node node, int deallocate)
 * @brief Erase a specific node from a list.
 * @param list List containing the node.
 * @param node Node to remove.
 * @param deallocate If non-zero the node is destroyed after unlinking.
 */

/**
 * @fn void blopfn_list_pop_back(blopt_list list, int deallocate)
 * @brief Pop the back node.
 * @param list List to operate on.
 * @param deallocate If non-zero the popped node is destroyed.
 */

/**
 * @fn void blopfn_list_pop_front(blopt_list list, int deallocate)
 * @brief Pop the front node.
 * @param list List to operate on.
 * @param deallocate If non-zero the popped node is destroyed.
 */

/**
 * @fn void blopfn_list_push_back(blopt_list list, blopt_node node)
 * @brief Push an existing node at the back of the list.
 * @param list Target list.
 * @param node Node to insert (must not already belong to a list).
 */

/**
 * @fn void blopfn_list_push_front(blopt_list list, blopt_node node)
 * @brief Push an existing node at the front of the list.
 * @param list Target list.
 * @param node Node to insert (must not already belong to a list).
 */

/**
 * @fn void blopfn_list_insert_next(blopt_list list, blopt_node pivot, blopt_node node)
 * @brief Insert node after a pivot node.
 * @param list Target list.
 * @param pivot Node after which to insert.
 * @param node Node to insert (must not already belong to a list).
 */

/**
 * @fn void blopfn_list_insert_prev (blopt_list list, blopt_node pivot, blopt_node node)
 * @brief Insert node before a pivot node.
 * @param list Target list.
 * @param pivot Node before which to insert.
 * @param node Node to insert (must not already belong to a list).
 */

/** @} */ /* end List API (name) */

/** @} */ /* end List API */

/* -------------------------------------------------------------------------- */
/*                                 Node API                                   */
/* -------------------------------------------------------------------------- */

/**
 * @defgroup list_node_api Node API
 * @ingroup blop_list
 * @brief The guide for node functions
 * 
 * - blopt_list will be expanded to BLOP_LIST_NAME
 * - blopt_node will be expanded to BLOP_LIST_NAME_node
 * - blopfn_* will be expanded to BLOP_LIST_NAME_*
 *
 * - The `t` in blopt means `typedef`
 * - The `s` in blops means `structure`
 * - The `fn` in blopfn means `function`
 * @{
 */

/**
 * @name Node API
 * @brief The guide for node functions
 * 
 * - blopt_list will be expanded to BLOP_LIST_NAME
 * - blopt_node will be expanded to BLOP_LIST_NAME_node
 * - blopfn_* will be expanded to BLOP_LIST_NAME_*
 *
 * - The `t` in blopt means `typedef`
 * - The `s` in blops means `structure`
 * - The `fn` in blopfn means `function`
 * @{
 */

/**
 * @fn blopt_node blopfn_node_create()
 * @brief Allocate a new node initialized to zero.
 * @return Newly allocated node pointer, or NULL on allocation failure.
 */

/**
 * @fn blopt_node blopfn_node_duplicate(blopt_node node)
 * @brief Duplicate a node (shallow copy of @c data only).
 * @param node Node to duplicate (must be non-NULL).
 * @return New node containing a copy of node->data, not attached to any list.
 */

/**
 * @fn void blopfn_node_destroy(blopt_node node)
 * @brief Destroy a standalone node.
 *
 * Node must not belong to any list. If BLOP_LIST_DEALLOCATE_DATA is provided
 * it will be invoked on the node data before freeing the node storage.
 *
 * @param node Node to destroy.
 */

/**
 * @fn blopt_node blopfn_node_set_data(blopt_node node, BLOP_LIST_DATA_TYPE data)
 * @brief Set node data.
 * @param node Target node.
 * @param data Data value to store.
 * @return The same node pointer (for convenience / chaining), or NULL on error.
 */

/**
 * @fn BLOP_LIST_DATA_TYPE blopfn_node_get_data(blopt_node node)
 * @brief Get node data.
 * @param node Node to query.
 * @return Stored data, or 0 (cast) if node is NULL.
 */

/**
 * @fn blopt_node blopfn_node_get_next(blopt_node node)
 * @brief Get next node pointer.
 * @param node Node to query.
 * @return Next node or NULL.
 */

/**
 * @fn blopt_node blopfn_node_get_prev(blopt_node node)
 * @brief Get previous node pointer.
 * @param node Node to query.
 * @return Previous node or NULL.
 */

/** @} */ /* end of node api (name) */

/** @} */ /* end of node api */

#include <blop/blop.h>

#ifndef BLOP_LIST_NAME
  #define BLOP_LIST_NAME listb
#endif /* BLOP_LIST_NAME */

#ifndef BLOP_LIST_DATA_TYPE
  #define BLOP_LIST_DATA_TYPE uint8_t
#endif /* BLOP_LIST_DATA_TYPE */

#define blopt_list              BLOP_LIST_NAME
#define blopt_node              BLOP_CONCAT2(blopt_list, _node)

#define blops_list              BLOP_CONCAT3(_, blopt_list, _t)
#define blops_node              BLOP_CONCAT3(_, blopt_list, _node_t)

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

typedef struct blops_list* blopt_list;
typedef struct blops_node* blopt_node;

blopt_list          blopfn_list_create      ();
void                blopfn_list_destroy     (blopt_list list);

blopt_node          blopfn_list_get_front   (blopt_list list);
blopt_node          blopfn_list_get_back    (blopt_list list);
size_t              blopfn_list_get_size    (blopt_list list);
blopt_node          blopfn_list_get_node    (blopt_list list, size_t index);

void                blopfn_list_clear       (blopt_list list, int deallocate);
void                blopfn_list_erase       (blopt_list list, blopt_node node, int deallocate);
void                blopfn_list_pop_back    (blopt_list list, int deallocate);
void                blopfn_list_pop_front   (blopt_list list, int deallocate);

void                blopfn_list_push_back   (blopt_list list, blopt_node node);
void                blopfn_list_push_front  (blopt_list list, blopt_node node);
void                blopfn_list_insert_next (blopt_list list, blopt_node pivot, blopt_node node);
void                blopfn_list_insert_prev (blopt_list list, blopt_node pivot, blopt_node node);

blopt_node          blopfn_node_create      ();
blopt_node          blopfn_node_duplicate   (blopt_node node);
void                blopfn_node_destroy     (blopt_node node);

blopt_node          blopfn_node_set_data    (blopt_node node, BLOP_LIST_DATA_TYPE data);
BLOP_LIST_DATA_TYPE blopfn_node_get_data    (blopt_node node);
blopt_node          blopfn_node_get_next    (blopt_node node);
blopt_node          blopfn_node_get_prev    (blopt_node node);

#ifdef BLOP_LIST_IMPLEMENTATION

struct blops_node {
    BLOP_LIST_DATA_TYPE data;  
    blopt_node          next;  
    blopt_node          prev;  
    blopt_list          list;  
};

struct blops_list {
    size_t              size;  
    blopt_node          front; 
    blopt_node          back;  
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

#ifdef _DOXYGEN_

  #define BLOP_LIST_DEALLOCATE_DATA(ptr)
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