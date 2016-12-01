#include <iostream>
#include <tr1/memory>

using namespace std;

class Invest {

};

class Investment {
public:
    Investment(){
        cout << "create function" << endl;
    }
    ~Investment(){
        cout << "delete function" << endl;
    }
    int getDayHeld() {
        return 1;
    }
    // 隐式转换
    operator Invest() const {
        return inv;
    }

    Invest inv;
};

int InvestmentSpecial(Investment* inv){
    inv->getDayHeld();
    cout << "Special delete function" << endl;
}

std::tr1::shared_ptr<Investment> createInvestment() {
    Investment* invPtr = new Investment();
    std::tr1::shared_ptr<Investment> inv(invPtr, InvestmentSpecial);
    return inv;
}



int main(int argc, char* argv[]) {
    std::tr1::shared_ptr<Investment> inv = createInvestment();

    return 0;
}
