//#include <unistd.h>
char* str = "Hello World\n";

void print()
{
  asm( "movl $12,%%edx \n\t"
       "movl %0,%%ecx  \n\t"
       "movl $0,%%ebx  \n\t"
       "movl $4,%%eax  \n\t"
       "int $0x80      \n\t"
       ::"r"(str):"edx","ecx","ebx");
}

// 另一种方式
// void print()
// {
//  asm( "int $0x80      \n\t"
//       ::"a"(4),"b"(0),"c"(str),"d"(12):"memory");
// }


void main()
{
  //write(0, str, 12);
  print();
}

