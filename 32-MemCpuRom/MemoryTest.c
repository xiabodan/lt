#include <stdio.h>
#include <stdlib.h>

#define MEMORYSIZE 1000000

int main(int argc, char **argv){
    long long totalNumber = 0;
    int number = 0;
    int flag = 0;
    void** p = NULL;
    if (argc<2){
        return 0;
    }
    long long numberts = atoi(argv[1]);
    printf("Begain Memory Test num:%lld\n",numberts);
    while(1){
        char inputStr[100];
        if(flag == 0) {
            int index = numberts/MEMORYSIZE;
            int remainder = numberts%MEMORYSIZE;
            void* tmp = malloc(sizeof(void*) * (index + 1));
            p = (void**)memset(tmp, 0, (index+1)*(sizeof(void*)));
            if(numberts <=0){
                //printf("Have memory %lld bytes\n",totalNumber);
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
                printf("Have memory %lld bytes\n",totalNumber);
                usleep(10000); // sleep 1s use for watch
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
            printf("Have memory %lld bytes\n",totalNumber);
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
            p = NULL;
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

        //printf("Exiting Have memory %lld bytes\n",totalNumber);
        usleep(1000000);
    }

    return 0;
}
