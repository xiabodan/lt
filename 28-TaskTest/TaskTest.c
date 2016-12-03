#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_TASK_NUM 100

pid_t pids[MAX_TASK_NUM];//task id
int indexPid = 0;  //task number index

int p[2];          //pipe
char pipeWrite[20];//write pipe buffer
char pipeRead[20]; //read  pipe buffer

int _setpriority(int n){
    int state = setpriority(PRIO_PROCESS, 0, n);

    return state;
}

void process(void){
    bool running = false;
    int oldProirity = 0;
    printf("Create Task Success, PID=%d\n", getpid());
    for(;;){
        read(p[0], pipeRead, sizeof(pipeRead));
        int mode = atoi(&pipeRead[1]);
        //close(p[0]);
        if((pipeRead[0] == 'w') && (mode == 1)){
            running = true;
        }
        if((pipeRead[0] == 's') && (mode == 0)){
            running = false;
        }
        if((pipeRead[0] == 'p') && (oldProirity != mode)) {
            int state = _setpriority(mode);
            oldProirity = mode;
            printf("Task ID=%d, proirity=%d\n", getpid(), mode);
        }

        if(running == true) {
            printf("Task ID=%d is running\n", getpid());
        }

        usleep(1000000);
    }
    close(p[0]);
}

bool haveThisId(int pid){
    bool state = false;
    int i = 0;
    for(i=0;i<indexPid;i++){
        if(pid == pids[i]){
            state = true;
            break;
        }
    }
    int j = i;
    for(j=i;j<indexPid-1;j++){
        pids[j] = pids[j+1];
    }

    return state;
}

void print(void){
    printf("Have created %d Task", indexPid);
    printf("There Id is [");
    int i = 0;
    for(i=0;i<indexPid;i++){
        printf("%d ",pids[i]);
    }
    printf("]\n");
}

void help(int mode){
    printf("\n请按照以下格式输入指令");
    printf("\ncreate(c)       :创建一个进程\n");
    printf("kill(k)         :杀死进程号为xxx的进程\n");
    printf("sleep(s)        :使进程进入休眠\n");
    printf("weakup(w)       :唤醒进程\n");
    printf("priority(p) xxx :设置进程的优先级为xxx\n");
    printf("quit(q)         :删除所有任务并退出程序\n\n");
}

int changeFileMode(int fd, int mode){
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }
    flags |= mode;
    if (fcntl(fd, F_SETFL, flags) < 0) {
        return -1;
    }

    return 0;
}

int main(int argc, char **argv){
    pid_t pid;

    pipe(p);
    int error = changeFileMode(p[0], O_NONBLOCK);
    int TaskNumFlag = 0;
    int number;

    help(1);
    while(1){
        char inputStr[1000] = {"\0"};
        scanf("%s",inputStr);

        if (((0 == strcmp(inputStr,"create"))||(0 == strcmp(inputStr,"c"))) && (TaskNumFlag == 0)) {
            if((pid = fork()) < 0){
                printf("Create Tack Error\n");
            }else if(0 == pid){
                process();
                exit(0);
            }else{
                pids[indexPid++] = pid;
                number = pid;
                usleep(1000);
                print();
                TaskNumFlag = 1;
            }
        }
        if((0 == strcmp(inputStr,"kill"))||(0 == strcmp(inputStr,"k"))){
            //scanf("%d",&number);
            //if(!haveThisId(number)){
            //    printf("Error: Please enter the correce PID!!\n");
            //    continue;
            //}
            kill(number,SIGKILL);
            sleep(1);// Wait kill complete
            indexPid--;
            print();
            int status;
            int id = waitpid(number, &status, WNOHANG);//check child status
            TaskNumFlag = 0;
        }
        if((0 == strcmp(inputStr,"sleep"))||(0 == strcmp(inputStr,"s"))){
            pipeWrite[0] = 's';
            pipeWrite[1] = '0';
            pipeWrite[2] = '\n';
            if(write(p[1], pipeWrite, strlen(pipeWrite)) != -1) {
                //printf("write sleep succes\n");
            }else{
                printf("errno=%d ",errno);
                char * mesg = strerror(errno);
                printf("Mesg:%s\n",mesg);
            }
            //close(p[1]);
        }
        if((0 == strcmp(inputStr,"weakup"))||(0 == strcmp(inputStr,"w"))){
            pipeWrite[0] = 'w';
            pipeWrite[1] = '1';
            pipeWrite[2] = '\n';
            if(write(p[1], pipeWrite, strlen(pipeWrite)) != -1){
                //printf("write weakup succes\n");
            }else{
                printf("errno=%d ",errno);
                char * mesg = strerror(errno);
                printf("Mesg:%s\n",mesg);
            }
            //close(p[1]);
        }
        if((0 == strcmp(inputStr,"priority"))||(0 == strcmp(inputStr,"p"))){
            int priority = 0;
            scanf("%d",&priority);
            pipeWrite[0] = 'p';
            sprintf(&pipeWrite[1], "%d", priority);
            pipeWrite[3] = '\n';
            if(write(p[1], pipeWrite, strlen(pipeWrite)) != -1){

            }else{
                printf("errno=%d ",errno);
                char * mesg = strerror(errno);
                printf("Mesg:%s\n",mesg);
            }
        }
        if((0 == strcmp(inputStr,"quit"))||(0 == strcmp(inputStr,"q"))){
            int i = 0;
            for(i=0;i<indexPid;i++){
                kill(pids[i],SIGKILL);
                int status;
                int id = waitpid(pids[i], &status, WNOHANG);//check child status
            }
            sleep(1);
            break;
        }
        if(0 == strcmp(inputStr,"print")){
            print();
        }

        usleep(10000);
        help(0);
    }

   close(p[1]);
   return 0;
}
