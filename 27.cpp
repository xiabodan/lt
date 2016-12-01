#include <iostream>

using namespace std;

class NameObject {
public:
    NameObject(int t);
    virtual ~NameObject();
    virtual void print() {
        cout << "BASE PRINT" << endl;
        cout << "base a:" << a << endl;
        a = 100; // 如果是副本中修改成员变量的值无意义
    }
    int geta() { return a; }
    int seta(int t) { a = t; }
private:
    int a;
};

NameObject::NameObject(int t){
    a = t;
}

NameObject::~NameObject(){

}

class Derived : public NameObject{
public:
    Derived(int t)
        : NameObject(t) {

    }
    virtual ~Derived(){

    }
    virtual void print(){
        // static_cast<NameObject>(*this).print();// 创建的是一个*this对象的base成分的副本,打算在base print中修改a=100(但实际不成功)
        NameObject::print();// 现在base中对成员变量的修改就正确生效了

        cout << "Derived PRINT" << endl;
        cout << "derived a:" << geta() << endl;// 如果采用static_cast转型,则a的值还是构造是的值为10
        seta(1000);
        cout << "derived a:" << geta() << endl;// a的值现在是1000
    }
};

int  main(int argc ,char** argv)
{
    NameObject* n = new Derived(10);
    n->print();

    return 0;
}
