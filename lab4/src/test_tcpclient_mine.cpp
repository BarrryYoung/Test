#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/*
1.地址
2.循环：读文件，建立连接，发送，关闭连接，结束。
*/


int main(){
    int clientsocket=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8080);
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");


    int ret = connect(clientsocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(ret<0)printf("connetc error\n");

    send(clientsocket,"holy shit",sizeof("holy shit"),0);

    close(clientsocket);

}