#include <iostream>

using namespace std;

class Person
{
    public:
        Person(){}
        Person(const Person& p)
        {
            cout << "Copy Constructor" << endl;
        }

        Person& operator=(const Person& p)
        {
            cout << "Assign" << endl;
            return *this;
        }

    private:
        int age;
        string name;
};

void f(Person p)
{
    return;
}

Person f1()
{
    Person p;
    return p;
}

int main()
{
    Person p;
    cout << "1" << endl;
    Person p1 = p;    // 1
    Person p2;
    cout << "2" << endl;
    p2 = p;           // 2
    cout << "3" << endl;
    f(p2);            // 3

    cout << "4" << endl;
    p2 = f1();        // 4

    cout << "5" << endl;
    Person p3 = f1(); // 5

    return 0;
}
