#include<stdio.h>
#include<unistd.h>

int main(){
    pid_t fd_pid=0;

    fd_pid=fork();
    if(fd_pid<0){
        printf("ford error");
        return 0;
    }


    if(fd_pid==0)printf("the new process id is %d",getpid());
    else printf("this is parent process, and my pid is %d",getpid());


    return 0;
}