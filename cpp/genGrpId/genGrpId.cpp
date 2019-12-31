#include <iostream>

int hashString(const std::string& channelId)
{
  int lHash = 0xf1e2d3c4;
  for (int i = 0; i < channelId.size(); ++i)
  {
    char c = channelId[i];
    lHash <<= 1;
    lHash += c;
  }
  return lHash;
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "invalid arg" << std::endl;
    return -1;
  }

  uint32_t hashValue = hashString(argv[1]);
  std::cout << "GroupId: " << hashValue << std::endl;
  return 0;
}

