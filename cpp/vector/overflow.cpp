#include <vector>
#include <string>
#include <iostream>

int main()
{
	std::vector<std::string> v;
	v.push_back("11");
	v.push_back("22");
	
	//v[3] is existed, but safe to get.
	std::cout << "v[3]=" << v[3] << ", length=" << strlen(v[3].c_str()) << std::endl;
	
	return 0;
}
