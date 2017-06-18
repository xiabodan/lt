#include <iostream>

int main() {
    int a = 10;
    __typeof__(a) b = 100;
    std::cout << b << std::endl;
    __typeof__(int) c = 1000;
    std::cout << c << std::endl;

    return 0;
}
