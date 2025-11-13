#include <blop/blop.h>

/*===============================================================================
 Module: BLOP list
 Version: 1.1.1
 Status: stable
 Dependencies:
   - blop/blop.h (provides BLOP_MALLOC, BLOP_ASSERT_PTR, BLOP_ERROR_MESSAGE,
     BLOP_ABORT, BLOP_CONCAT macros, and optional deallocation hooks)
   - C99 (uses size_t, stdint types, memset, malloc/free)

 Macros/flags that modify the API or behavior:

   - BLOP_LIST_NAME            — Default: listb
       Type: identifier
       Effect: Chooses the public type and symbol prefixes used by this header.
       Notes: Define before including this header to instantiate a different
              list variant in the same program/translation unit.

   - BLOP_LIST_DATA_TYPE       — Default: uint8_t
       Type: type identifier
       Effect: Sets the stored data type for nodes.
       Notes: Must be defined before include when using other-than-default.

   - BLOP_LIST_DEALLOCATE_DATA — Default: (none)
       Type: macro/function-like
       Effect: If defined, used in node destroy path to deallocate node->data.
       Notes: Should accept the node->data expression as its argument.

   - BLOP_LIST_IMPLEMENTATION  — Default: not defined
       Type: flag
       Effect: When defined in one translation unit, embeds the implementation
               (function bodies). Typically define in exactly one .c file.
   - Other required macros come from blop/blop.h (BLOP_MALLOC, BLOP_ABORT, etc.)
===============================================================================

Table of contents
  1) First Overview
  2) Macros, how to fit your needs
  3) API conventions and public types
  4) Memory and ownership
  5) Error handling and safe-mode behavior
  6) Thread-safety
  7) Common usage patterns and examples
  8) Troubleshooting and common mistakes
  9) Extensive internal documentation (reference)
===============================================================================

1) First Overview
 - This module provides a small, configurable doubly-linked list implementation
   suitable for embedding in headers and instantiating multiple typed variants.

 - Features:
   - Opaque list and node types (instantiable via macro configuration).
   - Basic operations: create/destroy, push/pop front/back, insert/erase,
     get-by-index, node duplication, explicit node allocation.
   - Optional per-node data deallocation hook.

 - Exclusions / boundaries:
   - Does NOT provide thread-safety primitives (caller must synchronize).
   - Does NOT provide iterators in the C++ STL sense; traversal done by
     following node->next/prev via accessor functions.
   - Not optimized for extreme performance (no pooling or specialized allocators)

 - Requirements/constraints:
   - Requires blop/blop.h and C99-compatible compiler.
   - Define BLOP_LIST_IMPLEMENTATION in one translation unit to produce code.
   - If changing BLOP_LIST_DATA_TYPE, ensure any (if given) BLOP_LIST_DEALLOCATE_DATA
     implementation matches that type.

 - Intended usage scenarios:
   - Lightweight lists where you need multiple typed list variants in a single
     codebase without writing separate .c files for each.
   - Embedded or header-only projects that prefer compile-time configuration.

2) Macros, how to fit your needs
 Rule: - Macro — Default — Type — Effect — Notes

   - BLOP_LIST_NAME — listb — identifier — Controls the generated symbol names
     and typedefs. 
     Example: #define BLOP_LIST_NAME mylist

   - BLOP_LIST_DATA_TYPE — uint8_t — type — Type stored in nodes. 
     Example: #define BLOP_LIST_DATA_TYPE uint32_t

   - BLOP_LIST_DEALLOCATE_DATA — (none) — macro(expr) — Called by node destroy
     to free or release node->data. 
     Example: #define BLOP_LIST_DEALLOCATE_DATA(x) free((x))

   - BLOP_LIST_IMPLEMENTATION — (none) — flag — Include in one .c to emit
     implementation bodies.

 - How to override safely:
   - You may define BLOP_LIST_NAME and/or BLOP_LIST_DATA_TYPE per translation
     unit before including the header; symbols are renamed accordingly so you
     can include multiple variants in the same build.

 - Interactions:
   - BLOP_LIST_DEALLOCATE_DATA is consulted only in blopfn_node_destroy when
     a node is not attached to a list; when erasing with deallocate=1 the
     node is destroyed and the macro is used if defined.

3) API conventions and public types
 - Naming conventions:
   - The implementation uses BLOP_LIST_NAME as base to form:
     - typenames: <BLOP_LIST_NAME> and <BLOP_LIST_NAME>_node (via macros)
     - functions: <BLOP_LIST_NAME>_create(), _destroy(), _push_back(), etc.
   - Internal macro helpers follow blop naming (BLOP_CONCAT, BLOP_MALLOC, ...).

 - Public/opaque types:
   - typedef struct _<BLOP_LIST_NAME>_t* <BLOP_LIST_NAME>;
     - Role: opaque handle for a list instance. Lifetime managed by create/destroy.
   - typedef struct _<BLOP_LIST_NAME>_node_t* <BLOP_LIST_NAME>_node;
     - Role: opaque handle to a node containing BLOP_LIST_DATA_TYPE data.

 - Function categories:
   - Constructors/destructors: create, destroy.
   - Accessors: get_front, get_back, get_size, get_node(index), node_get_next/prev/get_data.
   - Mutators: push/pop front/back, insert next/prev, erase, clear.
   - Node helpers: node_create, node_duplicate, node_set_data, node_destroy.

 - Nullability and return conventions:
   - Functions returning pointer types return NULL on invalid input or allocation
     failures.
   - Many functions use BLOP_ASSERT_PTR and may abort when a null or invalid
     input is provided depending on the blop configuration.

4) Memory and ownership
 - All objects allocated by *_create or *_node_create are owned by the caller.
   - List returned by <list>_create(): caller owns and must call
     <list>_destroy() after the list is emptied (destroy requires size == 0).
   - Node returned by <node>_create(): caller owns the node. To attach it to a
     list call push/insert functions; the list then becomes the node's owner in
     terms of membership (node->list points to that list) but memory ownership
     remains with the caller until explicitly destroyed (node must be detached
     or erased with deallocate=1).

 - Deallocation:
   - Call <list>_clear(list, deallocate) with deallocate=1 to deallocate nodes
     using blopfn_node_destroy (which in turn calls BLOP_LIST_DEALLOCATE_DATA
     on the node->data if provided).
   - Call <list>_erase(list, node, deallocate) with deallocate=1 to remove and
     free the node.
   - <list>_destroy(list) will assert/abort if list->size != 0; empty list may
     be freed with this function.

 - Returned accessors:
   - get_front/get_back/get_node return pointers to internal node objects; do
     NOT free them directly; use list erase/destroy or node_destroy when node
     is detached and safe to free.

 - Alignment/size:
   - No special alignment guarantees beyond natural alignment of the platform
     and the data type used (BLOP_LIST_DATA_TYPE).

5) Error handling and safe-mode behavior
 - Errors are reported via the blop infrastructure:
   - Enable Safe Mode: To enable safe mode define BLOP_SAFE_MODE before including
      this header. In safe mode, invalid inputs cause error messages and
      BLOP_ABORT calls (enables the use of BLOP_ASSERT_PTR).
   - Exit over Abort: To use exit(-1) instead of abort define BLOP_EXIT_ON_ERROR
   - Allocation failures: BLOP_ERROR_MESSAGE and BLOP_ABORT used after a failed
     allocation attempt; functions return NULL where applicable.
   - Invalid usage (e.g. passing node that belongs to another list) logs an
     error message and calls BLOP_ABORT; functions then return without effect.

 - Assertions:
   - The code uses BLOP_ASSERT_PTR and related macros to validate inputs.
     Behavior (abort, return, log) depends on the configuration in blop.h.

 - Recoverable vs non-recoverable:
   - Most misuse cases are treated as severe: the module logs and calls
     BLOP_ABORT;

 - Example of checking for allocation failure:
   - blopt_list l = blopfn_list_create();
     if (!l) { handle allocation failure, but the module will likely have already logged and aborted depending on configuration }

6) Thread-safety
 - This module is NOT thread-safe by default.
 - Safety map:
   - Creating and destroying lists: not thread-safe, caller must synchronize.
   - Mutating functions (push/pop/erase/insert/clear): caller must ensure only
     one thread accesses the same list concurrently.
   - Accessors (get_front/get_back/get_size/get_node): safe only if caller
     ensures no concurrent mutation.
 - Recommended pattern:
   - Protect each list instance with a mutex for any concurrent access.
   - For read-heavy scenarios, use a read-write lock or copy-on-write strategy.

7) Common usage patterns and examples
 - Basic create/use/destroy
   Example:
     blopt_list list = blopfn_list_create();
     blopt_node n = blopfn_node_create();
     blopfn_node_set_data(n, (BLOP_LIST_DATA_TYPE)42);
     blopfn_list_push_back(list, n);
     {work with list...}
     blopfn_list_clear(list, 1); // deallocate nodes
     blopfn_list_destroy(list);
   Notes:
     - Use deallocate=1 in clear/erase/pop to free nodes and call optional
       BLOP_LIST_DEALLOCATE_DATA.
     - Always ensure list is empty before calling blopfn_list_destroy.

 - Access by index
   Example:
     blopt_node n = blopfn_list_get_node(list, 0); // gets first node if any
     if (n) val = blopfn_node_get_data(n);

 - Duplicate node (shallow copy of data field)
   Example:
     blopt_node cloned = blopfn_node_duplicate(orig);
     // cloned->list == NULL; can be pushed to another list.
 - Remove node safely without deallocating its memory
   Example:
     blopfn_list_erase(list, node, 0); // node is detached and caller can reuse/free it

8) Troubleshooting and common mistakes
 - Top pitfalls and fixes:
   1) Trying to destroy non-empty list:
      - Symptom: blopfn_list_destroy aborts with an error message.
      - Fix: call blopfn_list_clear(list, 1) or erase nodes first.

   2) Freeing nodes while they belong to a list:
      - Symptom: blopfn_node_destroy aborts (node->list != NULL).
      - Fix: detach/erase the node from its list (with deallocate=1 to free).

   3) Passing node from one list into another without duplicating:
      - Symptom: push/insert rejects with error "node already belongs to a list".
      - Fix: use blopfn_node_duplicate to copy data, then push the duplicate.

   4) Forgetting to define BLOP_LIST_IMPLEMENTATION:
      - Symptom: link errors for function bodies.
      - Fix: define BLOP_LIST_IMPLEMENTATION in exactly one .c file before include.

   5) Wrong BLOP_LIST_DATA_TYPE:
      - Symptom: type mismatches at compile time or incorrect deallocation.
      - Fix: ensure BLOP_LIST_DATA_TYPE is consistent across translation units or
             recompile all units that include a different instantiation.

   6) Expecting thread-safety:
      - Symptom: data races and crashes under concurrent use.
      - Fix: add external synchronization around list usage.

9) Extensive internal documentation (reference)
 - Public types (opaque)
   Type: blopt_list (typedef struct blops_list* blopt_list)
     Summary: Opaque handle to a doubly-linked list instance.
     Lifetime: allocated by blopfn_list_create, freed by blopfn_list_destroy
               (only when list is empty).
     Thread-safety: not thread-safe.
     
   Type: blopt_node (typedef struct blops_node* blopt_node)
     Summary: Opaque handle to a node containing a BLOP_LIST_DATA_TYPE field.
     Lifetime: allocated by blopfn_node_create, freed by blopfn_node_destroy
               (when node->list == NULL) or automatically when erased with
               deallocate=1.
     Note: node->data has type BLOP_LIST_DATA_TYPE.

 - Functions (summary & contracts)
   Function: blopfn_list_create
   Prototype: blopt_list blopfn_list_create(void)
   Summary: Allocate and initialize an empty list. Returns NULL on allocation failure.
   Ownership: caller owns returned pointer and must destroy when done.
   Thread-safety: not thread-safe.

   Function: blopfn_list_destroy
   Prototype: void blopfn_list_destroy(blopt_list list)
   Summary: Destroy an empty list and free its memory. If the list is not empty,
            logs an error and aborts/returns without effect.
   Params: list — non-NULL pointer to a list. Must be empty (size == 0).
   Thread-safety: not thread-safe.

   Function: blopfn_list_get_front
   Prototype: blopt_node blopfn_list_get_front(blopt_list list)
   Summary: Return the front node or NULL if list is empty or list is NULL.
   Notes: Returned node is an internal pointer; do not free directly.

   Function: blopfn_list_get_back
   Prototype: blopt_node blopfn_list_get_back(blopt_list list)
   Summary: Return the back node or NULL.

   Function: blopfn_list_get_size
   Prototype: size_t blopfn_list_get_size(blopt_list list)
   Summary: Return the number of nodes in the list. Returns 0 for NULL if
            configured that way via blop asserts.

   Function: blopfn_list_get_node
   Prototype: blopt_node blopfn_list_get_node(blopt_list list, size_t index)
   Summary: Return the node at the given index (0-based). If index is out of
            bounds, logs an error and returns NULL.
   Complexity: O(min(index, size-index)).

   Function: blopfn_list_clear
   Prototype: void blopfn_list_clear(blopt_list list, int deallocate)
   Summary: Detach all nodes from the list; if deallocate is non-zero, each
            node is destroyed (blopfn_node_destroy called). After this call,
            list->size == 0.
   Side-effects: nodes' next/prev/list pointers are nulled.

   Function: blopfn_list_erase
   Prototype: void blopfn_list_erase(blopt_list list, blopt_node node, int deallocate)
   Summary: Remove a single node from the list. If deallocate != 0, the node
            is also destroyed. If the node does not belong to this list, logs
            an error and returns without effect.

   Function: blopfn_list_pop_back
   Prototype: void blopfn_list_pop_back(blopt_list list, int deallocate)
   Summary: Remove the back node. If deallocate != 0, free it.

   Function: blopfn_list_pop_front
   Prototype: void blopfn_list_pop_front(blopt_list list, int deallocate)
   Summary: Remove the front node. If deallocate != 0, free it.

   Function: blopfn_list_push_back
   Prototype: void blopfn_list_push_back(blopt_list list, blopt_node node)
   Summary: Append node to list's back. Node must not already belong to any list.
            On success node->list is set to list.
   Params: list and node must be non-NULL.

   Function: blopfn_list_push_front
   Prototype: void blopfn_list_push_front(blopt_list list, blopt_node node)
   Summary: Prepend node to front. Same ownership rules as push_back.

   Function: blopfn_list_insert_next
   Prototype: void blopfn_list_insert_next(blopt_list list, blopt_node pivot, blopt_node node)
   Summary: Insert node immediately after pivot. If pivot is the list's back,
            push_back is used. Node must not belong to any list and pivot must
            belong to this list.

   Function: blopfn_list_insert_prev
   Prototype: void blopfn_list_insert_prev(blopt_list list, blopt_node pivot, blopt_node node)
   Summary: Insert node immediately before pivot. If pivot is the front,
            push_front is used.

   Function: blopfn_node_create
   Prototype: blopt_node blopfn_node_create(void)
   Summary: Allocate and return a new node with zeroed pointers and data.
            Returns NULL on allocation failure.

   Function: blopfn_node_duplicate
   Prototype: blopt_node blopfn_node_duplicate(blopt_node node)
   Summary: Allocate a new node and copy the data field from node. Does not
            copy list membership or next/prev pointers. Returns NULL on failure.

   Function: blopfn_node_destroy
   Prototype: void blopfn_node_destroy(blopt_node node)
   Summary: Frees node memory. Node must not belong to any list. If
            BLOP_LIST_DEALLOCATE_DATA is defined, it is called with node->data
            before freeing.
   Params: node must be non-NULL and node->list == NULL.

   Function: blopfn_node_set_data
   Prototype: blopt_node blopfn_node_set_data(blopt_node node, BLOP_LIST_DATA_TYPE data)
   Summary: Set node->data and return node.

   Function: blopfn_node_get_data
   Prototype: BLOP_LIST_DATA_TYPE blopfn_node_get_data(blopt_node node)
   Summary: Return the data stored in node. If node is NULL behavior depends on
            blop assert macro (may abort).

   Function: blopfn_node_get_next / blopfn_node_get_prev
   Prototype: blopt_node blopfn_node_get_next(blopt_node node)
              blopt_node blopfn_node_get_prev(blopt_node node)
   Summary: Return the next/prev pointers or NULL.

 - Macros and compile-time behavior:
   - When BLOP_LIST_IMPLEMENTATION is defined, full definitions for structs and
     function implementations are emitted. Define in one .c file only.
   - The public typedef names and function names are macro-generated and depend
     on BLOP_LIST_NAME to allow multiple independent instantiations.

===============================================================================
End of documentation block
=============================================================================== */

#ifndef BLOP_LIST_NAME
    #define BLOP_LIST_NAME listb
#endif

#ifndef BLOP_LIST_DATA_TYPE
    #define BLOP_LIST_DATA_TYPE uint8_t
#endif

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
        BLOP_ERROR_MESSAGE("Failed to allocate memory for list (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    memset(list, 0, sizeof(struct blops_list));
    return list;
}
void                blopfn_list_destroy(blopt_list list) {
    BLOP_ASSERT_PTR_VOID(list);

    if (list->size > 0) {
        BLOP_ERROR_MESSAGE("A non empty list can not be destroyed, clear the list (returning without effect)");
        BLOP_ABORT();
        return;
    }
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

    if (idx >= list->size) {
        BLOP_OUT_OF_BONDS("Index out of bounds (returning without effect)", idx, list->size);
        BLOP_ABORT();
        return NULL;
    }

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

    if (node->list != list) {
        BLOP_ERROR_MESSAGE("The node does not belong to this list (returning without effect)");
        BLOP_ABORT();
        return;
    }

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
        BLOP_ERROR_MESSAGE("The list is empty (returning without effect)");
        BLOP_ABORT();
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
        BLOP_ERROR_MESSAGE("The list is empty (returning without effect)");
        BLOP_ABORT();
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

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list");
        BLOP_ABORT();
        return;
    }

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

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
        BLOP_ABORT();
        return;
    }

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

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
        BLOP_ABORT();
        return;
    }

    if (pivot->list != list) {
        BLOP_ERROR_MESSAGE("The pivot does not belong to this list (returning without effect)");
        BLOP_ABORT();
        return;
    }

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

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
        BLOP_ABORT();
        return;
    }

    if (pivot->list != list) {
        BLOP_ERROR_MESSAGE("The pivot does not belong to this list (returning without effect)");
        BLOP_ABORT();
        return;
    }

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
        BLOP_ERROR_MESSAGE("Failed to allocate memory for node (returning NULL)");
        BLOP_ABORT();
        return NULL;
    }

    memset(node, 0, sizeof(struct blops_node));
    return node;
}
blopt_node          blopfn_node_duplicate(blopt_node node) {
    BLOP_ASSERT_PTR(node, NULL);

    blopt_node dupnode = BLOP_MALLOC(struct blops_node, 1);
    if (!dupnode) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for node (returning NULL)");
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

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node belongs to a list and it can not be destroyed (returning without effect), you can deallocate a node at erase time.");
        BLOP_ABORT();
        return;
    }

    #ifdef BLOP_LIST_DEALLOCATE_DATA
        BLOP_LIST_DEALLOCATE_DATA(node->data);
    #endif

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

#endif // BLOP_LIST_IMPLEMENTATION

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