#ifndef _IRE_H_
#define _IRE_H_

#include "IRe.h"

class Re : public Home{
public:
    Re(){
        cout << "Re()" << endl;
    }
    virtual void open() {
        cout << "Re open" << endl;
    }
    virtual void close() {}
};

#endif

