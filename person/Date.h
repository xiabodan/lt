#ifndef _DATE_H_
#define _DATE_H_

#include <iostream>

class Date{
public:
    Date(std::string n){
        data = n;
    }
    ~Date(){}

    std::string getdata() const {
        return data;
    }
private:
    std::string  data;
};

#endif
