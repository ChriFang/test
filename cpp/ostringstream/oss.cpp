#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

int main()
{
	uint32_t n = -1;
	ostringstream oss;
	oss << n;
	cout << oss.str() << endl;
	
	return 0;
}
