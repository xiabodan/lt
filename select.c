#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct {
    int connectfd;
    struct sockaddr_in client;
}targs;

void *process(void *arg){
    targs client;
    client.connectfd=((targs *)arg)->connectfd;
    client.client=((targs *)arg)->client;

    char buf[1024];

    int n=0;
    while(0<(n=read(client.connectfd,buf,sizeof(buf)))){
        buf[n]='\0';
        printf("You got a message <%s> from %s.\n",buf,inet_ntoa(client.client.sin_addr));

        int i=0;
        while(buf[i]!='\0'&&i<1024){
            if(buf[i]>='a'&&buf[i]<='z')
                buf[i]+=('A'-'a');
            i++;
        }

        write(client.connectfd,buf,strlen(buf));
    }
    close(client.connectfd);
}

#define PORT 1111
#define BACKLOG 5


int main(){
    int listenfd,connectfd;
    struct sockaddr_in server;
    struct sockaddr_in client;

    if(-1==(listenfd=socket(AF_INET,SOCK_STREAM,0))){
        perror("create socket error\n");
        exit(1);
    }

    int opt=SO_REUSEADDR;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    if(-1==(bind(listenfd,(struct sockaddr*)&server,sizeof(struct sockaddr)))){
        perror("bind error\n");
        exit(1);
    }

    if(-1==(listen(listenfd,BACKLOG))){
        perror("listen error\n");
        exit(1);
    }

    int len=sizeof(client);
    targs arg;

    if(-1==(connectfd=accept(listenfd,(struct sockaddr *)&client,&len))){
        perror("accept error\n");
        exit(1);
    }
    printf("connect to client = %d\n",connectfd);
    int ret = fcntl(connectfd, F_SETFL, O_NONBLOCK);
    if (ret < 0) {
        printf("Error setting O_NONBLOCK errno:%d", errno);
    }

    fd_set fds;  
    struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0  
    
    char buf[1024];
    int nums = 0;
    //struct timeval timeout;
    //timeout.tv_sec = 2;
    //timeout.tv_usec = 0;
    while(1){
        FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化  
        FD_SET(connectfd,&fds); //同上  
        
        int r = select(connectfd+1,&fds,NULL,NULL,&timeout);
        printf("select return num = %d, nums = %d\n",r,nums++);
        if(r>0){
            if(FD_ISSET(connectfd,&fds)){
                int n = 0;
                while(0<(n=read(connectfd,buf,sizeof(buf)))){
                    puts(buf);
                    //buf[n]='\0';
                    //write(connectfd,buf,strlen(buf));
                }
            }
        }
        sleep(3);
    }
    close(listenfd);
    return 0;
}
