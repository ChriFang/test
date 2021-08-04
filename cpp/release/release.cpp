#include <iostream>
#include <string>

class A
{
public:
  A(std::string method);
  ~A();
  
private:
  std::string m_method;
};

A::A(std::string method)
{
  m_method = method;
  std::cout << "construct A -- " << m_method << std::endl;
}

A::~A()
{
  std::cout << "release A -- " << m_method << std::endl;
}

A oa("static");
A* pa = new A("new");

int main()
{
  return 0;
}
