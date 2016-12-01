#include <iostream>

class A {
private:
    static const int Number = 5;
    const int *p;
    void funA(void);
};


//const int A::Number; //如果要引用Number，就必须给出定义式

void A::funA(void) {
//    p = &Number;
    int a = Number;
}

int main() {

    return 0;
}
