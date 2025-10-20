#include <iostream>

int main()
{
	// 原始字符串字面量，使用分隔符来消除同时含有双引号和括号的原始字符串
	// const char* bad_parens = R"()")";  // error C2059
	const char* bad_parens = R"_()")_";
	std::cout << bad_parens << std::endl;
	
	const char* testStr = R"(
			SELECT column1, column2
			FROM your_table
			WHERE condition = 'value'
		)";
	std::cout << testStr << std::endl;
	return 0;
}
