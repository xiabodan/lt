#include <stdio.h>
#include <stdlib.h>
//#include "test.h"

void print(void){
    printf("hello world\n");
    char s[10];
    int i = 0;
    for(i=0;i<10;i++){
        s[i] = i;
    }
    printf("s = %s\n",s);
}
