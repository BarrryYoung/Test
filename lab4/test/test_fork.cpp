#include <unistd.h>
#include <stdio.h>

int main()
{
        pid_t fpid;
        int count = 1 ;

        printf("父进程开始运行 \n");

        fpid = fork();

        printf("创建一个子进程 \n");

        if(fpid<0)
        {
                printf("创建进程失败! \n");
        }
        else if (fpid==0)
        {
                printf("子进程ID号为：%d \n",getpid());
                count ++;
        }
        else
        {
                printf("父进程ID号为：%d \n",getpid());
                count ++;
        }
        printf("进程数为: %d \n",count);
        return 0 ;
}