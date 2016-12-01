#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 1111
#define MAXDATASIZE 1024


int main(int argc,char **argv){
    int sockfd;
    //char* buf = (char*)malloc(sizeof(char) * MAXDATASIZE);
    //char bu[10] = "hubert";
    //buf = bu;
    char buf[MAXDATASIZE]="hubert";
    struct hostent *he;
    struct sockaddr_in server;
    if(2>argc){
        fprintf(stderr,"Usage: %s \n",argv[0]);
        exit(1);
    }

    if(NULL==(he=gethostbyname(argv[1]))){
        perror("gethostbyname error\n");
        exit(1);
    }

    if(-1==(sockfd=socket(AF_INET,SOCK_STREAM,0))){
        perror("socket error\n");
        exit(1);
    }

    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr = *((struct in_addr*)he->h_addr);
    if(-1==connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr))){
        perror("connect error\n");
        exit(1);
    }
    int nums = 0;
    while(1){
        //gets(buf);
        buf[strlen(buf)]='\0';
        write(sockfd,buf,strlen(buf)+1);
        printf("send %s to server,nums = %d\n",buf,nums++);
        //int num=read(sockfd,buf,sizeof(buf));
        sleep(1);
        puts(buf);
    }
    close(sockfd);
    return 0;
}
