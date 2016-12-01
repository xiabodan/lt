#include <iostream>

class A{
    public:
void test();
static const void print();
};
const void A::print(){

    std::cout << "sfassd" << std::endl;
}
void A::test(){
    print();
}

int main(){
    A aa;
    aa.test();

}


