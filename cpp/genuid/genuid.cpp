#include <iostream>

uint32_t getHashValue(const char* str)
{
  uint32_t hashValue = 0;
  while (*str != 0)
  {
    hashValue = 31 * hashValue + (*str);
    ++str;
  }
  return hashValue;
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "invalid arg" << std::endl;
    return -1;
  }
  std::cout << "uid: " << getHashValue(argv[1]) << std::endl;
  return 0;
}
