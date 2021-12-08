#include <iostream>
#include <algorithm>

using namespace std;

class OOT
{
public:
	explicit OOT(int v)
	: m(v)
	{
	}
	
	int value() const
	{
		return m;
	}

public:
	inline bool operator > (const OOT& x) const
	{
		return m > x.m;
	}
	
	inline bool operator < (const OOT& x) const
	{
		return m < x.m;
	}
	
	inline bool operator == (const OOT& x) const
	{
		return m == x.m;
	}

private:
	friend OOT operator + (const OOT& x, const OOT& y);

private:
	int m;
};

// 体外重载（不喜欢这种方式 :=( ），需要声明友元才能访问私有变量
OOT operator + (const OOT& x, const OOT& y)
{
	return OOT(x.m + y.m);
}

int main()
{
	OOT o1(1);
	OOT o2(2);
	cout << "o1>o2: " << (o1 > o2 ? "true" : "false") << endl;
	cout << "o1<o2: " << (o1 < o2 ? "true" : "false") << endl;
	cout << "o1==o2: " << (o1 == o2 ? "true" : "false") << endl;
	
	// 测试 std
	cout << "min: " << std::min(o1, o2).value() << endl;
	cout << "max: " << std::max(o1, o2).value() << endl;
	
	// 体外重载测试
	cout << "sum: " << (o1 + o2).value() << endl;
	
	return 0;
}