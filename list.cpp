// constructing lists
#include <iostream>
#include <list>

std::list<int*> get() {
    std::list<int*> r;
    return r;
}

class A {
public:
    std::list<int*> first;
};

int main ()
{
    // constructors used in the same order as described above:
    A* a = new A();
    a->first = get();                                // empty list of ints
    std::cout << "first size " << a->first.size() << std::endl;

    // the iterator constructor can also be used to construct from arrays:

    std::cout << '\n';

    return 0;
}
