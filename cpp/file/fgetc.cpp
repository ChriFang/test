#include <stdio.h>

// fgetc返回int类型而不是char类型，目的是为了兼容返回EOF值

int main()
{
	FILE* fp = fopen("1.txt", "r");
	if (fp == NULL)
	{
		printf("open file failed\n");
		return -1;
	}

	char txt[100] = {0};
	int i = 0;
	while (1)
	{
		int c = fgetc(fp);
		if (c == EOF)
		{
			break;
		}
		txt[i++] = c;
		printf("%d ", c);
	}
	
	printf("\n%s", txt);
	return 0;
}
