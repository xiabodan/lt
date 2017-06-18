#include <stdlib.h>
#include <stdio.h>

class A {
    int a;
    static int b;
};

class B {
public:
    B() {
    }
    B(float m, float n, float k) {
        x = m;
        y = n;
        z = k;
    }

    B(const B& b) {
        printf("copy construcor\n");
        x = b.x;
        y = b.y;
        z = b.z;
    }

    B& operator=(const B& b) {
        printf("assigment construcor\n");
        x = b.x;
        y = b.y;
        z = b.z;
        return *this;
    }

    virtual ~B() {}

    static void sprint() {
        printf("&B::x=%p\n", &B::x);
        printf("&B::y=%p\n", &B::y);
        printf("&B::z=%p\n", &B::z);
    }

    void print() {
        printf("x=%f\n", x);
        printf("y=%f\n", y);
        printf("z=%f\n", z);
    }

public:
    static B b;
    float x,y,z;
};

B test(float j) {
    static float i = 1.0;
    i = i + j;
    B b(i, 1.2, 1.3);
    return b;
}

int main()
{
    // B::sprint();
    // printf("%d\n", sizeof(A));

    // B b(1.1, 2.2, 3.3);
    // b.print();
    // B b1 = b;
    // b1.print();

    B b3;
    b3 = test(2.3);
    // B b2 = test(3.4);

    return 0;
}

