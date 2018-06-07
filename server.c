#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

void processrequest(int client_sock,struct sockaddr_in *addr)
{
    char buf[1024] = {0};
    while(1)
    {
       ssize_t s = read(client_sock,buf,sizeof(buf)-1); 
       if(s < 0)
       {
           printf("read error\n");
           continue;
       }
       if(s == 0)
       {
           printf("client say bye!\n");
           close(client_sock);
           break;
       }
       buf[s] = '\0';
       printf("client #:%s\n",buf);
       write(client_sock,buf,strlen(buf));
    }
    return;
}
typedef struct Arg{
    int fd;
    struct sockaddr_in addr;
}Arg;
void *createworker(void *ptr)
{
    Arg *arg = (Arg *)ptr;
    processrequest(arg->fd,&arg->addr);
    free(arg);
    return NULL;
}
int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        printf("./client IP PORT\n");
        return 1;
    }
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        printf("socket error\n");
        return 2;
    }
    struct sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(atoi(argv[2]));
    server_socket.sin_addr.s_addr = inet_addr(argv[1]);
    if(bind(sock,(struct sockaddr*)&server_socket,sizeof(server_socket)) < 0)
    {
        printf("bind error\n");
        return 3;
    }
    if(listen(sock,5) < 0)
    {
        printf("listen error\n");
        return 4;
    }
    printf("bind and listen is success,please wait accept...\n");
    while(1)
    {
        struct sockaddr_in client_socket;
        socklen_t len = sizeof(client_socket);
        int client_sock = accept(sock,(struct sockaddr *)&client_socket,&len);
        if(client_sock < 0)
        {
            printf("accept error\n");
            continue;
        }
        pthread_t tid = 0;
        Arg *arg = (Arg *)malloc(sizeof(Arg));
        arg->fd = client_sock;
        arg->addr = client_socket;
        pthread_create(&tid,NULL,createworker,(void *)arg);//
        pthread_detach(tid);
    
    }
    close(sock);
    return 0;
}

