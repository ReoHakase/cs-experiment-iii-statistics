#include <stdio.h>
#include <stdlib.h>

#include "read-data.h"

int main(int ac, char *av[]) {
  FILE *fp;
  Data *dp;
  if ((fp = fopen(av[1], "r")) == NULL) {
    fputs("readData: error 1\n", stderr);
    exit(EXIT_FAILURE);
  }
  dp = readData(fp);
  printf("%d\n", dp->num);
  for (int i = 0; i < 10; i++) {
    printf("[%d] %lf %lf\n", i + 1, dp->data[i].x, dp->data[i].y);
  }
  puts("...");
  for (int i = dp->num - 10; i < dp->num; i++) {
    printf("[%d] %lf %lf\n", i + 1, dp->data[i].x, dp->data[i].y);
  }
}