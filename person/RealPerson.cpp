#include <iostream>
#include "RealPerson.h"

RealPerson::RealPerson(const std::string& name, const Date& birthday,const Address& addr)
  : theName(name), theBirthDate(birthday), theAddress(addr){

  }
RealPerson::~RealPerson() {}

std::string RealPerson::name() const{return theName;}      // 这里省略了这些函数的具体实现，
std::string RealPerson::birthDate() const{return theBirthDate.getdata();} // 但是很容易想象它们是什么样子。
std::string RealPerson::address() const{return theAddress.getadd();}

