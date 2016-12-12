#ifndef _REALPERSON_H_
#define _REALPERSON_H_

#include <iostream>
#include "Person.h"
#include "Date.h"
#include "Address.h"
//class Date;
//class Address;

class RealPerson: public Person {
public:
  RealPerson(const std::string& name, const Date& birthday,const Address& addr);

  virtual ~RealPerson();

  std::string name() const;      // 这里省略了这些函数的具体实现，
  std::string birthDate() const; // 但是很容易想象它们是什么样子。
  std::string address() const;

private:
  std::string theName;
  Date theBirthDate;
  Address theAddress;
};

#endif
