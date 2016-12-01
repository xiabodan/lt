#include <iostream>

using namespace std;


class NameObject {
public:
	virtual ~NameObject() = 0;
	virtual void Print() = 0;

	
};
NameObject::~NameObject(){

}
void NameObject::Print(){

}
class Derived : public NameObject{
public:
	virtual ~Derived(){

	}
	void Print(){
		
	}
};
//-------------------
class Base
{
public:
	virtual void Print(){
		cout << "Base print" << endl;
	}

	int a;
};
class Base_D :public Base
{
public:
	virtual void Print(){
		cout << "Derived print" << endl;
	}
};



int  main(int argc ,char** argv)
{
	NameObject* n = new Derived;
	//---------------------
	Base* b = new Base;
	b->Print();
	Base* b1 = new Base_D;
	b1->Print();
	Base_D* d = static_cast<Base_D*> (b);
	//Base_D* d1 = new Base;  //error!!
	d->Print();


	//const_cast
	Base* c = new Base;
	c->a = 10; // error !! c is read only
	const Base* c1 = const_cast<Base*>(c);
	//c1->a = 23;

   	return 0;
}
