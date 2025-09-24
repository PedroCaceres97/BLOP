#ifndef __BLOP_LIST_H__
#define __BLOP_LIST_H__

typedef struct _BNODE_t* BNODE;
typedef struct _BLIST_t* BLIST;

#ifdef __BLOP_SHOW_LIST_IMPLEMENTATION__

struct _BNODE_t {
    void*               heap;
    long long           stack;  // To copy values instead of having a ptr
    size_t              index;
    struct _BNODE_t*    next;
    struct _BNODE_t*    prev;
    struct _BLIST_t*    list;
};

struct _BLIST_t {
    size_t              size;
    struct _BNODE_t* front;
    struct _BNODE_t* back;
};

#endif // __BLOP_SHOW_LIST_IMPLEMENTATION__

BLIST       BLIST_Create    ();
void        BLIST_Destroy   (BLIST list);

BNODE       BNODE_Create    ();
BNODE       BNODE_Duplicate (BNODE node);
void        BNODE_Destroy   (BNODE node);

// The stack parameter lets you store a value by copying it instead of having a ptr
void        BNODE_SetStack  (BNODE node, long long stack);
void        BNODE_SetHeap   (BNODE node, void* heap);

// The stack parameter let  s you store a value by copying it instead of having a ptr
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

#endif // __BLOP_LIST_H__