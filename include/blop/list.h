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

BlopList  BlopNewList       ();
int       BlopFreeList      (BlopList list);

BlopNode  BlopNewNode       ();
BlopNode  BlopCopyNode      (BlopNode node);
int       BlopFreeNode      (BlopNode node);

// The stack parameter lets you store a value by copying it instead of having a ptr
int       BlopNodeSetStack  (BlopNode node, long long stack);
int       BlopNodeSetHeap   (BlopNode node, void* heap);

// The stack parameter lets you store a value by copying it instead of having a ptr
long long BlopNodeGetStack  (BlopNode node);
void*     BlopNodeGetHeap   (BlopNode node);
BlopNode  BlopNodeGetNext   (BlopNode node);
BlopNode  BlopNodeGetPrev   (BlopNode node);
BlopNode  BlopListGetFront  (BlopList list);
BlopNode  BlopListGetBack   (BlopList list);
size_t    BlopListGetSize   (BlopList list);
BlopNode  BlopListGetNode   (BlopList list, size_t index);

int       BlopListClear     (BlopList list, int deallocate);
int       BlopListErase     (BlopList list, BlopNode node, int deallocate);
int       BlopListPopBack   (BlopList list, int deallocate);
int       BlopListPopFront  (BlopList list, int deallocate);

int       BlopListPushBack  (BlopList list, BlopNode node);
int       BlopListPushFront (BlopList list, BlopNode node);
int       BlopListInsertNext(BlopList list, BlopNode pivot, BlopNode node);
int       BlopListInsertPrev(BlopList list, BlopNode pivot, BlopNode node);

#endif // __BLOP_LIST_H__