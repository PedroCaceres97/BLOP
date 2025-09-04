#ifndef __BLOP_LIST_H__
#define __BLOP_LIST_H__

#include "../Blop.h"

typedef struct _BlopNode_t* BlopNode;
typedef struct _BlopList_t* BlopList;

BlopList BlopNewList();
int      BlopFreeList(BlopList list);

BlopNode BlopNewNode(BlopList list);

void     BlopNodeSetData(BlopNode node, void* data);
void     BlopNodeSetNext(BlopNode node, BlopNode next);
void     BlopNodeSetPrev(BlopNode node, BlopNode prev);
void     BlopListSetHead(BlopList list, BlopNode head);
void     BlopListSetTail(BlopList list, BlopNode tail);

void*    BlopNodeGetData(BlopNode node);
BlopNode BlopNodeGetNext(BlopNode node);
BlopNode BlopNodeGetPrev(BlopNode node);
BlopNode BlopListGetHead(BlopList list);
BlopNode BlopListGetTail(BlopList list);
BlopNode BlopListGetNode(BlopList list, size_t index);

int      BlopListErase(BlopList list, BlopNode node);
int      BlopListPopBack(BlopList list);
int      BlopListPopFront(BlopList list);

int      BlopListPushBack(BlopList list, BlopNode node);
int      BlopListPushFront(BlopList list, BlopNode node);
int      BlopListInsertNext(BlopList list, BlopNode pivot, BlopNode node);
int      BlopListInsertPrev(BlopList list, BlopNode pivot, BlopNode node);

void     BlopPoolErrorCallback(PFN_BlopErrorCallback callback);

#endif // __BLOP_LIST_H__