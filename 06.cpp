#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

using namespace std;

class Point//声明
{
public:
    Point() {}
    Point(double xx, double yy, string s, char* ch) { x = xx; y = yy; str = s; c = ch;}//默认构造函数
    ~Point() {}
    string getString();//公有成员函数
    char* getChar();//公有成员函数
    void freeC();
    void setString(string string);
    friend double Distance(Point &a, Point &b);  //同时声明为两个类的友元函数
    friend void copy(Point &a, Point &b);  //同时声明为两个类的友元函数
private:
    // Point(const Point&);
    // Point& operator=(const Point&);
    double x, y;
    string str;
    char* c;
};

string Point::getString(){
    return str;
}

char* Point::getChar(){
    return c;
}

void Point::setString(string string){
    str = string;
}

void Point::freeC(){
    free(c);
}

double Distance(Point &a, Point &b)  //注意函数名前未加类声明符
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    return sqrt(dx*dx+dy*dy);
}

void copy(Point &a, Point &b)  //注意函数名前未加类声明符
{
    Point p = b;
}

int main(){
    string s1 = "Hello";
    string s2 = "World";
    char* c1 = new char[4];
    char* c2 = new char[4];
    c2[0] = 'x'; c2[1] = 'i'; c2[2] = 'a'; c2[3] = '\0';
    c1[0] = 'd'; c1[1] = 'a'; c1[2] = 'n'; c1[3] = '\0';
    Point   p1(3.0, 4.0, "Hello", c1), p2(6.0, 8.0, s2, c2);

    Point   p3(p1);
    Point   p4 = p2;

    double d = Distance(p1, p2);
    cout<<"Distance is "<< d << endl;
    cout<<"str="<< p1.getString() << endl;
    cout<<"str="<< p2.getString() << endl;
    cout<<"str="<< p1.getChar() << endl;
    cout<<"str="<< p2.getChar() << endl;
    // p2.freeC();
    p1.freeC();
    p2.setString("modify");
    cout<<"str="<< p2.getString() << endl;
    double d1 = Distance(p3, p4);
    cout<<"Distance is "<< d1 << endl;
    cout<<"str="<< p3.getString() << endl;
    cout<<"str="<< p4.getString() << endl;
    cout<<"str="<< p3.getChar() << endl;
    cout<<"str="<< p4.getChar() << endl;

    Point   p5(2.3, 5.5, s1, c1);
    Point   p6;
    copy(p1, p5);

    return 0;
}
