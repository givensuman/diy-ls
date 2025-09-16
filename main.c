#define _XOPEN_SOURCE 700
#include "linked_list/linked_list.h"
#include "linked_list/sort.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// https://stackoverflow.com/a/12506/14561388
List *read_dir(char *path) {
  DIR *dp;
  struct dirent *ep;

  dp = opendir(path);
  List *entries = new_list();

  if (dp != NULL) {
    while ((ep = readdir(dp)) != NULL) {
      append(entries, strdup(ep->d_name));
    }

    closedir(dp);
  } else {
    fprintf(stderr, "Could not open directory: %s\n", path);
    exit(1);
  }

  return entries;
}

int main(int argc, char **argv) {
  if (argc > 2) {
    fprintf(stderr, "Usage: %s [path]\n", argv[0]);
    exit(1);
  }

  char *path;
  if (argc == 1) {
    path = ".";
  } else {
    path = argv[1];
  }

  List *entries = read_dir(path);
  entries = sort(entries);
  print_list(entries);
  free_list(entries);
}
