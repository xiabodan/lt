#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    //输入2b,打印出2;输入b2,打印出 "cin is fail"
    int itest = 0; 
    if(!(cin >> itest)){
        cerr << "cin is fail" << endl;
        return EXIT_FAILURE;
    }  
    cout << itest <<endl;

    //不管输入什么，均能输出，因为都符合字符串
    string str;
    if(!(cin >> str)){
        cerr << "cin is fail" << endl;
        return EXIT_FAILURE;
    }
    cout << str << endl;

    //跟itest一样
    double itest1 = 0;
    cin >> itest1;
    if(!cin){
        cerr << "cin is fail" << endl;
        return EXIT_FAILURE;
    }  
    cout << itest1 <<endl;

    return EXIT_SUCCESS;
}