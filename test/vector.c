#define BLOP_VECTOR_NAME TestVector
#define BLOP_VECTOR_DATA_TYPE int
#define BLOP_VECTOR_IMPLEMENTATION
#define BLOP_VECTOR_EXIT_ON_ERROR
#include "../vector.h"

#define BLOP_VECTOR_NAME TestVector2
#define BLOP_VECTOR_DATA_TYPE const char*
#define BLOP_VECTOR_IMPLEMENTATION
#define BLOP_VECTOR_EXIT_ON_ERROR
#include "../vector.h"

void vector1_push(TestVector vector, int data) {
    TestVector_push_back(vector, data);
}
void vector2_push(TestVector2 vector, const char* data) {
    TestVector2_push_back(vector, data);
}

void vector1_print(TestVector vector) {
    for (size_t i = 0; i < TestVector_get_count(vector); i++) {
        printf("Node %zu Data: %d\n", i, TestVector_get(vector, i));
    }
}
void vector2_print(TestVector2 vector) {
    for (size_t i = 0; i < TestVector2_get_count(vector); i++) {
        printf("Node %zu Data: %s\n", i, TestVector2_get(vector, i));
    }
}

int main() {
    TestVector vector = TestVector_create();
    TestVector2 vector2 = TestVector2_create();

    const char* tests[10] = {"Test", "Vector", "Hello", "World", "This", "Is", "A", "Simple", "Dynamic", "Array"};
    for (int i = 0; i < 10; i++) {
        vector1_push(vector, i * 10);
    }

    TestVector2_resize(vector2, 10);
    TestVector2_memcpy(vector2, 0, tests, 10);

    vector1_print(vector);
    vector2_print(vector2);

    TestVector_erase(vector, 5);
    TestVector2_erase(vector2, 5);

    vector1_print(vector);
    vector2_print(vector2);

    TestVector_clear(vector);
    TestVector2_clear(vector2);

    TestVector_destroy(vector);
    TestVector2_destroy(vector2);

    printf("Success!\n");
    return 0;
}