#include "sort.h"
#include "linked_list.h"
#include <string.h>
#include <sys/stat.h>

typedef struct {
  char *name;
  struct stat statbuf;
} FileInfo;

int compare_files(FileInfo *a, FileInfo *b, int by_time, int by_size) {
  // Sort directories first
  int a_is_dir = S_ISDIR(a->statbuf.st_mode);
  int b_is_dir = S_ISDIR(b->statbuf.st_mode);
  if (a_is_dir != b_is_dir) {
    return b_is_dir - a_is_dir; // directories first
  }

  if (by_size) {
    if (a->statbuf.st_size != b->statbuf.st_size) {
      return a->statbuf.st_size - b->statbuf.st_size;
    }
  } else if (by_time) {
    if (a->statbuf.st_mtime != b->statbuf.st_mtime) {
      return a->statbuf.st_mtime - b->statbuf.st_mtime;
    }
  }

  // Default: alphabetical by name
  return strcmp(a->name, b->name);
}

List *sort(List *list, int reverse, int by_time, int by_size) {
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
      FileInfo *a = (FileInfo *)node->data;
      FileInfo *b = (FileInfo *)node->next->data;

      int cmp = compare_files(a, b, by_time, by_size);
      if (reverse) {
        cmp = -cmp;
      }

      if (cmp > 0) {
        // Swap data
        void *temp = node->data;
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
