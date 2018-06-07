#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        printf("usage is ./client IP PORT\n");
        return 1;
    }
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        printf("socket errno\n");
        return 2;
    }
    struct sockaddr_in server_sock;
    bzero(&server_sock,sizeof(server_sock));
    server_sock.sin_family = AF_INET;
    server_sock.sin_port = htons(atoi(argv[2]));
    server_sock.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = connect(sock,(struct sockaddr*)&server_sock,sizeof(server_sock));
    if(ret < 0)
    {
        printf("connect is error\n");
        return 3;
    }
    printf("connect is success\n");
    while(1)
    {
        char buf[1024] = {0};
        printf("client #:");
        fgets(buf,sizeof(buf),stdin);
        buf[sizeof(buf)] = '\0';
        write(sock,buf,strlen(buf));
        printf("please wait...\n");
        read(sock,buf,sizeof(buf));
        printf("server $:%s\n",buf);
    }
    close(sock);
    return 0;

}
