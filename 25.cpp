#include <iostream>
#include <string.h>
using namespace std;

class Widget{
public:
    Widget(const char* s){
        if(NULL == s){
            p = new char[1];
            *p = '\0';
            size = 0;
        }else{
            int m = strlen(s);
            p = new char[m+1];
            strcpy(p,s);
            size = m;
        }
    }
    ~Widget(){
        delete[] p;
    }
    Widget(const Widget& ths){
        int m = strlen(ths.p);
        p = new char[m+1];
        strcpy(p,ths.p);
        size = m;
    }
    Widget& operator=(const Widget& ths){
        if(this != &ths){
            char* tmp = new char[strlen(ths.p) + 1];
            strcpy(tmp,ths.p);
            delete[] p;
            p = tmp;
            size = strlen(ths.p); 
        }
        return *this;
    }
    char* getp(){
        return p;
    }
    void swap1(Widget& ths){ // 是我们自己实现的高效交换函数
        cout << "member swap" << endl;
        using std::swap;// 必须声明std::swap
        swap(p,ths.p);// 调用std的swap
    }

private:
    char* p;
    int size;
};

namespace std{
    template <>
    void swap<Widget>(Widget& l, Widget& r) { // 全特化特化std 中的swap函数,关于全特化和偏特化的Demo见TemplateFunc.cpp
        cout << "non-member swap" << endl;
        l.swap1(r);
    }
}

int main(){
    Widget A("hubert");
    Widget B("xia");
    cout << A.getp() << " " << B.getp() << endl;
    std:swap(A,B); //尽管使用了std,但是调用的是swap的全特化版本
    cout << A.getp() << " " << B.getp() << endl;
    cout << endl;

    cout << A.getp() << " " << B.getp() << endl;
    A.swap1(B);
    cout << A.getp() << " " << B.getp() << endl;
    cout << endl;

    cout << A.getp() << " " << B.getp() << endl;
    swap(A,B);
    cout << A.getp() << " " << B.getp() << endl;

    return 0;
}
