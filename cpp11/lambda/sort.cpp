#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	std::vector<int> vec{7, 1, 5, 9, 0, 3};
	std::sort(vec.begin(), vec.end(), [](int a, int b) -> bool { return a < b; });
	std::cout << "sort result:" << std::endl;
	for (int it : vec)
	{
		std::cout << it << ' ';
	}
	std::cout << std::endl;

	return 0;
}
