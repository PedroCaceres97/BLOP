#ifndef __BLOP_LIST_H__
#define __BLOP_LIST_H__

#ifndef BLOP_CONCAT
    #define BLOP_CONCAT(a, b) a##b
#endif

#ifndef BLOP_XCONCAT
    #define BLOP_XCONCAT(a, b) BLOP_CONCAT(a, b)
#endif

#define BLOP_DECLARE_LIST_SNAKE(name, type)                                                                                             \
                                                                                                                                        \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _t))* name;                                                                       \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _node_t))* BLOP_XCONCAT(name, _node);                                             \
                                                                                                                                        \
    name BLOP_XCONCAT(name, _create)();                                                                                                 \
    void BLOP_XCONCAT(name, _destroy)(name list);                                                                                       \
                                                                                                                                        \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _get_front)(name list);                                                                \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _get_back)(name list);                                                                 \
    size_t BLOP_XCONCAT(name, _get_size)(name list);                                                                                    \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _get_node)(name list, size_t index);                                                   \
                                                                                                                                        \
    void BLOP_XCONCAT(name, _clear)(name list, int deallocate);                                                                         \
    void BLOP_XCONCAT(name, _erase)(name list, BLOP_XCONCAT(name, _node) node, int deallocate);                                         \
    void BLOP_XCONCAT(name, _pop_back)(name list, int deallocate);                                                                      \
    void BLOP_XCONCAT(name, _pop_front)(name list, int deallocate);                                                                     \
                                                                                                                                        \
    void BLOP_XCONCAT(name, _push_back)(name list, BLOP_XCONCAT(name, _node) node);                                                     \
    void BLOP_XCONCAT(name, _push_front)(name list, BLOP_XCONCAT(name, _node) node);                                                    \
    void BLOP_XCONCAT(name, _insert_next)(name list, BLOP_XCONCAT(name, _node) pivot, BLOP_XCONCAT(name, _node) node);                  \
    void BLOP_XCONCAT(name, _insert_prev)(name list, BLOP_XCONCAT(name, _node) pivot, BLOP_XCONCAT(name, _node) node);                  \
                                                                                                                                        \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_create)();                                                                       \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_duplicate)(BLOP_XCONCAT(name, _node) node);                                      \
    void BLOP_XCONCAT(name, _node_destroy)(BLOP_XCONCAT(name, _node) node);                                                             \
    void BLOP_XCONCAT(name, _node_set_data)(BLOP_XCONCAT(name, _node) node, type data);                                                 \
    type BLOP_XCONCAT(name, _node_get_data)(BLOP_XCONCAT(name, _node) node);                                                            \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_get_next)(BLOP_XCONCAT(name, _node) node);                                       \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_get_prev)(BLOP_XCONCAT(name, _node) node);

#define BLOP_DECLARE_LIST_CAMEL(name, type)                                                                                             \
                                                                                                                                        \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _t))* name;                                                                       \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, Node_t))* BLOP_XCONCAT(name, Node);                                               \
                                                                                                                                        \
    name BLOP_XCONCAT(name, _create)();                                                                                                 \
    void BLOP_XCONCAT(name, _destroy)(name list);                                                                                       \
                                                                                                                                        \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, GetFront)(name list);                                                                   \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, GetBack)(name list);                                                                    \
    size_t BLOP_XCONCAT(name, GetSize)(name list);                                                                                      \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, GetNode)(name list, size_t index);                                                      \
                                                                                                                                        \
    void BLOP_XCONCAT(name, Clear)(name list, int deallocate);                                                                          \
    void BLOP_XCONCAT(name, Erase)(name list, BLOP_XCONCAT(name, Node) node, int deallocate);                                           \
    void BLOP_XCONCAT(name, PopBack)(name list, int deallocate);                                                                        \
    void BLOP_XCONCAT(name, PopFront)(name list, int deallocate);                                                                       \
                                                                                                                                        \
    void BLOP_XCONCAT(name, PushBack)(name list, BLOP_XCONCAT(name, Node) node);                                                        \
    void BLOP_XCONCAT(name, PushFront)(name list, BLOP_XCONCAT(name, Node) node);                                                       \
    void BLOP_XCONCAT(name, InsertBext)(name list, BLOP_XCONCAT(name, Node) pivot, BLOP_XCONCAT(name, Node) node);                      \
    void BLOP_XCONCAT(name, InsertPrev)(name list, BLOP_XCONCAT(name, Node) pivot, BLOP_XCONCAT(name, Node) node);                      \
                                                                                                                                        \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeCreate)();                                                                          \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeDuplicate)(BLOP_XCONCAT(name, Node) node);                                          \
    void BLOP_XCONCAT(name, NodeDestroy)(BLOP_XCONCAT(name, Node) node);                                                                \
    void BLOP_XCONCAT(name, NodeSetData)(BLOP_XCONCAT(name, Node) node, type data);                                                     \
    type BLOP_XCONCAT(name, NodeGetData)(BLOP_XCONCAT(name, Node) node);                                                                \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeGetNext)(BLOP_XCONCAT(name, Node) node);                                            \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeGetPrev)(BLOP_XCONCAT(name, Node) node);

#define BLOP_DEFINE_LIST_STRUCT_SNAKE(name, type)           \
                                                            \
    struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _node_t)) {   \
        type data;                                          \
        BLOP_XCONCAT(name, _node_t) next;                   \
        BLOP_XCONCAT(name, _node_t) prev;                   \
        name list;                                          \
    };                                                      \
                                                            \
    struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _t)) {        \
        size_t size;                                        \
        BLOP_XCONCAT(name, _node) front;                    \
        BLOP_XCONCAT(name, _node) back;                     \
    };

#define BLOP_DEFINE_LIST_STRUCT_CAMEL(name, type)           \
                                                            \
    struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, Node_t)) {    \
        type data;                                          \
        BLOP_XCONCAT(name, Node_t) next;                    \
        BLOP_XCONCAT(name, Node_t) prev;                    \
        name list;                                          \
    };                                                      \
                                                            \
    struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _t)) {        \
        size_t size;                                        \
        BLOP_XCONCAT(name, Node) front;                     \
        BLOP_XCONCAT(name, Node) back;                      \
    };

#define BLOP_DEFINE_LIST_SNAKE(name, type)                                                                                              \
                                                                                                                                        \
    BLOP_DEFINE_LIST_STRUCT_SNAKE(name, type)                                                                                           \
                                                                                                                                        \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _t))* name;                                                                       \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _node_t))* BLOP_XCONCAT(name, _node);                                             \
                                                                                                                                        \
    name BLOP_XCONCAT(name, _create)() {                                                                                                \
        name list = (name)malloc(sizeof(struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _t))));                                               \
        if (!list) {                                                                                                                    \
            return NULL;                                                                                                                \
        }                                                                                                                               \
        list->size = 0;                                                                                                                 \
        list->front = NULL;                                                                                                             \
        list->back = NULL;                                                                                                              \
        return list;                                                                                                                    \
    }                                                                                                                                   \
                                                                                                                                        \
    void BLOP_XCONCAT(name, _destroy)(name list);                                                                                       \
                                                                                                                                        \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _get_front)(name list);                                                                \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _get_back)(name list);                                                                 \
    size_t BLOP_XCONCAT(name, _get_size)(name list);                                                                                    \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _get_node)(name list, size_t index);                                                   \
                                                                                                                                        \
    void BLOP_XCONCAT(name, _clear)(name list, int deallocate);                                                                         \
    void BLOP_XCONCAT(name, _erase)(name list, BLOP_XCONCAT(name, _node) node, int deallocate);                                         \
    void BLOP_XCONCAT(name, _pop_back)(name list, int deallocate);                                                                      \
    void BLOP_XCONCAT(name, _pop_front)(name list, int deallocate);                                                                     \
                                                                                                                                        \
    void BLOP_XCONCAT(name, _push_back)(name list, BLOP_XCONCAT(name, _node) node);                                                     \
    void BLOP_XCONCAT(name, _push_front)(name list, BLOP_XCONCAT(name, _node) node);                                                    \
    void BLOP_XCONCAT(name, _insert_next)(name list, BLOP_XCONCAT(name, _node) pivot, BLOP_XCONCAT(name, _node) node);                  \
    void BLOP_XCONCAT(name, _insert_prev)(name list, BLOP_XCONCAT(name, _node) pivot, BLOP_XCONCAT(name, _node) node);                  \
                                                                                                                                        \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_create)();                                                                       \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_duplicate)(BLOP_XCONCAT(name, _node) node);                                      \
    void BLOP_XCONCAT(name, _node_destroy)(BLOP_XCONCAT(name, _node) node);                                                             \
    void BLOP_XCONCAT(name, _node_set_data)(BLOP_XCONCAT(name, _node) node, type data);                                                 \
    type BLOP_XCONCAT(name, _node_get_data)(BLOP_XCONCAT(name, _node) node);                                                            \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_get_next)(BLOP_XCONCAT(name, _node) node);                                       \
    BLOP_XCONCAT(name, _node) BLOP_XCONCAT(name, _node_get_prev)(BLOP_XCONCAT(name, _node) node);

#define BLOP_DEFINE_LIST_CAMEL(name, type)                                                                                              \
                                                                                                                                        \
    BLOP_DEFINE_LIST_STRUCT_CAMEL(name, type)                                                                                           \
                                                                                                                                        \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, _t))* name;                                                                       \
    typedef struct BLOP_XCONCAT(_, BLOP_XCONCAT(name, Node_t))* BLOP_XCONCAT(name, Node);                                               \
                                                                                                                                        \
    name BLOP_XCONCAT(name, _create)();                                                                                                 \
    void BLOP_XCONCAT(name, _destroy)(name list);                                                                                       \
                                                                                                                                        \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, GetFront)(name list);                                                                   \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, GetBack)(name list);                                                                    \
    size_t BLOP_XCONCAT(name, GetSize)(name list);                                                                                      \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, GetNode)(name list, size_t index);                                                      \
                                                                                                                                        \
    void BLOP_XCONCAT(name, Clear)(name list, int deallocate);                                                                          \
    void BLOP_XCONCAT(name, Erase)(name list, BLOP_XCONCAT(name, Node) node, int deallocate);                                           \
    void BLOP_XCONCAT(name, PopBack)(name list, int deallocate);                                                                        \
    void BLOP_XCONCAT(name, PopFront)(name list, int deallocate);                                                                       \
                                                                                                                                        \
    void BLOP_XCONCAT(name, PushBack)(name list, BLOP_XCONCAT(name, Node) node);                                                        \
    void BLOP_XCONCAT(name, PushFront)(name list, BLOP_XCONCAT(name, Node) node);                                                       \
    void BLOP_XCONCAT(name, InsertBext)(name list, BLOP_XCONCAT(name, Node) pivot, BLOP_XCONCAT(name, Node) node);                      \
    void BLOP_XCONCAT(name, InsertPrev)(name list, BLOP_XCONCAT(name, Node) pivot, BLOP_XCONCAT(name, Node) node);                      \
                                                                                                                                        \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeCreate)();                                                                          \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeDuplicate)(BLOP_XCONCAT(name, Node) node);                                          \
    void BLOP_XCONCAT(name, NodeDestroy)(BLOP_XCONCAT(name, Node) node);                                                                \
    void BLOP_XCONCAT(name, NodeSetData)(BLOP_XCONCAT(name, Node) node, type data);                                                     \
    type BLOP_XCONCAT(name, NodeGetData)(BLOP_XCONCAT(name, Node) node);                                                                \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeGetNext)(BLOP_XCONCAT(name, Node) node);                                            \
    BLOP_XCONCAT(name, Node) BLOP_XCONCAT(name, NodeGetPrev)(BLOP_XCONCAT(name, Node) node);

#endif // __BLOP_LIST_H__