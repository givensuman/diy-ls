#define _XOPEN_SOURCE 700
#include "linked_list/linked_list.h"
#include "linked_list/sort.h"
#include <dirent.h>
#include <getopt.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  char *name;
  struct stat statbuf;
} FileInfo;

List *read_dir(char *path, int all_files) {
  DIR *dp;
  struct dirent *ep;

  dp = opendir(path);
  List *entries = new_list();

  if (dp != NULL) {
    while ((ep = readdir(dp)) != NULL) {
      if (!all_files && ep->d_name[0] == '.') {
        continue; // Skip hidden files unless -a is specified
      }

      FileInfo *info = malloc(sizeof(FileInfo));
      info->name = strdup(ep->d_name);

      char full_path[PATH_MAX];
      snprintf(full_path, sizeof(full_path), "%s/%s", path, ep->d_name);
      if (stat(full_path, &info->statbuf) != 0) {
        free(info->name);
        free(info);
        continue;
      }

      append(entries, info);
    }

    closedir(dp);
  } else {
    fprintf(stderr, "Could not open directory: %s\n", path);
    exit(1);
  }

  return entries;
}

typedef struct {
  int all_files;
  int long_format;
  int reverse_sort;
  int sort_by_time;
  int sort_by_size;
  int human_readable;
} Options;

void print_permissions(mode_t mode) {
  printf((S_ISDIR(mode)) ? "d" : "-");
  printf((mode & S_IRUSR) ? "r" : "-");
  printf((mode & S_IWUSR) ? "w" : "-");
  printf((mode & S_IXUSR) ? "x" : "-");
  printf((mode & S_IRGRP) ? "r" : "-");
  printf((mode & S_IWGRP) ? "w" : "-");
  printf((mode & S_IXGRP) ? "x" : "-");
  printf((mode & S_IROTH) ? "r" : "-");
  printf((mode & S_IWOTH) ? "w" : "-");
  printf((mode & S_IXOTH) ? "x" : "-");
}

void print_human_readable_size(off_t size) {
  const char *units[] = {"", "K", "M", "G", "T"};
  int unit_index = 0;
  double display_size = (double)size;

  while (display_size >= 1024 && unit_index < 4) {
    display_size /= 1024;
    unit_index++;
  }

  if (unit_index == 0) {
    printf("%4lld ", (long long)size);
  } else {
    printf("%3.1f%s ", display_size, units[unit_index]);
  }
}

void print_long_format(FileInfo *info, int human_readable) {
  print_permissions(info->statbuf.st_mode);
  printf(" %lu ", (unsigned long)info->statbuf.st_nlink);

  struct passwd *pw = getpwuid(info->statbuf.st_uid);
  struct group *gr = getgrgid(info->statbuf.st_gid);
  printf("%s %s ", pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown");

  if (human_readable) {
    print_human_readable_size(info->statbuf.st_size);
  } else {
    printf("%8lld ", (long long)info->statbuf.st_size);
  }

  char time_str[20];
  strftime(time_str, sizeof(time_str), "%b %d %H:%M",
           localtime(&info->statbuf.st_mtime));
  printf("%s ", time_str);

  if (S_ISDIR(info->statbuf.st_mode)) {
    printf("\e[1;34m%-6s/\e[m\n", info->name);
  } else if (info->statbuf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
    printf("\e[1;32m%s\e[m*\n", info->name);
  } else {
    printf("%s\n", info->name);
  }
}

void print_list_long(List *list, int human_readable) {
  Node *node = list->head;
  while (node != NULL) {
    print_long_format((FileInfo *)node->data, human_readable);
    node = node->next;
  }
}

void free_file_list(List *list) {
  Node *node = list->head;
  while (node != NULL) {
    Node *next = node->next;
    if (node->data) {
      FileInfo *info = (FileInfo *)node->data;
      free(info->name);
      free(info);
    }
    free(node);
    node = next;
  }
  free(list);
}

int main(int argc, char **argv) {
  Options opts = {.all_files = 0,
                  .long_format = 0,
                  .reverse_sort = 0,
                  .sort_by_time = 0,
                  .sort_by_size = 0,
                  .human_readable = 0};
  int opt;

  while ((opt = getopt(argc, argv, "alrthS")) != -1) {
    switch (opt) {
    case 'a':
      opts.all_files = 1;
      break;
    case 'l':
      opts.long_format = 1;
      break;
    case 'r':
      opts.reverse_sort = 1;
      break;
    case 't':
      opts.sort_by_time = 1;
      break;
    case 'h':
      opts.human_readable = 1;
      break;
    case 'S':
      opts.sort_by_size = 1;
      break;
    default:
      fprintf(stderr, "Usage: %s [-alrthS] [path]\n", argv[0]);
      exit(1);
    }
  }

  char *path = (optind < argc) ? argv[optind] : ".";

  List *entries = read_dir(path, opts.all_files);
  entries =
      sort(entries, opts.reverse_sort, opts.sort_by_time, opts.sort_by_size);
  if (opts.long_format) {
    print_list_long(entries, opts.human_readable);
  } else {
    Node *node = entries->head;
    while (node != NULL) {
      FileInfo *info = (FileInfo *)node->data;
      if (S_ISDIR(info->statbuf.st_mode)) {
        printf("\e[1;34m%s/\e[m\n", info->name);
      } else if (info->statbuf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
        printf("\e[1;32m%s\e[m*\n", info->name);
      } else {
        printf("%s\n", info->name);
      }
      node = node->next;
    }
  }
  free_file_list(entries);
}
