#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "blop_basics.h"

#ifndef BLOP_LIST_NAME
    #define BLOP_LIST_NAME blop
#endif

#ifndef BLOP_LIST_DATA_TYPE
    #define BLOP_LIST_DATA_TYPE void*
#endif

#ifndef BLOP_LIST_DEALLOCATE_DATA
    #define BLOP_LIST_DEALLOCATE_DATA(ptr)
#endif

#ifdef BLOP_LIST_SAFE_MODE
    #define BLOP_LIST_ASSERT_PTR(ptr, rt) if (ptr == NULL) {printf("[BLOP {list.h}]: " #ptr " is a null ptr (returning without effect)"); return rt;}
    #define BLOP_LIST_ASSERT_PTR_VOID(ptr) if (ptr == NULL) {printf("[BLOP {list.h}]: " #ptr " is a null ptr (returning without effect)"); return;}
#else
    #define BLOP_LIST_ASSERT_PTR(ptr, rt)
    #define BLOP_LIST_ASSERT_PTR_VOID(ptr)
#endif

#ifndef BLOP_LIST_CAMEL

    #define __BLOPS_LIST                BLOP_CONCAT3(_, BLOP_LIST_NAME, _t)
    #define __BLOPS_NODE                BLOP_CONCAT3(_, BLOP_LIST_NAME, _node_t)

    #define __BLOPT_LIST                BLOP_LIST_NAME
    #define __BLOPT_NODE                BLOP_CONCAT2(BLOP_LIST_NAME, _node)

    #define __BLOPF_LIST_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, _create)
    #define __BLOPF_LIST_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, _destroy)
    #define __BLOPF_LIST_GET_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, _get_front)
    #define __BLOPF_LIST_GET_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, _get_back)
    #define __BLOPF_LIST_GET_SIZE       BLOP_CONCAT2(BLOP_LIST_NAME, _get_size)
    #define __BLOPF_LIST_GET_NODE       BLOP_CONCAT2(BLOP_LIST_NAME, _get_node)
    #define __BLOPF_LIST_CLEAR          BLOP_CONCAT2(BLOP_LIST_NAME, _clear)
    #define __BLOPF_LIST_ERASE          BLOP_CONCAT2(BLOP_LIST_NAME, _erase)
    #define __BLOPF_LIST_POP_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, _pop_back)
    #define __BLOPF_LIST_POP_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, _pop_front)
    #define __BLOPF_LIST_PUSH_BACK      BLOP_CONCAT2(BLOP_LIST_NAME, _push_back)
    #define __BLOPF_LIST_PUSH_FRONT     BLOP_CONCAT2(BLOP_LIST_NAME, _push_front)
    #define __BLOPF_LIST_INSERT_NEXT    BLOP_CONCAT2(BLOP_LIST_NAME, _insert_next)
    #define __BLOPF_LIST_INSERT_PREV    BLOP_CONCAT2(BLOP_LIST_NAME, _insert_prev)

    #define __BLOPF_NODE_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, _node_create)
    #define __BLOPF_NODE_DUPLICATE      BLOP_CONCAT2(BLOP_LIST_NAME, _node_duplicate)
    #define __BLOPF_NODE_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, _node_destroy)
    #define __BLOPF_NODE_SET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, _node_set_data)
    #define __BLOPF_NODE_GET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, _node_get_data)
    #define __BLOPF_NODE_GET_NEXT       BLOP_CONCAT2(BLOP_LIST_NAME, _node_get_next)
    #define __BLOPF_NODE_GET_PREV       BLOP_CONCAT2(BLOP_LIST_NAME, _node_get_prev)

#else // BLOP_LIST_CAMEL

    #define __BLOPS_LIST                BLOP_CONCAT3(_, BLOP_LIST_NAME, _t)
    #define __BLOPS_NODE                BLOP_CONCAT3(_, BLOP_LIST_NAME, Node_t)

    #define __BLOPT_LIST                BLOP_LIST_NAME
    #define __BLOPT_NODE                BLOP_CONCAT2(BLOP_LIST_NAME, Node)

    #define __BLOPF_LIST_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, Create)
    #define __BLOPF_LIST_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, Destroy)

    #define __BLOPF_LIST_GET_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, GetFront)
    #define __BLOPF_LIST_GET_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, GetBack)
    #define __BLOPF_LIST_GET_SIZE       BLOP_CONCAT2(BLOP_LIST_NAME, GetSize)
    #define __BLOPF_LIST_GET_NODE       BLOP_CONCAT2(BLOP_LIST_NAME, GetNode)

    #define __BLOPF_LIST_CLEAR          BLOP_CONCAT2(BLOP_LIST_NAME, Clear)
    #define __BLOPF_LIST_ERASE          BLOP_CONCAT2(BLOP_LIST_NAME, Erase)
    #define __BLOPF_LIST_POP_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, PopBack)
    #define __BLOPF_LIST_POP_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, PopFront)

    #define __BLOPF_LIST_PUSH_BACK      BLOP_CONCAT2(BLOP_LIST_NAME, PushBack)
    #define __BLOPF_LIST_PUSH_FRONT     BLOP_CONCAT2(BLOP_LIST_NAME, PushFront)
    #define __BLOPF_LIST_INSERT_NEXT    BLOP_CONCAT2(BLOP_LIST_NAME, InsertNext)
    #define __BLOPF_LIST_INSERT_PREV    BLOP_CONCAT2(BLOP_LIST_NAME, InsertPrev)

    #define __BLOPF_NODE_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, NodeCreate)
    #define __BLOPF_NODE_DUPLICATE      BLOP_CONCAT2(BLOP_LIST_NAME, NodeDuplicate)
    #define __BLOPF_NODE_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, NodeDestroy)
    #define __BLOPF_NODE_SET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, NodeSetData)
    #define __BLOPF_NODE_GET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, NodeGetData)
    #define __BLOPF_NODE_GET_NEXT       BLOP_CONCAT2(BLOP_LIST_NAME, NodeGetNext)
    #define __BLOPF_NODE_GET_PREV       BLOP_CONCAT2(BLOP_LIST_NAME, NodeGetPrev)

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __BLOPS_LIST* __BLOPT_LIST;
typedef struct __BLOPS_NODE* __BLOPT_NODE;

__BLOPT_LIST        __BLOPF_LIST_CREATE     ();
void                __BLOPF_LIST_DESTROY    (__BLOPT_LIST list);

__BLOPT_NODE        __BLOPF_LIST_GET_FRONT  (__BLOPT_LIST list);
__BLOPT_NODE        __BLOPF_LIST_GET_BACK   (__BLOPT_LIST list);
size_t              __BLOPF_LIST_GET_SIZE   (__BLOPT_LIST list);
__BLOPT_NODE        __BLOPF_LIST_GET_NODE   (__BLOPT_LIST list, size_t index);

void                __BLOPF_LIST_CLEAR      (__BLOPT_LIST list, int deallocate);
void                __BLOPF_LIST_ERASE      (__BLOPT_LIST list, __BLOPT_NODE node, int deallocate);
void                __BLOPF_LIST_POP_BACK   (__BLOPT_LIST list, int deallocate);
void                __BLOPF_LIST_POP_FRONT  (__BLOPT_LIST list, int deallocate);

void                __BLOPF_LIST_PUSH_BACK  (__BLOPT_LIST list, __BLOPT_NODE node);
void                __BLOPF_LIST_PUSH_FRONT (__BLOPT_LIST list, __BLOPT_NODE node);
void                __BLOPF_LIST_INSERT_NEXT(__BLOPT_LIST list, __BLOPT_NODE pivot, __BLOPT_NODE node);
void                __BLOPF_LIST_INSERT_PREV(__BLOPT_LIST list, __BLOPT_NODE pivot, __BLOPT_NODE node);

__BLOPT_NODE        __BLOPF_NODE_CREATE     ();
__BLOPT_NODE        __BLOPF_NODE_DUPLICATE  (__BLOPT_NODE node);
void                __BLOPF_NODE_DESTROY    (__BLOPT_NODE node);
void                __BLOPF_NODE_SET_DATA   (__BLOPT_NODE node, BLOP_LIST_DATA_TYPE data);
BLOP_LIST_DATA_TYPE __BLOPF_NODE_GET_DATA   (__BLOPT_NODE node);
__BLOPT_NODE        __BLOPF_NODE_GET_NEXT   (__BLOPT_NODE node);
__BLOPT_NODE        __BLOPF_NODE_GET_PREV   (__BLOPT_NODE node);

#ifdef BLOP_INCLUDE_IMPLEMENTATION

struct __BLOPS_NODE {
    BLOP_LIST_DATA_TYPE data;
    __BLOPT_NODE        next;
    __BLOPT_NODE        prev;
    __BLOPT_LIST        list;
};
                        
struct __BLOPS_LIST {
    size_t          size;
    __BLOPT_NODE    front;
    __BLOPT_NODE    back;
};

__BLOPT_LIST __BLOPF_LIST_CREATE() {
    __BLOPT_LIST list = (__BLOPT_LIST)calloc(1, sizeof(struct __BLOPS_LIST));
    if (!list) {
        printf("[BLOP {list.h}]: Failed to allocate memory for list (returning NULL)\n");
        return NULL;
    }
    return list;
}
void __BLOPF_LIST_DESTROY(__BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size > 0) {
        printf("[BLOP {list.h}]: A non empty list can not be destroyed (returning without effect), clear the list\n");
        return;
    }
    free(list);
}

__BLOPT_NODE __BLOPF_LIST_GET_FRONT(__BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR(list, NULL);
    return list->front;
}
__BLOPT_NODE __BLOPF_LIST_GET_BACK(__BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR(list, NULL);
    return list->back;
}
size_t __BLOPF_LIST_GET_SIZE(__BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR(list, 0);
    return list->size;
}
__BLOPT_NODE __BLOPF_LIST_GET_NODE(__BLOPT_LIST list, size_t index) {
    BLOP_LIST_ASSERT_PTR(list, NULL);

    if (index >= list->size) {
        printf("[BLOP {list.h}]: Out of bounds (returning NULL)\n");
        return NULL;
    }

    __BLOPT_NODE current = NULL;
    if (index < list->size / 2) {
        current = list->front;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = list->back;
        for (int i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }
    return current;
}

void __BLOPF_LIST_CLEAR(__BLOPT_LIST list, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size == 0) {
        return;
    }

    __BLOPT_NODE current = list->front;
    __BLOPT_NODE next = NULL;
    while (current) {
        next = current->next;
        current->list = NULL;
        current->next = NULL;
        current->prev = NULL;
        if (deallocate) {
            __BLOPF_NODE_DESTROY(current);
        }
        current = next;
    }

    list->size = 0;
    list->front = NULL;
    list->back = NULL;
}
void __BLOPF_LIST_ERASE(__BLOPT_LIST list, __BLOPT_NODE node, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list != list) {
        printf("[BLOP {list.h}]: The node does not belong to this list (returning without effect)\n");
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
        __BLOPF_NODE_DESTROY(node);
    }
}
void __BLOPF_LIST_POP_BACK(__BLOPT_LIST list, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size == 0) {
        printf("[BLOP {list.h}]: The list is empty (returning without effect)\n");
        return;
    }

    __BLOPT_NODE back = list->back;

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
        __BLOPF_NODE_DESTROY(back);
    }
}
void __BLOPF_LIST_POP_FRONT(__BLOPT_LIST list, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size == 0) {
        printf("[BLOP {list.h}]: The list is empty (returning without effect)\n");
        return;
    }

    __BLOPT_NODE front = list->front;

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
        __BLOPF_NODE_DESTROY(front);
    }
}

void __BLOPF_LIST_PUSH_BACK(__BLOPT_LIST list, __BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        printf("[BLOP {list.h}]: The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.\n");
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
void __BLOPF_LIST_PUSH_FRONT(__BLOPT_LIST list, __BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        printf("[BLOP {list.h}]: The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.\n");
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
void __BLOPF_LIST_INSERT_NEXT(__BLOPT_LIST list, __BLOPT_NODE pivot, __BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(pivot);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        printf("[BLOP {list.h}]: The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.\n");
        return;
    }

    if (list->size == 0) {
        __BLOPF_LIST_PUSH_BACK(list, node);
        return;
    }

    if (pivot->list != list) {
        printf("[BLOP {list.h}]: The pivot does not belong to this list (returning without effect)\n");
        return;
    }

    node->list = list;
    node->prev = pivot;
    node->next = pivot->next;

    if (pivot->next) {
        pivot->next->prev = node;
        pivot->next = node;
    } else {
        pivot->next = node;
        list->back = node;
    }

    list->size++;
}
void __BLOPF_LIST_INSERT_PREV(__BLOPT_LIST list, __BLOPT_NODE pivot, __BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(pivot);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        printf("[BLOP {list.h}]: The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.\n");
        return;
    }

    if (list->size == 0) {
        __BLOPF_LIST_PUSH_BACK(list, node);
        return;
    }

    if (pivot->list != list) {
        printf("[BLOP {list.h}]: The pivot does not belong to this list (returning without effect)\n");
        return;
    }

    node->list = list;
    node->next = pivot;
    node->prev = pivot->prev;

    if (pivot->prev) {
        pivot->prev->next = node;
        pivot->prev = node;
    } else {
        pivot->prev = node;
        list->front = node;
    }

    list->size++;
}

__BLOPT_NODE __BLOPF_NODE_CREATE() {
    __BLOPT_NODE node = (__BLOPT_NODE)calloc(1, sizeof(struct __BLOPS_NODE));
    if (!node) {
        printf("[BLOP {list.h}]: Failed to allocate memory for node (returning NULL)\n");
        return NULL;
    }
    return node;
}
__BLOPT_NODE __BLOPF_NODE_DUPLICATE(__BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, NULL);

    __BLOPT_NODE dupnode = (__BLOPT_NODE)calloc(1, sizeof(struct __BLOPS_NODE));
    if (!dupnode) {
        printf("[BLOP {list.h}]: Failed to allocate memory for node (returning NULL)\n");
        return NULL;
    }
    dupnode->data = node->data;
    return dupnode;
}
void __BLOPF_NODE_DESTROY(__BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        printf("[BLOP {list.h}]: The node belongs to a list, it can not be destroyed (returning without effect), you can deallocate a node at erase time.\n");
        return;
    }
    BLOP_LIST_DEALLOCATE_DATA(node->data);
    free(node);
}
void __BLOPF_NODE_SET_DATA(__BLOPT_NODE node, BLOP_LIST_DATA_TYPE data) {
    BLOP_LIST_ASSERT_PTR_VOID(node);
    node->data = data;
}
BLOP_LIST_DATA_TYPE __BLOPF_NODE_GET_DATA(__BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, (BLOP_LIST_DATA_TYPE)0);
    return node->data;
}
__BLOPT_NODE __BLOPF_NODE_GET_NEXT(__BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, NULL);
    return node->next;
}
__BLOPT_NODE __BLOPF_NODE_GET_PREV(__BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, NULL);
    return node->prev;
}

#endif

#ifdef __cplusplus
}
#endif

#undef BLOP_LIST_SAFE_MODE
#undef BLOP_LIST_ASSERT_PTR

#undef BLOP_LIST_NAME
#undef BLOP_LIST_DATA_TYPE
#undef BLOP_LIST_DEALLOCATE_DATA

#undef __BLOPS_LIST               
#undef __BLOPS_NODE               

#undef __BLOPT_LIST               
#undef __BLOPT_NODE   

#undef BLOP_LIST_CAMEL

#undef __BLOPF_LIST_CREATE        
#undef __BLOPF_LIST_DESTROY       
#undef __BLOPF_LIST_GET_FRONT     
#undef __BLOPF_LIST_GET_BACK      
#undef __BLOPF_LIST_GET_SIZE      
#undef __BLOPF_LIST_GET_NODE      
#undef __BLOPF_LIST_CLEAR         
#undef __BLOPF_LIST_ERASE         
#undef __BLOPF_LIST_POP_BACK      
#undef __BLOPF_LIST_POP_FRONT     
#undef __BLOPF_LIST_PUSH_BACK     
#undef __BLOPF_LIST_PUSH_FRONT    
#undef __BLOPF_LIST_INSERT_NEXT   
#undef __BLOPF_LIST_INSERT_PREV   

#undef __BLOPF_NODE_CREATE        
#undef __BLOPF_NODE_DUPLICATE     
#undef __BLOPF_NODE_DESTROY       
#undef __BLOPF_NODE_SET_DATA      
#undef __BLOPF_NODE_GET_DATA      
#undef __BLOPF_NODE_GET_NEXT      
#undef __BLOPF_NODE_GET_PREV      