#include <iostream>

using namespace std;

class Rational {
public:
    //explicit Rational(int m1 = 0, int n1 = 1) { // 如果禁止隐式转换,main中的 oneHalf * 2 编译出错{
    Rational(int m1 = 0, int n1 = 1) {
        n = n1;
        m = m1;
    }
    int getm() const{
        return m;
    }
    int getn() const{
        return n;
    }
    //不该为friend或者member函数
    //friend const Rational operator* (const Rational& rhs, const Rational& lth){
    //    return Rational(rhs.m * lth.m, rhs.n * lth.n);
    //}
    const Rational operator* (const Rational& rhs){
        cout << "member ";
        return Rational(rhs.m * this->m, rhs.n * this->n);
    }
private:
    int n;
    int m;
};

// 合理的应该是个non-member函数
const Rational operator* (const Rational& rhs, const Rational& lth){
    cout << "non-member ";
    return Rational(rhs.getm() * lth.getm(), rhs.getn() * lth.getn());
}


int main(){
    Rational oneEight(1,8);
    Rational oneHalf(1,2);
    Rational r = oneEight * oneHalf;
    cout << "oneEight*oneHalf m=" << r.getm()  << endl;
    cout << "oneEight*oneHalf n=" << r.getn()  << endl;

    r = oneHalf * 2;
    cout << "oneEight*oneHalf m=" << r.getm()  << endl;
    cout << "oneEight*oneHalf n=" << r.getn()  << endl;

    r = 2 * oneEight;//2被作为函数参数列中时，会发生隐式转换城Rational对象
                     //但是如果构造函数加上explicit Rational,则2不能被隐式转换
    cout << "oneEight*oneHalf m=" << r.getm()  << endl;
    cout << "oneEight*oneHalf n=" << r.getn()  << endl;

    return 0;
}
