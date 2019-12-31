#include <iostream>
#include <queue>

struct TestItem
{
	int a;
	int b;
	std::string c;
};

int main()
{
	std::queue<TestItem> myqueue;
	
	TestItem item = {0, 1, "foo"};
	myqueue.push(item);
	
	TestItem& item_index = myqueue.front();
	std::cout << "myqueue.front() is now " << item_index.c << std::endl;
	
	myqueue.pop();
	TestItem& item_index2 = myqueue.front();
	std::cout << "after pop(), queue size= " << myqueue.size() << std::endl;
	std::cout << "myqueue.front() is now " << item_index2.c << std::endl;
	
	return 0;
}
