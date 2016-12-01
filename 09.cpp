#include <iostream>

using namespace std;

class Translate {
public:
  Translate();
  virtual void LogTranslate() const = 0;
  // {
  //   cout << "parent " << endl;
  // }
  virtual void print(){
    cout <<  "print parent" << endl;
  }
};
void Translate::LogTranslate() const {
  cout << "parent " << endl;
}
Translate::Translate(){
  LogTranslate();
}

class BuyTranslate : public Translate{
public:
  virtual void LogTranslate() const{
    cout << "BuyTranslate " << endl;
  }
  virtual void print(){
    cout <<  "print BuyTranslate" << endl;
  }
};
class SellTranslate : public Translate{
public:
  virtual void LogTranslate() const{
    cout << "SellTranslate " << endl;
  }

};

int  main(int argc ,char** argv)
{

	BuyTranslate b;  //error 
  b.LogTranslate();
  Translate* t = new BuyTranslate;
  t->LogTranslate();
  //t->print();
  return 0;
}
