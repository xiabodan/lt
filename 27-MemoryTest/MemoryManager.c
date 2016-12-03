#include <stdio.h>
#include <stdlib.h>

#define MEMORYSIZE 1000000

void help(void){
    printf("\nplease input below fomati, Unit(byte)\n");
    printf("malloc(m) xxx \n");
    printf("free(f)    \n");
    printf("stop(s)       \n\n");
}
int main(int argc, char **argv){
    //printf("3: stop\n");

    long long totalNumber = 0;
    int number = 0;
    int flag = 0;
    void** p = NULL;
    while(1){
        long long numberts;
        char inputStr[100];

        help();
        scanf("%s",inputStr);
        if(((!strcmp(inputStr,"malloc"))||(!strcmp(inputStr,"m"))) && (flag == 0)) {
            scanf("%lld",&numberts);
            int index = numberts/MEMORYSIZE;
            int remainder = numberts%MEMORYSIZE;
            void* tmp = malloc(sizeof(void*) * (index + 1));
            p = (void**)memset(tmp, 0, (index+1)*(sizeof(void*)));
            if(numberts <=0){
                printf("Have memory %lld bytes\n",totalNumber);
                continue;
            }
            int i = 0;
            for(i=0;i<index;i++){
                if(NULL == (void*)p[i]){
                    p[i] = malloc(sizeof(char) * MEMORYSIZE);
                }
                if(NULL != p[i]){
                    number++;
                    totalNumber += MEMORYSIZE;
                    memset(p[i], 0, MEMORYSIZE);
                }
            }
            if(remainder != 0){
                if(NULL == p[i]){
                    p[i] = malloc(sizeof(char) * remainder);
                }
                if(NULL != p[i]){
                    number++;
                    totalNumber += remainder;
                    memset(p[i], 0, remainder);
                }
            }
            flag = 1;
        }
        if((!strcmp(inputStr,"free"))||(!strcmp(inputStr,"f"))){
            int i = 0;
            for(i=0;i<number;i++){
                if(NULL != p[i]) {
                    free(p[i]);
                    p[i] = NULL;
                }
            }
            free(p);
            totalNumber = 0;
            number = 0;
            flag = 0;
        }
        if((!strcmp(inputStr,"stop"))||(!strcmp(inputStr,"s"))){
            int i = 0;
            for(i=0;i<number;i++){
                if(NULL != p[i]) {
                    free(p[i]);
                    p[i] = NULL;
                }
            }
            free(p);
            p = NULL;
            number = 0;
            return 0;
        }

        printf("Exiting Have memory %lld bytes\n",totalNumber);
        usleep(10000);
    }

    return 0;
}
