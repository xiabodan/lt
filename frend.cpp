#include <iostream>

using namespace std;
class A{
    class B{
    private:
        A* pa;
    public:
        B(A* p):pa(p){}
        void fun(){cout<<pa->a<<endl;}
    };
private:
    int a;
    B b;
    void get(){cout<<a<<endl;}
public:
    A(int i):a(i),b(this){}
    void showInnerB(){b.fun();}
};
int main( void ){
    A a(999);
    a.showInnerB();
 
    //system("pause");
    return 0;
}
