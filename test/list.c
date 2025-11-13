#define BLOP_SAFE_MODE
#define BLOP_EXIT_ON_ERROR

#define BLOP_LIST_NAME              TestList
#define BLOP_LIST_DATA_TYPE         int
#define BLOP_LIST_IMPLEMENTATION
#include <blop/list.h>

#define BLOP_LIST_NAME              TestList2
#define BLOP_LIST_DATA_TYPE const   char*
#define BLOP_LIST_IMPLEMENTATION
#include <blop/list.h>

void list1_push(TestList list, int data) {
    TestList_push_back(list, TestList_node_set_data(TestList_node_create(), data));
}
void list2_push(TestList2 list, const char* data) {
    TestList2_push_back(list, TestList2_node_set_data(TestList2_node_create(), data));
}

void list1_print(TestList list) {
    for (size_t i = 0; i < TestList_get_size(list); i++) {
        printf("Node %zu Data: %d\n", i, TestList_node_get_data(TestList_get_node(list, i)));
    }
}

void list2_print(TestList2 list) {
    for (size_t i = 0; i < TestList2_get_size(list); i++) {
        printf("Node %zu Data: %s\n", i, TestList2_node_get_data(TestList2_get_node(list, i)));
    }
}


int main() {

    TestList list = TestList_create();
    TestList2 list2 = TestList2_create();

    const char* tests[10] = {"Test", "List", "Hello", "World", "This", "Is", "A", "Simple", "Doubly", "Linked List"};
    for (int i = 0; i < 10; i++) {
        list1_push(list, i * 10);
        list2_push(list2, tests[i]);
    }

    list1_print(list);
    list2_print(list2);

    TestList_node node = TestList_get_node(list, 5);
    TestList2_node node2 = TestList2_get_node(list2, 5);

    TestList_erase(list, node, true);
    TestList2_erase(list2, node2, true);

    list1_print(list);
    list2_print(list2);

    TestList_get_node(list, 0);

    TestList_clear(list, true);
    TestList2_clear(list2, true);

    TestList_destroy(list);
    TestList2_destroy(list2);

    printf("Success!\n");
    return 0;
}