#ifndef _IRE_H_
#define _IRE_H_
#include <iostream>
#include <typeinfo>

using namespace std;

class Home{
public:
    virtual void open() = 0;
    virtual void close() = 0;
    
    static Home* Create();
};

#endif

