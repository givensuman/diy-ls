#include "sort.h"
#include "linked_list.h"
#include <string.h>

List *sort(List *list) {
  if (list == NULL || list->head == NULL || list->head->next == NULL) {
    return list; // List is empty or has one element
  }

  int swapped;
  Node *node;
  Node *comparison = NULL;

  do {
    swapped = 0;
    node = list->head;

    while (node->next != comparison) {
      if (strcmp(node->data, node->next->data) > 0) {
        // Swap data
        char *temp = node->data;
        node->data = node->next->data;
        node->next->data = temp;
        swapped = 1;
      }

      node = node->next;
    }

    comparison = node;
  } while (swapped);

  return list;
}
