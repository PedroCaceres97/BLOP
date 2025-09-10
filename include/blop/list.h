#ifndef __BLOP_LIST_H__
#define __BLOP_LIST_H__

#include <blop/blop.h>

typedef struct _BlopNode_t* BlopNode;
typedef struct _BlopList_t* BlopList;

#ifdef __BLOP_SHOW_LIST_IMPLEMENTATION__

struct _BlopNode_t {
    void*               heap;
    long long           stack;  // To copy values instead of having a ptr
    size_t              index;
    struct _BlopNode_t* next;
    struct _BlopNode_t* prev;
    struct _BlopList_t* list;
};

struct _BlopList_t {
    size_t              size;
    struct _BlopNode_t* front;
    struct _BlopNode_t* back;
};

#endif // __BLOP_SHOW_LIST_IMPLEMENTATION__

BlopResult BlopNewList       (BlopList* buffer);
BlopResult BlopFreeList      (BlopList list);

BlopResult BlopNewNode       (BlopNode* buffer);
BlopResult BlopCopyNode      (BlopNode node, BlopNode* buffer);
BlopResult BlopFreeNode      (BlopNode node);

// The stack parameter lets you store a value by copying it instead of having a ptr
BlopResult BlopNodeSetStack  (BlopNode node, long long stack);
BlopResult BlopNodeSetHeap   (BlopNode node, void* heap);

// The stack parameter lets you store a value by copying it instead of having a ptr
BlopResult BlopNodeGetStack  (BlopNode node, long long* buffer);
BlopResult BlopNodeGetHeap   (BlopNode node, void** buffer);
BlopResult BlopNodeGetNext   (BlopNode node, BlopNode* buffer);
BlopResult BlopNodeGetPrev   (BlopNode node, BlopNode* buffer);
BlopResult BlopListGetFront  (BlopList list, BlopNode* buffer);
BlopResult BlopListGetBack   (BlopList list, BlopNode* buffer);
BlopResult BlopListGetSize   (BlopList list, size_t* buffer);
BlopResult BlopListGetNode   (BlopList list, BlopNode* buffer, size_t index);

BlopResult BlopListClear     (BlopList list, int deallocate);
BlopResult BlopListErase     (BlopList list, BlopNode node, int deallocate);
BlopResult BlopListPopBack   (BlopList list, int deallocate);
BlopResult BlopListPopFront  (BlopList list, int deallocate);

BlopResult BlopListPushBack  (BlopList list, BlopNode node);
BlopResult BlopListPushFront (BlopList list, BlopNode node);
BlopResult BlopListInsertNext(BlopList list, BlopNode pivot, BlopNode node);
BlopResult BlopListInsertPrev(BlopList list, BlopNode pivot, BlopNode node);

#endif // __BLOP_LIST_H__