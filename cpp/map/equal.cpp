#include <iostream>
#include <map>

int main()
{
	std::map<int, int> map1, map2;
	map1.insert(std::make_pair(1, 1));
	map1.insert(std::make_pair(2, 1));
	map2.insert(std::make_pair(1, 1));
	std::cout << (map1 == map2 ? "equal" : "not equal") << std::endl;
	
	return 0;
}
