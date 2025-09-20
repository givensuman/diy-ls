#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

List *new_list() {
  List *list = malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;

  return list;
}

void print_list(List *list) {
  if (list == NULL || list->head == NULL) {
    printf("List is empty.\n");
    return;
  }

  Node *node = list->head;
  while (node != NULL) {
    printf("%p%s", node->data, node->next == NULL ? "" : "\n");
    node = node->next;
  }
  printf("\n");
}

List *append(List *list, void *data) {
  Node *node = malloc(sizeof(Node));
  node->data = data;
  node->next = NULL;

  if (list == NULL) {
    list = malloc(sizeof(List));
    list->head = node;
    list->tail = node;
    return list;
  }

  if (list->tail == NULL) {
    list->head = node;
    list->tail = node;
    return list;
  }

  list->tail->next = node;
  list->tail = node;

  return list;
}

List *prepend(List *list, void *data) {
  Node *node = malloc(sizeof(Node));
  node->data = data;

  if (list == NULL || list->head == NULL) {
    list->head = node;
    list->tail = node;
    node->next = NULL;

    return list;
  }

  node->next = list->head;
  list->head = node;

  return list;
}

void free_list(List *list) {
  Node *node = list->head;
  while (node != NULL) {
    Node *next = node->next;
    free(node);
    node = next;
  }
}

void destroy_list(List *list) {
  free_list(list);
  free(list);
}
