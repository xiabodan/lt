#include <iostream>
#include <tr1/memory>
#include <tr1/functional>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
// using namespace std::placeholders;

class Point {
public:
    Point(int x1, int y1) {
        x = x1;
        y = y1;
    }
    Point() {}
    void setX(int x1) { x = x1;}
    void setY(int y1) { y = y1;}
    const int getX(void) const { return x;}
    const int getY(void) const { return y;}
private:
    int x;
    int y;
};

struct RectData {
    Point ulhc;
    Point lrhc;
    // RectData & operator=(const RectData r) {
    //      RectData r1;
    //      r1.ulhc = r.ulhc;
    //      r1.lrhc = r.lrhc;
    // }
};

class Rectangle {
public:
    Rectangle(Point cu, Point cl)
        // : pData(new RectData()) 
    {
        cout << "Rectangle" << endl;
        // pData = new RectData(); // !!error, shared_ptr can't use this
        pData.reset(new RectData());
        pData->ulhc = cu;
        pData->lrhc = cl;
        cout << "x:" << pData->ulhc.getX() << endl;
        cout << "y:" << pData->ulhc.getY() << endl;
    }
    const Point& upperLeft() const { return pData->ulhc; }
    const Point& lowerLeft() const { return pData->lrhc; }
private:
    std::tr1::shared_ptr<RectData> pData;
};

class GUIObject {

};

const Rectangle boundingBox(const GUIObject& obj) {
    Point c1(100,100);
    Point c2(1000,1000);
    const Rectangle r1(c1, c2);
    // Rectangle r(r1);
    return r1;
}

class Base {
public:
    virtual void bb() {}
    char c;
    int b;
    int d;
};
class Davied : public Base {
public:
    virtual void dd() {}
    virtual void bb() {}
    virtual void xx() {}
    void ee() {}
    char d;
};
void test(Base* b) {
    // printf("test Base(%p)\n", b);
    // Davied* d = dynamic_cast<Davied*> (b);
    // printf("test Davied(%p)\n", d);
}

int main() {
    // GUIObject* pgo = new GUIObject();
    // const Point* pUpperLeft = &(boundingBox(*pgo).upperLeft());
    // cout << "x:" << pUpperLeft->getX() << endl; // 为0
    // cout << "y:" << pUpperLeft->getY() << endl; // 为0,因为boundingBox(*pgo)是个临时对象,执行完成就被析构了

    // const Rectangle r = boundingBox(*pgo);
    // const Point* p = &(r.upperLeft());
    // cout << "x:" << pUpperLeft->getX() << endl; // 为100
    // cout << "y:" << pUpperLeft->getY() << endl; // 为100,因为boundingBox(*pgo)对象被保存下来了

    Base* b = new Davied();
    printf("test Base(%p)\n", b);
    printf("Base size(%d)\n", sizeof(Base));
    printf("Base &c(%p)\n", &b->c);
    printf("Base &b(%p)\n", &b->b);
    printf("Base &d(%p)\n", &b->d);
    printf("Davied size(%d)\n", sizeof(Davied));
    printf("Base &bb(%p)\n", &Base::bb);
    printf("Davied &bb(%p)\n", &Davied::bb);
    printf("Davied &dd(%p)\n", &Davied::dd);
    printf("Davied &xx(%p)\n", &Davied::xx);
    printf("Davied &ee(%p)\n", &Davied::ee);
    test(b);

    return 0;
}
