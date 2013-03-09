#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
  struct radio {
    int min_freq;
    int max_freq;
  };

  struct car_part {
    char * name;
    char * maker;
    union {
      int capacity;
      char * style;
      struct radio rpt;
    };
  };

  typedef struct car_part CarPart;

  CarPart cp;

  cp.name = strdup("Hello there!");
  printf("name: %s\n", cp.name);
  cp.capacity = 1;
  printf("capacity: %d\n", cp.capacity);
  cp.rpt.min_freq = 10;
  printf("min_freq: %d\n", cp.rpt.min_freq);

  free(cp.name);

  return 0;
}
