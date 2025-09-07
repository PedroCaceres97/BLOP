#define __BLOP_DEFAULT_CALLBACKS__
#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/blop.h>
#include <blop/list.h>

BlopList  BlopNewList() {
    BlopList list = blop_alloc(struct _BlopList_t, 1);
    return_verbose_if(list == NULL, "Failed to allocate BlopList")
    list->head = NULL;
    list->tail = NULL;
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
    BlopNode node = blop_alloc(struct _BlopNode_t, 1);
    return_verbose_if(node == NULL, "Failed to allocate BlopNode")
    node->heap = NULL;
    node->stack = 0;
    node->next = NULL;
    node->prev = NULL;
    node->list = NULL;
    return node;
}
BlopNode  BlopCopyNode(BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    BlopNode copy = blop_alloc(struct _BlopNode_t, 1);
    return_verbose_if(copy == NULL, "Failed to allocate BlopNode")
    copy->heap = node->heap;
    copy->stack = node->stack;
    copy->next = NULL;
    copy->prev = NULL;
    copy->list = NULL;
    return copy;
}
int       BlopFreeNode(BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    blop_free(node);
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
int       BlopNodeSetNext(BlopNode node, BlopNode next) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    node->next = next;
    return BlopSuccess;
}
int       BlopNodeSetPrev(BlopNode node, BlopNode prev) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr") 

    node->prev = prev;
    return BlopSuccess;
}
int       BlopListSetHead(BlopList list, BlopNode head) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 

    list->head = head;
    return BlopSuccess;
}
int       BlopListSetTail(BlopList list, BlopNode tail) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 

    list->tail = tail;
    return BlopSuccess;
}

void*     BlopNodeGetHeap(BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    return node->heap;
}
long long BlopNodeGetStack(BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    return node->stack;
}
BlopNode  BlopNodeGetNext(BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    return node->next;
}
BlopNode  BlopNodeGetPrev(BlopNode node) {
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")

    return node->prev;
}
BlopNode  BlopListGetHead(BlopList list) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 

    return list->head;
}
BlopNode  BlopListGetTail(BlopList list) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 

    return list->tail;
}
BlopNode  BlopListGetNode(BlopList list, size_t index) {
    return_verbose_if(list == NULL, NULL, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, NULL, "BlopList must have at least one element") 
    return_verbose_if(index >= list->size, NULL, "Index is out of bounds")

    if (index == 0) return list->head;
    if (index == list->size - 1) return list->tail;

    if (index < list->size / 2) {
        BlopNode current = list->head->next;
        for (int i = 1; i < index; i++) {
            current = current->next;
        }
        return current;
    } else {
        BlopNode current = list->tail->prev;
        for (int i = list->size - 2; i > index; i--) {
            current = current->prev;
        }
        return current;
    }
}

int       BlopListClear(BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr")

    BlopNode current = list->head;
    while (current) {
        BlopNode next = current->next;
        current->list = NULL;
        blop_free_if(current, deallocate);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return BlopSuccess;
}
int       BlopListErase(BlopList list, BlopNode node, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(node == NULL, BlopNullException, "BlopNode cant be a NULL ptr")
    return_verbose_if(node->list != list, BlopLogicalException, "BlopNode does not belong to the given BlopList")

    list->size--;
    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    } else if (node == list->head) {
        list->head = list->head->next;
        list->head->prev = NULL;
    } else if (node == list->tail) {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node->list = NULL;
    blop_free_if(node, deallocate)
    return BlopSuccess;
}
int       BlopListPopBack(BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, BlopLogicalException, "BlopList its empty")

    list->size--;
    struct _BlopNode_t* back = list->tail;
    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }

    back->list = NULL;
    blop_free_if(back, deallocate)
    return BlopSuccess;
}
int       BlopListPopFront(BlopList list, int deallocate) {
    return_verbose_if(list == NULL, BlopNullException, "BlopList cant be a NULL ptr") 
    return_verbose_if(list->size == 0, BlopLogicalException, "BlopList its empty")

    list->size--;
    struct _BlopNode_t* front = list->head;
    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    front->list = NULL;
    blop_free_if(front, deallocate)
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
        list->head = node;
        list->tail = node;
        list->head->prev = NULL;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
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
        list->head = node;
        list->tail = node;
        list->head->next = NULL;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
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
    
    if (pivot == list->tail) {
        list->tail = node;
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
    
    if (pivot == list->head) {
        list->head = node;
    } else {
        pivot->prev->next = node;
    }
    
    pivot->prev = node;
    return BlopSuccess;
}