#ifndef _DATE_H_
#define _DATE_H_

#include <iostream>

class Date{
public:
    Date(std::string n){
        std::cout << "new" << std::endl;
        std::cout << "new" << std::endl;
        data = n;
    }
    ~Date() { }

    std::string getdata() const {
        return data;
    }
private:
    std::string  data;
    const static int a = 10;
};

#endif
