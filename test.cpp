#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//const int& get(int& b){
//    b = 10;
//    return b;
//}

#define TAGET qin2 
//#define xiabo
//#define hubert 

int& abc(int a, int b, int c, int& r){

	 r = a + b + c;
	 int s = 100 ;
	 return r;
}

void output(int x);
void output(int x, float y = 0.0 );

void output(int x){
    cout << x << endl;
}

void output(int x, float y ){
    cout << x << " " << y <<endl;
}

int main(){
    int c = 100;
    int b = abc(10,20,30,c);
    printf("b = %d\n",b);
    
    int x = 10;
    float y = 0.5;

#define haha1

#ifdef haha
    cout  << "haha #ifdef "  << endl;
#else 
    cout  << "haha #else "  << endl;
#endif 

#if defined(xiabo) || defined(hubert)
    cout  << "test #ifdef "  << endl;
#else 
    cout  << "test #else "  << endl;
#endif 


#if TAGET == qin 
    cout  << "2:test #ifdef "  << endl;
#else 
    cout  << "2:test #else "  << endl;
#endif 

    return 0;
}
