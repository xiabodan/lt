#include <iostream>

using namespace std;

template<class T>
class NameObject {
public:
   	NameObject (std::string& name,  const T& value)
   	:nameValue(name),
   	 objectValue(value)
   	{
   		//objectValue++;  //objectValue is read only
   	}
private:
	std::string& nameValue;
    const T& objectValue;
};

int  main(int argc ,char** argv)
{
	std::cout<<"main"<<endl;
    std::string newDog("persephone");
	std::string oldDog("Satch");
	NameObject<int> p(newDog,2);
	NameObject<int> s(oldDog,3);
	
	//p = s;		//error
	
   	return 0;
}
