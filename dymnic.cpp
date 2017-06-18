#include <iostream>

class A {
};

class D {
public:
    D(A* aa){
        a = aa;
    }
private:
    A* a;
};

class B {
public:
    class C : public A {
    };

    B() {
        c = new C();
        d = new D(c);
    }
private:
    A* c;
    D* d;
};

int main() {
    B* b = new B();
    return 0;
}
