#include <iostream>

int main()
{
	void* ptr = NULL;
	if (ptr == nullptr) {
		std::cout << "ptr == nullptr" << std::endl;
	} else {
		std::cout << "ptr != nullptr" << std::endl;
	}
	return 0;
}
