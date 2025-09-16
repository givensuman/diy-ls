#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

/**
 * Integer node type for linked list
 *
 * @param int data
 * @param Node *next
 */
typedef struct node {
  char *data;
  struct node *next;
} Node;

/**
 * Linked list data structure
 *
 * @param Node *head
 * @param Node *tail
 */
typedef struct list {
  Node *head;
  Node *tail;
} List;

/**
 * Creates a new linked list
 *
 * @return List *list
 */
List *new_list();

/**
 * Prints the linked list
 *
 * @param List *list
 */
void print_list(List *list);

/**
 * Appends data to the end of the linked list
 *
 * @param List *list
 */
List *append(List *list, char *data);

/**
 * Prepends data to the beginning of the linked list
 *
 * @param List *list
 */
List *prepend(List *list, char *data);

/**
 * Frees the linked list from memory
 *
 * @param Node *head
 */
void free_list(List *list);

/**
 * Frees the linked list from memory
 * and the list itself
 */
void destroy_list(List *list);

#endif // LINKED_LIST_H_
