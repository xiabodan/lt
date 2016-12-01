#include "HanFeiziObservable.h"
#include "LiSiObserver.h"
#include "ZhouSiObserver.h"
#include "iobserver.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;

void DoNewNew()
{
    //IObservable.h, HanfeiziObservable.h, IObserver.h, LiSiObserver.h
    cout << "----------用更新的方法再试试----------" << endl;
    IObserver *pLiSi = new CLiSiObserver();
    IObserver *pZhouSi = new CZhouSiObserver();

    CHanFeiziObservable *pHanFeiZi = new CHanFeiziObservable();

    pHanFeiZi->AddObserver(pLiSi);
    pHanFeiZi->AddObserver(pZhouSi);
    pHanFeiZi->HaveBreakfast();
    pHanFeiZi->HaveFun();

    delete pLiSi;
    pLiSi = NULL;
    delete pHanFeiZi;
    pHanFeiZi = NULL;
}


int main(int argc, char* argv[])
{
    //更抽象的方式，想要观察韩非子的人多了去了，不可能只允许李斯观察。
    DoNewNew();

    return 0;
}
