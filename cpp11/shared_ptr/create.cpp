#include <iostream>

class C
{
public:
  C(int v)
  {
    m_v = v;
    std::cout << "contructor C, value " << m_v << std::endl;
  }
  ~C()
  {
    std::cout << "destructor C" << std::endl;
  }
  
private:
  int m_v;
};

int main()
{
  std::shared_ptr<C> pShared1 = std::shared_ptr<C>(new C(1));
  //std::shared_ptr<C> pShared2(new C(2), [](C *p) {
  //  std::cout << "manul destructor C" << std::endl;
  //});
  std::shared_ptr<C> pShared3 = std::make_shared<C>(3);
  
  return 0;
}
