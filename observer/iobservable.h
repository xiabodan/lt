#ifndef _IOBSERVABLE_H_
#define _IOBSERVABLE_H_

#include "iobserver.h"
#include <iostream>
using std::string;
class IObservable
{
public:
    IObservable(void)
    {
    }
    virtual ~IObservable(void)
    {
    }
    virtual void AddObserver(IObserver *pObserver) = 0;
    virtual void DeleteObserver(IObserver *pObserver) = 0;
    virtual void NotifyObservers(string context) = 0;
};

#endif
