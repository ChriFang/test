#include <iostream>


struct true_type {
    static constexpr bool value = true;
};

struct false_type {
    static constexpr bool value = false;
};


template <typename>
struct is_void : false_type {};

template <>
struct is_void<void> : true_type {};


void test1();
int test2();

int main()
{
    std::cout << is_void<decltype(test1())>::value << std::endl; // 1
    std::cout << is_void<decltype(test2())>::value << std::endl; // 0
    return 0;
}

