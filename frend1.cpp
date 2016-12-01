#include <iostream>
#include <math.h>

using namespace std;
class Point2D;//声明

class Point//声明
{
public:
    Point(double xx, double yy) { x=xx; y=yy; }//默认构造函数
    void Getxy();//公有成员函数
    friend double Distance(Point &a, Point &b, Point2D &c, Point2D &d);  //同时声明为两个类的友元函数
private:
    double x, y;
};

void Point::Getxy(){
    //cout<<"("<
}


class Point2D//声明
{
public:
    Point2D(double xx, double yy) { x=xx; y=yy; }//默认构造函数
    void Getxy();//公有成员函数
    friend double Distance(Point &a, Point &b, Point2D &c, Point2D &d);//同时声明为两个类的友元函数
private:
    double x, y;
};

void Point2D::Getxy(){
    //cout<<"("<
}

double Distance(Point &a, Point &b, Point2D &c, Point2D &d)  //注意函数名前未加类声明符
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    double dx1 = c.x - d.x;
    double dy1 = c.y - d.y;
    return sqrt(dx1*dx1+dy1*dy1) + sqrt(dx*dx+dy*dy);
}

int main(){
    Point   p1(3.0, 4.0), p2(6.0, 8.0);
    Point2D p3(3.0, 4.0), p4(6.0, 8.0);
    p1.Getxy();
    p2.Getxy();
    p3.Getxy();
    p4.Getxy();
    double d = Distance(p1, p2, p3, p4);
    cout<<"Distance is "<< d << endl;

    return 0;
}
