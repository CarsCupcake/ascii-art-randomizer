#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

typedef struct node {
  struct node *next;
  void *data;
} Node;

int main(const int argc, char *argv[])
{
  if (argc == 1) {
    printf("Usage: ascii-art-randomizer <dir>\n");
    return 1;
  }
  DIR *dir;
  if (NULL == (dir = opendir(argv[1]))) {
    printf("Error opening directory %s\n", argv[1]);
    return 1;
  }
  int count = 0;
  Node *list = nullptr;
  struct dirent *entry;
  while ((entry = readdir(dir))) {
    const auto name = entry->d_name;
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
      continue;
    }
    count++;
    // ReSharper disable once CppDFAMemoryLeak
    const auto new_node = (Node *)malloc(sizeof(Node));
    new_node->data = name;
    if (list == NULL) {
      list = new_node;
    } else {
      new_node->next = list;
      list = new_node;
    }
  }
  if (list == NULL) {
    printf("Empty list\n");
    // ReSharper disable once CppDFAMemoryLeak
    return 1;
  }
  srand(time(nullptr)); // NOLINT(*-msc51-cpp)
  const int r = rand() % count; // NOLINT(*-msc50-cpp)
  for (int i = 0; i < r; i++) {
    Node *l = list;
    list = list->next;
    free(l);
  }
  const char* filename = list->data;
  const char *dirname = argv[1];
  const bool hasNoSlash = dirname[strlen(dirname) - 1] != '/';
  char *totalName = malloc(strlen(dirname) + hasNoSlash + strlen(filename));
  strcpy(totalName, dirname);
  if (hasNoSlash) {
    totalName[strlen(dirname) - 1] = '/';
  }
  strcat(totalName, filename);
  FILE *fptr = fopen(totalName, "r");
  while (list != NULL) {
    Node *temp = list;
    list = list->next;
    free(temp);
  }
  closedir(dir);
  // Check if the file was opened successfully
  if (fptr == NULL) {
    printf("Error opening file %s\n", totalName);
    free(totalName);
    return 1;
  }

  // Read and print each character from the file
  char ch;
  while ((ch = fgetc(fptr)) != EOF) {
    putchar(ch);
  }
  fclose(fptr);
  free(totalName);
  return 0;
}
