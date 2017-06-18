#include <iostream>
#include <string.h>
using namespace std;


namespace WidgetStuff{

    template<typename T>
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
        char* getp() {
            return p;
        }
        void swap(Widget& ths) {
            cout << "member swap" << endl;
            using std::swap;// std
            swap(p,ths.p);
        }

    private:
        char* p;
        int size;
    };

    template<typename T>
    void swap(Widget<T>& l, Widget<T>& r) {// swap的重载版本,而不是特化版本
        cout << "non-member swap" << endl;
        l.swap(r);
    }

    template<typename T>
    void doSomething(T& l, T& r) {
        using std::swap;
        // std::swap(l,r);// 这样就会调用std的swap
        swap(l,r);

        int a = 10;
        int b = 20;
        swap(a,b);//正常调用普通std内部的swap
    }

}// namespace WidgetStuff end

using namespace WidgetStuff;
int main(){
    Widget<char> A("hubert");
    Widget<char> B("xia");
    cout << A.getp() << " " << B.getp() << endl;
    doSomething(A,B);
    cout << A.getp() << " " << B.getp() << endl;
    cout << endl;

    return 0;
}
