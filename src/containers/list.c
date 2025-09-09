#define __BLOP_DEFAULT_CALLBACKS__
#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/blop.h>
#include <blop/list.h>
#include <blop/utils.h>

BlopList  BlopNewList() {
    BlopList list = blop_calloc(struct _BlopList_t, 1);
    return_verbose_if(list == NULL, NULL, "Failed to allocate BlopList")
    list->front = NULL;
    list->back = NULL;
    list->size = 0;
    return list;
}
int       BlopFreeList(BlopList list) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr")
    return_verbose_if(list->size > 0, BlopLogicalException, "To free a BlopList it must be empty")

    blop_free(list);
    return BlopSuccess;
}

BlopNode  BlopNewNode() {
    BlopNode node = blop_calloc(struct _BlopNode_t, 1);
    return_verbose_if(node == NULL, NULL, "Failed to allocate BlopNode")
    node->heap = NULL;
    node->stack = 0;
    node->next = NULL;
    node->prev = NULL;
    node->list = NULL;
    return node;
}
BlopNode  BlopCopyNode(BlopNode node) {
    return_verbose_if(node == NULL, NULL, "BlopNode cant be a NULL ptr")
    BlopNode copy = blop_calloc(struct _BlopNode_t, 1);
    return_verbose_if(copy == NULL, NULL, "Failed to allocate BlopNode")
    copy->heap = node->heap;
    copy->stack = node->stack;
    copy->next = NULL;
    copy->prev = NULL;
    copy->list = NULL;
    return copy;
}
int       BlopFreeNode(BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    return_verbose_if(blop_free(node) != BlopSuccess, BlopLogicalException, "Failed to free BlopNode");
    return BlopSuccess;
}

int       BlopNodeSetHeap(BlopNode node, void* heap) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    node->heap = heap;
    return BlopSuccess;
}
int       BlopNodeSetStack(BlopNode node, long long stack) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    node->stack = stack;
    return BlopSuccess;
}

void*     BlopNodeGetHeap(BlopNode node) {
    return_verbose_if(node == NULL, NULL, "BlopNode cant be a NULL ptr")

    return node->heap;
}
long long BlopNodeGetStack(BlopNode node) {
    return_verbose_if(node == NULL, -1, "BlopNode cant be a NULL ptr")

    return node->stack;
}
BlopNode  BlopNodeGetNext(BlopNode node) {
    return_verbose_if(node == NULL, NULL, "BlopNode cant be a NULL ptr")

    return node->next;
}
BlopNode  BlopNodeGetPrev(BlopNode node) {
    return_verbose_if(node == NULL, NULL, "BlopNode cant be a NULL ptr")

    return node->prev;
}
BlopNode  BlopListGetFront(BlopList list) {
    return_verbose_if(list == NULL, NULL, "BlopList cant be a NULL ptr") 

    return list->front;
}
BlopNode  BlopListGetBack(BlopList list) {
    return_verbose_if(list == NULL, NULL, "BlopList cant be a NULL ptr") 

    return list->back;
}
size_t    BlopListGetSize(BlopList list) {
    return_verbose_if(list == NULL, -1, "BlopList cant be a NULL ptr") 

    return list->size;
}
BlopNode  BlopListGetNode(BlopList list, size_t index) {
    return_verbose_if(list == NULL, NULL, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, NULL, "BlopList must have at least one element") 
    return_verbose_if(index >= list->size, NULL, "Index is out of bounds")

    if (index == 0) return list->front;
    if (index == list->size - 1) return list->back;

    if (index < list->size / 2) {
        BlopNode current = list->front->next;
        for (int i = 1; i < index; i++) {
            current = current->next;
        }
        return current;
    } else {
        BlopNode current = list->back->prev;
        for (int i = list->size - 2; i > index; i--) {
            current = current->prev;
        }
        return current;
    }
}

int       BlopListClear(BlopList list, int deallocate) {
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
int       BlopListErase(BlopList list, BlopNode node, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    return_verbose_if(node->list != list, BlopLogicalException, "BlopNode does not belong to the given BlopList")

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
int       BlopListPopBack(BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, BlopIndexException, "BlopList its empty")

    list->size--;
    struct _BlopNode_t* back = list->back;
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
int       BlopListPopFront(BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, BlopIndexException, "BlopList its empty")

    list->size--;
    struct _BlopNode_t* front = list->front;
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

int       BlopListPushBack(BlopList list, BlopNode node) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr")
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    return_verbose_if(node->list != NULL, BlopLogicalException, "The give BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")

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
int       BlopListPushFront(BlopList list, BlopNode node) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    return_verbose_if(node->list != NULL, BlopLogicalException, "The give BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")

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
int       BlopListInsertNext(BlopList list, BlopNode pivot, BlopNode node) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(pivot == NULL, BlopNullException, "BlopNode (pivot) cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopLogicalException, "BlopNode (node) cant be a NULL ptr") 
    return_verbose_if(node->list != NULL, BlopLogicalException, "The give BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")
    return_verbose_if(pivot->list != list, BlopLogicalException, "The pivot BlopNode does not belong to the given BlopList")

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
int       BlopListInsertPrev(BlopList list, BlopNode pivot, BlopNode node) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(pivot == NULL, BlopNullException, "BlopNode (pivot) cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopLogicalException, "BlopNode (node) cant be a NULL ptr") 
    return_verbose_if(node->list != NULL, BlopLogicalException, "The give BlopNode already belongs to a list, use BlopCopyNode to allocate a new BlopNode ready to insert into another list")
    return_verbose_if(pivot->list != list, BlopLogicalException, "The pivot BlopNode does not belong to the given BlopList")

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