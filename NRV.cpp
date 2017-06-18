#include <iostream>
#include <memory.h>

using namespace std;

class Test {
    friend Test foo(double);
public:
    Test() {
        memset(array, 0, 100*sizeof(double));
        cout << "----" << endl;
    }
    ~Test() {
        cout << "dddd" << endl;
    }
    Test(const Test& t) {
        memcpy(this, &t, sizeof(Test));
        cout << "++++" << endl;
    }
    Test& operator =(const Test& t) {
        cout << "====" << endl;
        return *this;
    }
private:
    double array[100];
};

Test foo(double val) {
    Test local;
    local.array[0] = val;
    local.array[99] = val;
    cout << "foo" << endl;

    return local;
}

int main() {
    // for(long long i=0;i<1;i++) {
        Test t;
        t = foo(double (1.1));
    // }

    // Test t1;
    // Test t2 = t1;
    // Test t3;
    // t3 = t1;

    return 0;
}
