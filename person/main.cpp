#include <iostream>
#include "Person.h"
#include "Date.h"
#include "Address.h"

// 创建一个支持 Person 接口的对象
int main(){
    std::string name = "hubert";
    Date dateOfBirth("1989.12.07");
    Address address("shanghai");

    std::tr1::shared_ptr<Person> pp(Person::create(name, dateOfBirth, address));
    std::cout << pp->name()         // 通过 Person 的接口使用这一对象
          << " was born on "
          << pp->birthDate()
          << " and now lives at "
          << pp->address() 
          << std::endl;
}
