#include <iostream>
#include <tr1/memory>

using namespace std;

class Invest {

};

class Investment {
public:
    int getDayHeld() {
        return 1;
    }
    // 隐式转换
    operator Invest() const {
        return inv;
    }

    Invest inv;
};

Investment* createInvestment() {
    Investment* inv = new Investment();
    return inv;
}

void changeClass(Invest inv) {

}

int dayHeld(const Investment* pi) {
    return const_cast<Investment*>(pi)->getDayHeld();
}

int main(int argc, char* argv[]) {
    std::tr1::shared_ptr<Investment> pInv(createInvestment());
    // int day = dayHeld(const_cast<Investment*>(static_cast<const Investment*>(pInv.get())));
    int day = dayHeld(pInv.get());
    int day1 = (*pInv).getDayHeld();
    int day2 = pInv->getDayHeld();
    cout << "day " <<  day << endl;
    cout << "day " <<  day1 << endl;
    cout << "day " <<  day2 << endl;

    changeClass(*pInv.get());

    return 0;
}
