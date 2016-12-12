#ifndef _PERSON_H_
#define _PERSON_H_

#include <iostream>
#include <tr1/memory>

class Address;
class Date;
// #include "Date.h"

class Person {
public:
  Person() { }
  virtual ~Person(){}

  virtual std::string name() const = 0;
  virtual std::string birthDate() const = 0;
  virtual std::string address() const = 0;
  static std::tr1::shared_ptr<Person>// 返回一个 tr1::shared_ptr ，
         create(const std::string& name,   // 它指向一个 Person 对象，这个
                const Date& birthday,       // Person 对象由给定的参数初始化，
                const Address& addr);       // 为什么返回智能指针参见第 18 条 
};

#endif
