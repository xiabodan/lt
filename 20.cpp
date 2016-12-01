#include <iostream>

using namespace std;

class Translate {
public:
    Translate(const string s, int value)
    :name(s),
     value(value)
    {
        cout << value << " "  << name << endl;
        cout << "parent normal " << endl;
    }
    Translate(const Translate& ths);
    Translate& operator=(const Translate& ths);
  
    string name;
    int value;
};

Translate::Translate(const Translate& ths)
  :name(ths.name)
{
    cout << "parent copy constructor" << endl;
}
Translate& Translate::operator=(const Translate& ths){
    cout << "parent copy assignment operator" << endl;
    name  = ths.name;
    return *this;
}

class BuyTranslate : public Translate{
public:
    BuyTranslate(const string s ,int vulue, int priority )
    :priority(priority),
    Translate(s,value)
    {
        cout << "child normal " << endl;
    }
    BuyTranslate(const BuyTranslate& ths);
    BuyTranslate& operator=(const BuyTranslate& ths);

public:
    int priority;
};

BuyTranslate::BuyTranslate(const BuyTranslate& ths)
  :Translate(ths),
  priority(ths.priority)
{
  cout << "child copy constructor" << endl;
}

BuyTranslate& BuyTranslate::operator=(const BuyTranslate& ths){
    cout << "child copy assignment operator" << endl;
    priority  = ths.priority;
    Translate::operator=(ths);
    return *this;
}

bool validateTranslate(BuyTranslate s){
    
    return true;
}

bool validateTranslate2(const BuyTranslate& s){
    
    return true;
}

int  main(int argc ,char** argv)
{
    BuyTranslate plato("hubert",10,20);
    bool r = validateTranslate(plato);

    cout << endl;
    BuyTranslate hubert("xia",100,200);
    bool r2 = validateTranslate2(hubert);    

    return 0;
}
