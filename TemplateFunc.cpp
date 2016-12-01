#include <iostream>

// 原模板函数
template <typename T1, typename T2>
int compare(const T1& lhs, const T2& rhs)
{
    std::cout << "template compare" << std::endl;
    return 0;
}

// 普通函数
int compare(const char* lhs, const char* rhs)
{
    std::cout << "ordinary compare" << std::endl;
    return 0;
}

// 模板函数重载
template <typename T1>
int compare(const T1& lhs, const char* rhs)
{
    std::cout << "Template overloade compare" << std::endl;
    return 0;
}

// 模板函数重载
template <typename T1, typename T2>
int compare(const T1& lhs, const T2& rhs, double d)
{
    std::cout << "template overload compare" << std::endl;
    return 0;
}

// 模板函数全特化
template <>
int compare<int, double>(const int& lhs, const double& rhs)
{
    std::cout << "Template whole special" << std::endl;
    return 0;
}

// 模板函数偏特化 --不允许
// template <typename T2>
// int compare<int, T2>(const int& lhs, const T2& rhs)
// {
//     std::cout << "Template half special" << std::endl;
//     return 0;
// }

int main(int argc, char *argv[])
{
    char c[] = "hello";
    char c0[] = "hello";
    compare(c, c0); // 原模板函数

    const char c1[] = "hello";
    const char c2[] = "hello";
    compare(c1, c2); // 普通函数

    const short c3= 7;
    const char c4[] = "hello";
    compare(c3, c4);// 模板函数重载

    compare(c1, c2, 2.3);// 模板函数重载

    const int c5 = 7;
    const double c6 = 0.32;
    compare(c5, c6);// 模板函数全特化
}
