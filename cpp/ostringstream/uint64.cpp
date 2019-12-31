#include <sstream>
#include <iostream>

int main()
{
	std::ostringstream oss;
	uint64_t i = 10;
	oss << i;
	std::cout << oss.str() << std::endl;
	
	return 0;
}
