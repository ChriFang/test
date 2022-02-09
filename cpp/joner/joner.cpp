#include <iostream>

using namespace std;

char name_a[20] = "aaa";
char name_b[20] = "bbb";
char name_c[20] = "ccc";
char name_d[20] = "ddd";

/**
 * 方式一：展开成对应的变量，直接打印
 */
#define PRINT(n) {cout << name_##n << endl;}

/*
 * 方式二：展开成对应的函数，再调用函数打印
 *
void print_a()
{
	cout << name_a << endl;
}

void print_b()
{
	cout << name_b << endl;
}

void print_c()
{
	cout << name_c << endl;
}

void print_d()
{
	cout << name_d << endl;
}

#define PRINT(n) {print_##n();}
*/

int main()
{
	PRINT(a);
	PRINT(b);
	PRINT(c);
	PRINT(d);
	return 0;
}