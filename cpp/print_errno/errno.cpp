#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    printf("invalid argument. eg: ./errno 1\n");
  }

  int e = atoi(argv[1]);
  if (e == 0)
  {
    for (int i = 1; i < sys_nerr; i++)
    {
      printf("errno %d -> %s\n", i, strerror(i));
    }
  }
  else
  {
    printf("errno %d -> %s\n", e, strerror(e));
  }

  return 0;
}
