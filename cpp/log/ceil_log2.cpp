#include <stdio.h>
#include <stdlib.h>


/* Compute Ceil(Log2(v)) */
/* Derived from branchless code for integer log2(v) from:
   <http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog> */
unsigned int ceil_log2 (unsigned int v)
{
  unsigned int r, shift;

  v--;
  r = (v > 0xFFFF) << 4;
  v >>= r;
  shift = (v > 0xFF) << 3;
  v >>= shift;
  r |= shift;
  shift = (v > 0xF) << 2;
  v >>= shift;
  r |= shift;
  shift = (v > 0x3) << 1;
  v >>= shift;
  r |= shift;
  r |= (v >> 1);
  return r + 1;
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("invalid argument, eg: log 16\n");
		return 0;
	}
	unsigned int v = (unsigned int)atoi(argv[1]);

	/*Find the log base 2 of argv[1]*/
	printf("ceil log2 %s is %u\n", argv[1], ceil_log2(v));
	return 0;
}
