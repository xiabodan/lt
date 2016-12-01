#ifndef _HANFEIZI_H_
#define _HANFEIZI_H_

#include "iobservable.h"
#include "iobserver.h"
#include <vector>
using std::vector;
class CHanFeiziObservable : public IObservable
{
public:
    CHanFeiziObservable(void);
    ~CHanFeiziObservable(void);
    void AddObserver(IObserver *pObserver);
    void DeleteObserver(IObserver *pObserver);
    void NotifyObservers(string context);
    void HaveBreakfast();
    void HaveFun();
private:
    vector<IObserver*> m_observerList;
    typedef vector<IObserver*>::const_iterator ObserverList_C_iterator;
};

#endif
