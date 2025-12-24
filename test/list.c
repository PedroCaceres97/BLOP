#define BLOP_LOG_SHORT
#define BLOP_LOG_COLOURED

#define BLOP_ANSI_SHORT

#define BLOP_ASSERT_SHORT

#define BLOP_LIST_NAME      TList
#define BLOP_LIST_TYPE_DATA char
#define BLOP_LIST_IMPLEMENTATION
#include <blop/list.h>

void push(TList list, char c) {
  TList_node node = TList_node_create(NULL);
  node->data = c;
  TList_push_back(list, node);
}

int main() {
  ANSI_ENABLE();

  TList list = TList_create(NULL);
  LOG_SUCCESS("List created");

  push(list, 'h');
  push(list, 'o');
  push(list, 'l');
  push(list, 'a');
  push(list, ' ');
  push(list, 'M');
  push(list, 'u');
  push(list, 'n');
  push(list, 'd');
  push(list, 'o');
  push(list, '\n');
  push(list, 0);
  LOG_SUCCESS("Pushed to list");

  TList_node current = list->front;
  while (current->data != 0) {
    putchar(current->data);
    current = current->next;
  }

  LOG_SUCCESS("List printed");

  ANSI_DISABLE();
  return 0;
}