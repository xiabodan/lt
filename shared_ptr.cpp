#include <iostream>
#include <tr1/memory>

class A {

};

class B {
public:
    B() {
        // a = std::tr1::shared_ptr(new A());
        a.reset(new A());
    }
    std::tr1::shared_ptr<A> a;
};


int main() {
    B b;

    return 0;
}
