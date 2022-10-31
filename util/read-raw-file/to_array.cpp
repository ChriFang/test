#include <stdio.h>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    printf("invalid arg\n");
    return -1;
  }

  char* filename = argv[1];
  FILE* fp = fopen(filename, "rb+");
  if (fp == NULL)
  {
    printf("open file failed, filename %s\n", filename);
    return -1;
  }
  
  char buffer[2] = { 0 };
  size_t readlen;
  int count = 0;
  while (fread(buffer, 1, 1, fp) > 0)
  {
    unsigned char val = (unsigned char)buffer[0];
    printf("0x%02x, ", val);
    if (count++ >= 16) 
    {
        printf("\n");
        count = 0;
    }
  }
  fclose(fp);
  return 0;
}