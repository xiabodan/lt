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

#define MAX_TASK_NUM 4

pid_t pids[MAX_TASK_NUM];//task id
int indexPid = 0;  //task number index

int p[2];          //pipe
char pipeWrite[20];//write pipe buffer
char pipeRead[20]; //read  pipe buffer

void process(void){
    bool running = false;
    int oldProirity = 0;
    printf("Create Task Success, PID=%d\n", getpid());
    for(;;){
        //usleep(1);
    }
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

void help(int mode){
    printf("\n请按照以下格式输入指令");
    printf("\ncreate(c)       :创建一个进程\n");
    printf("kill(k)         :杀死进程号为xxx的进程\n");
    printf("sleep(s)        :使进程进入休眠\n");
    printf("weakup(w)       :唤醒进程\n");
    printf("priority(p) xxx :设置进程的优先级为xxx\n");
    printf("quit(q)         :删除所有任务并退出程序\n\n");
}

int main(int argc, char **argv){
    int taskNum = MAX_TASK_NUM;
    pid_t pid;

    while(taskNum--){
        if((pid = fork()) < 0){
            printf("Create Tack Error\n");
        }else if(0 == pid){
            process();
            //exit(0);
        }else{
            pids[indexPid++] = pid;
            usleep(1000);
        }
        sleep(5);
    }
    while(1){
        sleep(1);
    }

   return 0;
}
