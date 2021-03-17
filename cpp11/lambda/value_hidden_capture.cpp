#include <iostream>

using namespace std;

int main()
{
	int a = 123;
	auto f = [=] { cout << a << endl; };
	f();

	return 0;
}