#define __BLOP_DEFAULT_CALLBACKS__
#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>
#include <blop/error.h>
#include <blop/stdmacro.h>

BLIST       BLIST_Create    () {
    BLIST list = stdm_calloc(struct _BLIST_t, 1);
    if (list == NULL) {
        BERROR_SetLast(BlopAllocationFailed);
        return NULL;
    }

    memset(list, 0, sizeof(struct _BLIST_t));
    return list;
}
void        BLIST_Destroy   (BLIST list) {
    if (list->size > 0) {
        BERROR_SetLast(BlopNonEmptyStructure);
        return;
    }

    stdm_free(list);
}

BNODE       BNODE_Create    ();
BNODE       BNODE_Duplicate (BNODE node);
void        BNODE_Destroy   (BNODE node);

// The stack parameter lets you store a value by copying it instead of having a ptr
void        BNODE_SetStack  (BNODE node, long long stack);
void        BNODE_SetHeap   (BNODE node, void* heap);

// The stack parameter lets you store a value by copying it instead of having a ptr
long long   BNODE_GetStack  (BNODE node);
void*       BNODE_GetHeap   (BNODE node);
BNODE       BNODE_GetNext   (BNODE node);
BNODE       BNODE_GetPrev   (BNODE node);
BNODE       BLIST_GetFront  (BLIST list);
BNODE       BLIST_GetBack   (BLIST list);
size_t      BLIST_GetSize   (BLIST list);
BNODE       BLIST_GetNode   (BLIST list, size_t index);

void        BLIST_Clear     (BLIST list, int deallocate);
void        BLIST_Erase     (BLIST list, BNODE node, int deallocate);
void        BLIST_PopBack   (BLIST list, int deallocate);
void        BLIST_PopFront  (BLIST list, int deallocate);

void        BLIST_PushBack  (BLIST list, BNODE node);
void        BLIST_PushFront (BLIST list, BNODE node);
void        BLIST_InsertNext(BLIST list, BNODE pivot, BNODE node);
void        BLIST_InsertPrev(BLIST list, BNODE pivot, BNODE node);

BlopResult BlopNewList       (BlopList* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    *buffer = blop_calloc(struct _BlopList_t, 1);
    return_verbose_if(*buffer == NULL, BlopAllocationFailed, "Failed to allocate BlopList")
    *buffer->front = NULL;
    *buffer->back = NULL;
    *buffer->size = 0;
    return BlopSuccess;
}
BlopResult BlopFreeList      (BlopList list) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr")
    return_verbose_if(list->size > 0, BlopNonEmptyStructure, "To free a BlopList it must be empty")

    blop_free(list);
    return BlopSuccess;
}

BlopResult BlopNewNode       (BlopNode* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    *buffer = blop_calloc(struct _BlopNode_t, 1);
    return_verbose_if(node == NULL, BlopAllocationFailed, "Failed to allocate BlopNode")
    *buffer->heap = NULL;
    *buffer->stack = 0;
    *buffer->next = NULL;
    *buffer->prev = NULL;
    *buffer->list = NULL;
    return BlopSuccess;
}
BlopResult BlopCopyNode      (BlopNode node, BlopNode* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    *buffer = blop_calloc(struct _BlopNode_t, 1);
    return_verbose_if(*buffer == NULL, BlopAllocationFailed, "Failed to allocate BlopNode")
    *buffer->heap = node->heap;
    *buffer->stack = node->stack;
    *buffer->next = NULL;
    *buffer->prev = NULL;
    *buffer->list = NULL;
    return BlopSuccess;
}
BlopResult BlopFreeNode      (BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    return_verbose_if(blop_free(node) != BlopSuccess, BlopDeallocationFailed, "Failed to free BlopNode");
    return BlopSuccess;
}

// The stack parameter lets you store a value by copying it instead of having a ptr
BlopResult BlopNodeSetStack  (BlopNode node, long long stack) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    node->heap = heap;
    return BlopSuccess;
}
BlopResult BlopNodeSetHeap   (BlopNode node, void* heap) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    node->stack = stack;
    return BlopSuccess;
}

// The stack parameter lets you store a value by copying it instead of having a ptr
BlopResult BlopNodeGetStack  (BlopNode node, long long* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    *buffer = node->stack;
    return BlopSuccess;
}
BlopResult BlopNodeGetHeap   (BlopNode node, void** buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    *buffer = node->heap;
    return BlopSuccess;
}
BlopResult BlopNodeGetNext   (BlopNode node, BlopNode* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    *buffer = node->next;
    return BlopSuccess;
}
BlopResult BlopNodeGetPrev   (BlopNode node, BlopNode* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    *buffer = node->prev;
    return BlopSuccess;
}
BlopResult BlopListGetFront  (BlopList list, BlopNode* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    *buffer = list->front;
    return BlopSuccess;
}
BlopResult BlopListGetBack   (BlopList list, BlopNode* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    *buffer = list->back;
    return BlopSuccess;
}
BlopResult BlopListGetSize   (BlopList list, size_t* buffer) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    *buffer = list->size;
    return BlopSuccess;
}
BlopResult BlopListGetNode   (BlopList list, BlopNode* buffer, size_t index) {
    return_verbose_if(buffer == NULL, BlopNullException, "buffer cant be a NULL ptr")
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, BlopIndexException, "BlopList must have at least one element") 
    return_verbose_if(index >= list->size, BlopIndexException, "Index is out of bounds")

    if (index == 0) {
        *buffer = list->front;
        return BlopSuccess;
    }
    if (index == list->size - 1) {
        *buffer = list->back;
        return BlopSuccess;
    }

    BlopNode current = NULL;
    if (index < list->size / 2) {
        current = list->front->next;
        for (int i = 1; i < index; i++) {
            current = current->next;
        }
    } else {
        current = list->back->prev;
        for (int i = list->size - 2; i > index; i--) {
            current = current->prev;
        }

    }

    *buffer = current;
    return BlopSuccess;
}

BlopResult BlopListClear     (BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr")

    BlopNode current = list->front;
    while (current) {
        BlopNode next = current->next;
        current->list = NULL;
        blop_free_if(deallocate, current);
        current = next;
    }
    list->front = NULL;
    list->back = NULL;
    list->size = 0;
    return BlopSuccess;
}
BlopResult BlopListErase     (BlopList list, BlopNode node, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    return_verbose_if(node->list != list, BlopWrongSignature, "BlopNode does not belong to the given BlopList")

    list->size--;
    if (list->size == 0) {
        list->front = NULL;
        list->back = NULL;
    } else if (node == list->front) {
        list->front = list->front->next;
        list->front->prev = NULL;
    } else if (node == list->back) {
        list->back = list->back->prev;
        list->back->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node->list = NULL;
    blop_free_if(deallocate, node)
    return BlopSuccess;
}
BlopResult BlopListPopBack   (BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, BlopIndexException, "BlopList its empty")

    list->size--;
    BlopNode* back = list->back;
    if (list->size == 0) {
        list->front = NULL;
        list->back = NULL;
    } else {
        list->back = list->back->prev;
        list->back->next = NULL;
    }

    back->list = NULL;
    blop_free_if(deallocate, back)
    return BlopSuccess;
}
BlopResult BlopListPopFront  (BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, BlopIndexException, "BlopList its empty")

    list->size--;
    BlopNode* front = list->front;
    if (list->size == 0) {
        list->front = NULL;
        list->back = NULL;
    } else {
        list->front = list->front->next;
        list->front->prev = NULL;
    }

    front->list = NULL;
    blop_free_if(deallocate, front)
    return BlopSuccess;
}

BlopResult BlopListPushBack  (BlopList list, BlopNode node) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    return_verbose_if(node->list != NULL, BlopWrongSignature, "The give BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")

    list->size++;
    node->list = list;
    node->next = NULL;

    if (list->size == 1) {
        list->front = node;
        list->back = node;
        list->front->prev = NULL;
    } else {
        node->prev = list->back;
        list->back->next = node;
        list->back = node;
    }

    return BlopSuccess;
}
BlopResult BlopListPushFront (BlopList list, BlopNode node) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    return_verbose_if(node->list != NULL, BlopWrongSignature, "The give BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")

    list->size++;
    node->list = list;
    node->prev = NULL;

    if (list->size == 1) {
        list->front = node;
        list->back = node;
        list->front->next = NULL;
    } else {
        node->next = list->front;
        list->front->prev = node;
        list->front = node;
    }

    return BlopSuccess;
}
BlopResult BlopListInsertNext(BlopList list, BlopNode pivot, BlopNode node) {
    return_verbose_if(list == NULL,  BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(pivot == NULL, BlopNullException, "BlopNode (pivot) cant be a NULL ptr") 
    return_verbose_if(node == NULL,  BlopNulllException, "BlopNode (node) cant be a NULL ptr") 
    return_verbose_if(node->list != NULL,  BlopWrongSignature, "The given BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")
    return_verbose_if(pivot->list != list, BlopWrongSignature, "The pivot BlopNode does not belong to the given BlopList")

    list->size++;
    node->list = list;
    node->prev = pivot;
    node->next = pivot->next;
    
    if (pivot == list->back) {
        list->back = node;
    } else {
        pivot->next->prev = node;
    }
    
    pivot->next = node;
    return BlopSuccess;
}
BlopResult BlopListInsertPrev(BlopList list, BlopNode pivot, BlopNode node) {
    return_verbose_if(list == NULL,  BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(pivot == NULL, BlopNullException, "BlopNode (pivot) cant be a NULL ptr") 
    return_verbose_if(node == NULL,  BlopNullException, "BlopNode (node) cant be a NULL ptr") 
    return_verbose_if(node->list != NULL,  BlopWrongSignature, "The given BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")
    return_verbose_if(pivot->list != list, BlopWrongSignature, "The pivot BlopNode does not belong to the given BlopList")

    list->size++;
    node->list = list;
    node->next = pivot;
    node->prev = pivot->prev;
    
    if (pivot == list->front) {
        list->front = node;
    } else {
        pivot->prev->next = node;
    }
    
    pivot->prev = node;
    return BlopSuccess;
}