#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <blop/blop.h>

#ifndef BLOP_LIST_NAME
    #define BLOP_LIST_NAME BList
#endif

#ifndef BLOP_LIST_DATA_TYPE
    #define BLOP_LIST_DATA_TYPE uint8_t
#endif

#ifdef BLOP_LIST_SAFE_MODE
    #define BLOP_LIST_ASSERT_PTR(ptr, rt) if (ptr == NULL) {BLOP_ERROR_MESSAGE(#ptr " is a null ptr (returning without effect)"); BLOP_LIST_EXIT; return rt;}
    #define BLOP_LIST_ASSERT_PTR_VOID(ptr) if (ptr == NULL) {BLOP_ERROR_MESSAGE(#ptr " is a null ptr (returning without effect)"); BLOP_LIST_EXIT; return;}
#else
    #define BLOP_LIST_ASSERT_PTR(ptr, rt)
    #define BLOP_LIST_ASSERT_PTR_VOID(ptr)
#endif

#ifdef BLOP_LIST_EXIT_ON_ERROR
    #define BLOP_LIST_EXIT exit(-1)
#else
    #define BLOP_LIST_EXIT
#endif

#ifndef BLOP_LIST_CAMEL

    #define _BLOPS_LIST                BLOP_CONCAT3(_, BLOP_LIST_NAME, _t)
    #define _BLOPS_NODE                BLOP_CONCAT3(_, BLOP_LIST_NAME, _node_t)

    #define _BLOPT_LIST                BLOP_LIST_NAME
    #define _BLOPT_NODE                BLOP_CONCAT2(BLOP_LIST_NAME, _node)

    #define _BLOPF_LIST_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, _create)
    #define _BLOPF_LIST_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, _destroy)
    #define _BLOPF_LIST_GET_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, _get_front)
    #define _BLOPF_LIST_GET_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, _get_back)
    #define _BLOPF_LIST_GET_SIZE       BLOP_CONCAT2(BLOP_LIST_NAME, _get_size)
    #define _BLOPF_LIST_GET_NODE       BLOP_CONCAT2(BLOP_LIST_NAME, _get_node)
    #define _BLOPF_LIST_CLEAR          BLOP_CONCAT2(BLOP_LIST_NAME, _clear)
    #define _BLOPF_LIST_ERASE          BLOP_CONCAT2(BLOP_LIST_NAME, _erase)
    #define _BLOPF_LIST_POP_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, _pop_back)
    #define _BLOPF_LIST_POP_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, _pop_front)
    #define _BLOPF_LIST_PUSH_BACK      BLOP_CONCAT2(BLOP_LIST_NAME, _push_back)
    #define _BLOPF_LIST_PUSH_FRONT     BLOP_CONCAT2(BLOP_LIST_NAME, _push_front)
    #define _BLOPF_LIST_INSERT_NEXT    BLOP_CONCAT2(BLOP_LIST_NAME, _insert_next)
    #define _BLOPF_LIST_INSERT_PREV    BLOP_CONCAT2(BLOP_LIST_NAME, _insert_prev)

    #define _BLOPF_NODE_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, _node_create)
    #define _BLOPF_NODE_DUPLICATE      BLOP_CONCAT2(BLOP_LIST_NAME, _node_duplicate)
    #define _BLOPF_NODE_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, _node_destroy)
    #define _BLOPF_NODE_SET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, _node_set_data)
    #define _BLOPF_NODE_GET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, _node_get_data)
    #define _BLOPF_NODE_GET_NEXT       BLOP_CONCAT2(BLOP_LIST_NAME, _node_get_next)
    #define _BLOPF_NODE_GET_PREV       BLOP_CONCAT2(BLOP_LIST_NAME, _node_get_prev)

#else // BLOP_LIST_CAMEL

    #define _BLOPS_LIST                BLOP_CONCAT3(_, BLOP_LIST_NAME, _t)
    #define _BLOPS_NODE                BLOP_CONCAT3(_, BLOP_LIST_NAME, Node_t)

    #define _BLOPT_LIST                BLOP_LIST_NAME
    #define _BLOPT_NODE                BLOP_CONCAT2(BLOP_LIST_NAME, Node)

    #define _BLOPF_LIST_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, Create)
    #define _BLOPF_LIST_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, Destroy)

    #define _BLOPF_LIST_GET_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, GetFront)
    #define _BLOPF_LIST_GET_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, GetBack)
    #define _BLOPF_LIST_GET_SIZE       BLOP_CONCAT2(BLOP_LIST_NAME, GetSize)
    #define _BLOPF_LIST_GET_NODE       BLOP_CONCAT2(BLOP_LIST_NAME, GetNode)

    #define _BLOPF_LIST_CLEAR          BLOP_CONCAT2(BLOP_LIST_NAME, Clear)
    #define _BLOPF_LIST_ERASE          BLOP_CONCAT2(BLOP_LIST_NAME, Erase)
    #define _BLOPF_LIST_POP_BACK       BLOP_CONCAT2(BLOP_LIST_NAME, PopBack)
    #define _BLOPF_LIST_POP_FRONT      BLOP_CONCAT2(BLOP_LIST_NAME, PopFront)

    #define _BLOPF_LIST_PUSH_BACK      BLOP_CONCAT2(BLOP_LIST_NAME, PushBack)
    #define _BLOPF_LIST_PUSH_FRONT     BLOP_CONCAT2(BLOP_LIST_NAME, PushFront)
    #define _BLOPF_LIST_INSERT_NEXT    BLOP_CONCAT2(BLOP_LIST_NAME, InsertNext)
    #define _BLOPF_LIST_INSERT_PREV    BLOP_CONCAT2(BLOP_LIST_NAME, InsertPrev)

    #define _BLOPF_NODE_CREATE         BLOP_CONCAT2(BLOP_LIST_NAME, NodeCreate)
    #define _BLOPF_NODE_DUPLICATE      BLOP_CONCAT2(BLOP_LIST_NAME, NodeDuplicate)
    #define _BLOPF_NODE_DESTROY        BLOP_CONCAT2(BLOP_LIST_NAME, NodeDestroy)
    #define _BLOPF_NODE_SET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, NodeSetData)
    #define _BLOPF_NODE_GET_DATA       BLOP_CONCAT2(BLOP_LIST_NAME, NodeGetData)
    #define _BLOPF_NODE_GET_NEXT       BLOP_CONCAT2(BLOP_LIST_NAME, NodeGetNext)
    #define _BLOPF_NODE_GET_PREV       BLOP_CONCAT2(BLOP_LIST_NAME, NodeGetPrev)

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BLOPS_LIST* _BLOPT_LIST;
typedef struct _BLOPS_NODE* _BLOPT_NODE;

_BLOPT_LIST         _BLOPF_LIST_CREATE     ();
void                _BLOPF_LIST_DESTROY    (_BLOPT_LIST list);

_BLOPT_NODE         _BLOPF_LIST_GET_FRONT  (_BLOPT_LIST list);
_BLOPT_NODE         _BLOPF_LIST_GET_BACK   (_BLOPT_LIST list);
size_t              _BLOPF_LIST_GET_SIZE   (_BLOPT_LIST list);
_BLOPT_NODE         _BLOPF_LIST_GET_NODE   (_BLOPT_LIST list, size_t index);

void                _BLOPF_LIST_CLEAR      (_BLOPT_LIST list, int deallocate);
void                _BLOPF_LIST_ERASE      (_BLOPT_LIST list, _BLOPT_NODE node, int deallocate);
void                _BLOPF_LIST_POP_BACK   (_BLOPT_LIST list, int deallocate);
void                _BLOPF_LIST_POP_FRONT  (_BLOPT_LIST list, int deallocate);

void                _BLOPF_LIST_PUSH_BACK  (_BLOPT_LIST list, _BLOPT_NODE node);
void                _BLOPF_LIST_PUSH_FRONT (_BLOPT_LIST list, _BLOPT_NODE node);
void                _BLOPF_LIST_INSERT_NEXT(_BLOPT_LIST list, _BLOPT_NODE pivot, _BLOPT_NODE node);
void                _BLOPF_LIST_INSERT_PREV(_BLOPT_LIST list, _BLOPT_NODE pivot, _BLOPT_NODE node);

_BLOPT_NODE         _BLOPF_NODE_CREATE     ();
_BLOPT_NODE         _BLOPF_NODE_DUPLICATE  (_BLOPT_NODE node);
void                _BLOPF_NODE_DESTROY    (_BLOPT_NODE node);
_BLOPT_NODE         _BLOPF_NODE_SET_DATA   (_BLOPT_NODE node, BLOP_LIST_DATA_TYPE data);
BLOP_LIST_DATA_TYPE _BLOPF_NODE_GET_DATA   (_BLOPT_NODE node);
_BLOPT_NODE         _BLOPF_NODE_GET_NEXT   (_BLOPT_NODE node);
_BLOPT_NODE         _BLOPF_NODE_GET_PREV   (_BLOPT_NODE node);

#ifdef BLOP_LIST_IMPLEMENTATION

struct _BLOPS_NODE {
    BLOP_LIST_DATA_TYPE data;
    _BLOPT_NODE         next;
    _BLOPT_NODE         prev;
    _BLOPT_LIST         list;
};
                        
struct _BLOPS_LIST {
    size_t          size;
    _BLOPT_NODE     front;
    _BLOPT_NODE     back;
};

_BLOPT_LIST _BLOPF_LIST_CREATE() {
    _BLOPT_LIST list = BLOP_MALLOC(struct _BLOPS_LIST, 1);
    if (!list) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for list (returning NULL)");
        BLOP_LIST_EXIT;
        return NULL;
    }

    memset(list, 0, sizeof(struct _BLOPS_LIST));
    return list;
}
void _BLOPF_LIST_DESTROY(_BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size > 0) {
        BLOP_ERROR_MESSAGE("A non empty list can not be destroyed (returning without effect), clear the list");
        BLOP_LIST_EXIT;
        return;
    }
    free(list);
}

_BLOPT_NODE _BLOPF_LIST_GET_FRONT(_BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR(list, NULL);
    return list->front;
}
_BLOPT_NODE _BLOPF_LIST_GET_BACK(_BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR(list, NULL);
    return list->back;
}
size_t _BLOPF_LIST_GET_SIZE(_BLOPT_LIST list) {
    BLOP_LIST_ASSERT_PTR(list, 0);
    return list->size;
}
_BLOPT_NODE _BLOPF_LIST_GET_NODE(_BLOPT_LIST list, size_t idx) {
    BLOP_LIST_ASSERT_PTR(list, NULL);

    if (idx >= list->size) {
        BLOP_ERROR_MESSAGE_BONDS("Index out of bounds (returning without effect)", idx, list->size);
        BLOP_LIST_EXIT;
        return NULL;
    }

    _BLOPT_NODE current = NULL;
    if (idx < list->size / 2) {
        current = list->front;
        for (int i = 0; i < idx; i++) {
            current = current->next;
        }
    } else {
        current = list->back;
        for (int i = list->size - 1; i > idx; i--) {
            current = current->prev;
        }
    }
    return current;
}

void _BLOPF_LIST_CLEAR(_BLOPT_LIST list, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size == 0) {
        return;
    }

    _BLOPT_NODE current = list->front;
    _BLOPT_NODE next = NULL;
    while (current) {
        next = current->next;
        current->list = NULL;
        current->next = NULL;
        current->prev = NULL;
        if (deallocate) {
            _BLOPF_NODE_DESTROY(current);
        }
        current = next;
    }

    list->size = 0;
    list->front = NULL;
    list->back = NULL;
}
void _BLOPF_LIST_ERASE(_BLOPT_LIST list, _BLOPT_NODE node, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list != list) {
        BLOP_ERROR_MESSAGE("The node does not belong to this list (returning without effect)");
        BLOP_LIST_EXIT;
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
        _BLOPF_NODE_DESTROY(node);
    }
}
void _BLOPF_LIST_POP_BACK(_BLOPT_LIST list, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size == 0) {
        BLOP_ERROR_MESSAGE("The list is empty (returning without effect)")
        BLOP_LIST_EXIT;
        return;
    }

    _BLOPT_NODE back = list->back;

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
        _BLOPF_NODE_DESTROY(back);
    }
}
void _BLOPF_LIST_POP_FRONT(_BLOPT_LIST list, int deallocate) {
    BLOP_LIST_ASSERT_PTR_VOID(list);

    if (list->size == 0) {
        BLOP_ERROR_MESSAGE("The list is empty (returning without effect)")
        BLOP_LIST_EXIT;
        return;
    }

    _BLOPT_NODE front = list->front;

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
        _BLOPF_NODE_DESTROY(front);
    }
}

void _BLOPF_LIST_PUSH_BACK(_BLOPT_LIST list, _BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
        BLOP_LIST_EXIT;
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
void _BLOPF_LIST_PUSH_FRONT(_BLOPT_LIST list, _BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
        BLOP_LIST_EXIT;
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
void _BLOPF_LIST_INSERT_NEXT(_BLOPT_LIST list, _BLOPT_NODE pivot, _BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(pivot);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
        BLOP_LIST_EXIT;
        return;
    }

    if (pivot->list != list) {
        BLOP_ERROR_MESSAGE("The pivot does not belong to this list (returning without effect)");
        BLOP_LIST_EXIT;
        return;
    }

    if (pivot == list->back) {
        _BLOPF_LIST_PUSH_BACK(list, node);
        return;
    }

    node->list = list;
    node->prev = pivot;
    node->next = pivot->next;

    pivot->next->prev = node;
    pivot->next = node;

    list->size++;
}
void _BLOPF_LIST_INSERT_PREV(_BLOPT_LIST list, _BLOPT_NODE pivot, _BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(list);
    BLOP_LIST_ASSERT_PTR_VOID(pivot);
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node already belongs to a list (returning without effect), duplicate the node to obtain same data without belonging to a list or erase it from the belonging list.");
        BLOP_LIST_EXIT;
        return;
    }

    if (pivot->list != list) {
        BLOP_ERROR_MESSAGE("The pivot does not belong to this list (returning without effect)");
        BLOP_LIST_EXIT;
        return;
    }

    if (pivot == list->front) {
        _BLOPF_LIST_PUSH_FRONT(list, node);
        return;
    }

    node->list = list;
    node->next = pivot;
    node->prev = pivot->prev;

    pivot->prev->next = node;
    pivot->prev = node;

    list->size++;
}

_BLOPT_NODE _BLOPF_NODE_CREATE() {
    _BLOPT_NODE node = BLOP_MALLOC(struct _BLOPS_NODE, 1);
    if (!node) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for node (returning NULL)");
        BLOP_LIST_EXIT;
        return NULL;
    }

    memset(node, 0, sizeof(struct _BLOPS_NODE));
    return node;
}
_BLOPT_NODE _BLOPF_NODE_DUPLICATE(_BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, NULL);

    _BLOPT_NODE dupnode = BLOP_MALLOC(struct _BLOPS_NODE, 1);
    if (!dupnode) {
        BLOP_ERROR_MESSAGE("Failed to allocate memory for node (returning NULL)");
        BLOP_LIST_EXIT;
        return NULL;
    }

    dupnode->next = NULL;
    dupnode->prev = NULL;
    dupnode->list = NULL;
    dupnode->data = node->data;
    return dupnode;
}
void _BLOPF_NODE_DESTROY(_BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR_VOID(node);

    if (node->list) {
        BLOP_ERROR_MESSAGE("The node belongs to a list, it can not be destroyed (returning without effect), you can deallocate a node at erase time.");
        BLOP_LIST_EXIT;
        return;
    }

    #ifdef BLOP_LIST_DEALLOCATE_DATA
        BLOP_LIST_DEALLOCATE_DATA(node->data);
    #endif

    free(node);
}
_BLOPT_NODE _BLOPF_NODE_SET_DATA(_BLOPT_NODE node, BLOP_LIST_DATA_TYPE data) {
    BLOP_LIST_ASSERT_PTR_VOID(node);
    node->data = data;
    return node;
}
BLOP_LIST_DATA_TYPE _BLOPF_NODE_GET_DATA(_BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, (BLOP_LIST_DATA_TYPE)0);
    return node->data;
}
_BLOPT_NODE _BLOPF_NODE_GET_NEXT(_BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, NULL);
    return node->next;
}
_BLOPT_NODE _BLOPF_NODE_GET_PREV(_BLOPT_NODE node) {
    BLOP_LIST_ASSERT_PTR(node, NULL);
    return node->prev;
}

#endif // BLOP_LIST_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#undef BLOP_LIST_SAFE_MODE
#undef BLOP_LIST_EXIT_ON_ERROR
#undef BLOP_LIST_ASSERT_PTR
#undef BLOP_LIST_EXIT

#undef BLOP_LIST_NAME
#undef BLOP_LIST_DATA_TYPE
#undef BLOP_LIST_DEALLOCATE_DATA

#undef BLOP_LIST_CAMEL
#undef BLOP_LIST_IMPLEMENTATION

#undef _BLOPS_LIST               
#undef _BLOPS_NODE               

#undef _BLOPT_LIST               
#undef _BLOPT_NODE   

#undef _BLOPF_LIST_CREATE        
#undef _BLOPF_LIST_DESTROY       
#undef _BLOPF_LIST_GET_FRONT     
#undef _BLOPF_LIST_GET_BACK      
#undef _BLOPF_LIST_GET_SIZE      
#undef _BLOPF_LIST_GET_NODE      
#undef _BLOPF_LIST_CLEAR         
#undef _BLOPF_LIST_ERASE         
#undef _BLOPF_LIST_POP_BACK      
#undef _BLOPF_LIST_POP_FRONT     
#undef _BLOPF_LIST_PUSH_BACK     
#undef _BLOPF_LIST_PUSH_FRONT    
#undef _BLOPF_LIST_INSERT_NEXT   
#undef _BLOPF_LIST_INSERT_PREV   

#undef _BLOPF_NODE_CREATE        
#undef _BLOPF_NODE_DUPLICATE     
#undef _BLOPF_NODE_DESTROY       
#undef _BLOPF_NODE_SET_DATA      
#undef _BLOPF_NODE_GET_DATA      
#undef _BLOPF_NODE_GET_NEXT      
#undef _BLOPF_NODE_GET_PREV      