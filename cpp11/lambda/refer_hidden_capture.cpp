#include <iostream>

using namespace std;

int main()
{
	int a = 123;
	auto f = [&] { cout << a << endl; };
	a = 321;
	f();

	return 0;
}