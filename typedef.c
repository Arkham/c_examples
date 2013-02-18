#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  enum colors { RED, YELLOW, GREEN };
  typedef enum colors state;

  state semaphore;
  semaphore = GREEN;

  printf("The color of the semaphore is %d\n", semaphore);

  return 0;
}
