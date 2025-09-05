#include "BlopList.h"

static void DefaultErrorCallback(const char* file, uint32_t line, const char* function, const char* message, const char* solution) {
    printf("[BLOP ERROR]:\nFile: %s\nLine: %u\nFunction: %s\nMessage> %s\nSolution: %s\n", file, line, function, message, solution);
}

static  PFN_BlopErrorCallback BlopErrorCallback = DefaultErrorCallback;
#define _berror(msg, slt)     BlopErrorCallback(__FILE__, __LINE__, __FUNCTION__, msg, slt);

struct _BlopNode_t {
    void* data;
    size_t index;
    struct _BlopNode_t* next;
    struct _BlopNode_t* prev;
    struct _BlopList_t* list;
};

struct _BlopList_t {
    size_t size;
    struct _BlopNode_t* head;
    struct _BlopNode_t* tail;
    struct _BlopNode_t* last;
};

BlopList BlopNewList() {
    return _balloc(struct _BlopList_t, 1);
}
int      BlopFreeList(BlopList list) {
    _bfree(list);

    if (list->size > 0) {
        _berror("Non empty BlopList freed (List was freed but data may remain)", "Empty list before freeing");
        return -1;
    }

    return 0;
}

void     BlopNodeSetData(BlopNode node, void* data) {
    if (node == NULL) {
        _berror("Node was NULL", "Check if Node is a valid ptr");
        return;
    } 

    node->data = data;
}
void     BlopNodeSetNext(BlopNode node, BlopNode next) {
    if (node == NULL) {
        _berror("Node was NULL", "Check if Node is a valid ptr");
        return;
    } 

    node->next = next;
}
void     BlopNodeSetPrev(BlopNode node, BlopNode prev) {
    if (node == NULL) {
        _berror("Node was NULL", "Check if Node is a valid ptr");
        return;
    } 

    node->prev = prev;
}
void     BlopListSetHead(BlopList list, BlopNode head) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return;
    } 

    list->head = head;
}
void     BlopListSetTail(BlopList list, BlopNode tail) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return;
    } 

    list->tail = tail;
}

void*    BlopNodeGetData(BlopNode node) {
    if (node == NULL) {
        _berror("Node was NULL", "Check if Node is a valid ptr");
        return NULL;
    }

    return node->data;
}
BlopNode BlopNodeGetNext(BlopNode node) {
    if (node == NULL) {
        _berror("Node was NULL", "Check if Node is a valid ptr");
        return NULL;
    } 

    return node->next;
}
BlopNode BlopNodeGetPrev(BlopNode node) {
    if (node == NULL) {
        _berror("Node was NULL", "Check if Node is a valid ptr");
        return NULL;
    } 

    return node->prev;
}
BlopNode BlopListGetHead(BlopList list) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return NULL;
    } 

    return list->head;
}
BlopNode BlopListGetTail(BlopList list) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return NULL;
    } 

    return list->tail;
}
BlopNode BlopListGetNode(BlopList list, size_t index) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return NULL;
    }

    if (list->size == 0) {
        _berror("List was empty", "First place any element in the list");
    }

    if (index >= list->size) {
        _berror("Index is bigger than list size", "Pass an index lower than list size");
        return NULL;
    }

    if (index == 0) return list->head;
    if (index == list->size - 1) return list->tail;
    if (list->last->index == index) return list->last;

    size_t dshead = index;
    size_t dstail = (list->size - 1) - index;
    size_t dslast = (list->last->index > index) ? list->last->index - index : index - list->last->index;
    
    bool forward ;
    size_t distance;
    struct _BlopNode_t* current;

    if (dshead <= dslast) {
        forward = true;
        current = list->head;
        distance = dshead;
    } else if (dstail <= dslast) {
        forward = false;    
        current = list->tail;
        distance = dstail;
    } else {
        current = list->last;
        distance = dslast;
        forward = index > list->last->index;
    }

    for (size_t i = 0; i < distance; i++) {
        current = forward ? current->next : current->prev;
    }

    list->last = current;
    return current;
}

int     BlopListErase(BlopList list, BlopNode node) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return -1;
    }

    if (node == NULL) {
        _berror("Node was NULL", "Check if Node is a valid ptr");
        return -1;
    }

    if (list->size == 0) {
        _berror("Trying to erase an element in empty list", "Probably you are not matching right the List and the Node");
        return -1;
    }

    if (node->list != list) {
        _berror("Node does not belong to List", "Probably you are not matching right the List and the Node");
        return -1;
    }

    list->size--;
    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
        list->last = NULL;
    } else if (node == list->head) {
        if (list->last == list->head) list->last = list->head->next;
        list->head = list->head->next;
        list->head->prev = NULL;
    } else if (node == list->tail) {
        if (list->last == list->tail) list->last = list->tail->prev;
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    } else {
        if (list->last == node) list->last = node->prev; 
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    _bfree(node);
    return 0;
}
int     BlopListPopBack(BlopList list) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return -1;
    }

    if (list->size == 0) {
        _berror("Trying to pop back in empty list", "Fill the list with at least one element");
        return -1;
    }

    list->size--;
    struct _BlopNode_t* back = list->tail;
    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
        list->last = NULL;
    } else {
        if (list->last == list->tail) list->last = list->tail->prev;
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }

    _bfree(back);
    return 0;
}
int     BlopListPopFront(BlopList list) {
    if (list == NULL) {
        _berror("List was NULL", "Check if List is a valid ptr");
        return -1;
    }

    if (list->size == 0) {
        _berror("Trying to pop front in empty list", "Fill the list with at least one element");
        return -1;
    }

    list->size--;
    struct _BlopNode_t* front = list->head;
    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
        list->last = NULL;
    } else {
        if (list->last == list->head) list->last = list->head->next;
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    _bfree(front);
    return 0;
}

int     BlopListInsert(BlopList list, BlopNode node, void* data) {
    
}
int     BlopListPushBack(BlopList list, void* data) {

}
int     BlopListPushFront(BlopList list, void* data) {

}