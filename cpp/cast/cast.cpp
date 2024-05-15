#include <iostream>

int main()
{
  int num = 0x00636261;
  float fNum = static_cast<float>(num); // 转换基础类型
  std::cout << "int value: " << std::hex << num << std::endl;
  std::cout << "float value: " << fNum << std::endl;
  // fNum = 12.345;
  // num = static_cast<int>(fNum);
  // std::cout << "int value: " << std::dec << num << std::endl;
  
  int* pInt = &num;
  char* pChar = reinterpret_cast<char*>(pInt); // 转换指针
  std::cout << "int point: " << pInt << std::endl;
  std::cout << "string point: " << pChar << std::endl;
  
  const int kValue = 1;
  int* pModify = const_cast<int*>(&kValue); // 去掉const属性
  *pModify = 2;
  std::cout << "after modify, kValue: " << kValue << " modify: " << *pModify << std::endl;
}
