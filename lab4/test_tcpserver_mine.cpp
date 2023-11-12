#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int serversocket=socket(AF_INET,SOCK_STREAM,0);
    int connectionsocket=0;

    struct sockaddr_in serveraddr={};
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8080);
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

    listen(serversocket,10);

    pid_t fpid=0;
    struct sockaddr_in clientaddr={};
    unsigned int clientaddr_len=sizeof(clientaddr);
    char buf[100]={};


    while(1){
        connectionsocket=accept(serversocket,(struct sockaddr *)&clientaddr,&clientaddr_len);
        fpid=fork();

        if(fpid<0)
        {
            printf("fork error");
            return 0;
        }
        else if (fpid==0)
        {
            //子进程start here
            printf("\n\nchild process here, my pid is %d\n",getpid());
            printf("a new connection,ip: %s, port:%d \n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port)); 
            //处理connectionsocket
            while(1)
            {
                if(recv(connectionsocket,buf,sizeof(buf),0)<=0)
                {
                    printf("connection closed\n");
                    break;
                }
                printf("message: %s\n",buf);
                //添加写到文件并尝试解决文件打开冲突的方案。
                memset(buf,0,sizeof(buf));
            }
            
            close(connectionsocket);
            return 0;
        }
        else 
        {
            //结束，继续等待连接。
            printf("\n\ni am the parent process, and i am keeping accepting\n");
        }
    }
}