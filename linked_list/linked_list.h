#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

/**
 * Generic node type for linked list
 *
 * @param void *data
 * @param Node *next
 */
typedef struct node {
  void *data;
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
 * @param void *data
 */
List *append(List *list, void *data);

/**
 * Prepends data to the beginning of the linked list
 *
 * @param List *list
 * @param void *data
 */
List *prepend(List *list, void *data);

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
