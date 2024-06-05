#include <iostream>
#include <memory>
#include <thread>


class Animal: public std::enable_shared_from_this<Animal>
{
public:
  Animal() {}
  Animal(std::string name) { m_name = name; }
  ~Animal() { std::cout << "destruct" << std::endl; }
  
public:
  std::shared_ptr<Animal> getptr()
  {
    return shared_from_this();
  }
  
  void running()
  {
    std::cout << m_name << " running" << std::endl;
  }
  
private:
  std::string m_name;
};

std::shared_ptr<Animal> getAnimal(std::string name)
{
  std::shared_ptr<Animal> pA = std::make_shared<Animal>(name);
  return pA->getptr();
}

void thread_func()
{
  std::shared_ptr<Animal> pA = getAnimal("dog");
  int count = 100;
  while (count-- > 0)
  {
    pA->running();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main()
{
  std::thread t(thread_func);
  t.join();
  return 0;
}
