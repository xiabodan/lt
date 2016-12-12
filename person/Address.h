#ifndef _ADDRESS_H_
#define _ADDRESS_H_

#include <iostream>

class Address{
public:
    Address(std::string  s){
        add = s;
    }

    ~Address(){}

    std::string getadd() const {
        return add;
    }
private:
    std::string  add;
};

#endif
