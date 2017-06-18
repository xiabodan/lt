#include<iostream>  
using namespace std;  
class Complex  
{  
    public:  
        Complex(){real=0;imag=0;}  
        Complex(double r,double i)  
        {  
            real=r;  
            imag=i;  
        }  
        friend Complex operator+(Complex &c1,Complex &c2);  
        friend ostream & operator << (ostream &output,Complex &c);  
        friend istream & operator >> (istream &input ,Complex &c);  
    private:  
        double real;  
        double imag;  
};  
Complex operator+(Complex &c1,Complex &c2)  
{  
    return Complex(c1.real+c2.real,c1.imag+c2.imag);  
}  
ostream & operator << (ostream &output,Complex &c)  
{  
    output<<"("<<c.real<<","<<c.imag<<")"<<endl;  
    return output;  
}  
istream & operator >> (istream &input ,Complex &c)  
{  
    input>>c.real>>c.imag;  
    return input;  
}  
int main()  
{  
    Complex c1,c2,c3;  
    cout<<"please input c1:";  
    cin>>c1;  
    cout<<"please input c2:";  
    cin>>c2;  
    c3=c1+c2;  
    cout<<"c3=c1+c2"<<endl;  
    cout<<"c1:"<<c1;  
    cout<<"c2:"<<c2;  
    cout<<"c3=c1+c2"<<endl;  
    cout<<"c3:"<<c3;  
    cout<<c3;  
    int a = 18;
    cout<<a;
    return 0;  
}  

