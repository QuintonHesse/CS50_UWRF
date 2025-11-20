#include <stdio.h>
#include <stdlib.h>

char* func()
{
  // Allocation of 19 bytes
  char *p = (char *) malloc(19);
  free(p);
  return p;
}


int main()
{
  char *p;

  // Allocation of 19 bytes
  p = (char *) malloc(19);
  free(p);

  // Allocation of 12 bytes
  p = (char *) malloc(12);
  free(p);

  // Allocation of 16 bytes
  p = (char *) malloc(16);
  free(p);

  func();

  return 0;
}
