#define BLOP_LIST_NAME  TestList
#define BLOP_LIST_DATA_TYPE int
#define BLOP_LIST_IMPLEMENTATION
#include "../list.h"

#define BLOP_LIST_NAME  TestList2
#define BLOP_LIST_DATA_TYPE const char*
#define BLOP_LIST_IMPLEMENTATION
// #define BLOP_LIST_EXIT_ON_ERROR
#include "../list.h"

#include <stdbool.h>

void list1_push(TestList list, int data) {
    TestList_push_back(list, TestList_node_set_data(TestList_node_create(), data));
}

void list2_push(TestList2 list, const char* data) {
    TestList2_push_back(list, TestList2_node_set_data(TestList2_node_create(), data));
}

void ups() {
    printf("Ups\n");
}

int main() {
    atexit(ups);

    TestList list = TestList_create();
    TestList2 list2 = TestList2_create();

    const char* tests[10] = {"Test", "List", "Hello", "World", "This", "Is", "A", "Simple", "Doubly", "Linked List"};
    for (int i = 0; i < 10; i++) {
        list1_push(list, i * 10);
        list2_push(list2, tests[i]);
    }

    for (int i = 0; i < 10; i++) {
        printf("List1 Node %d Data: %d\n", i, TestList_node_get_data(TestList_get_node(list, i)));
    }
    for (int i = 0; i < 10; i++) {
        printf("List2 Node %d Data: %s\n", i, TestList2_node_get_data(TestList2_get_node(list2, i)));
    }

    TestList_destroy(list);
    TestList2_destroy(list2);

    TestList_clear(list, true);
    TestList2_clear(list2, true);

    TestList_destroy(list);
    TestList2_destroy(list2);

    printf("Success!\n");
    return 0;
}