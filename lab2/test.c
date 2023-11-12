#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int ReadButtonStatus(int* button_fds,char* ButtonStatus);
int Ledmode(int* led_fds, int num, int LoopCount);



int main(int argc, char** argv)
{
    char ButtonStatus[2]={0};//当前status
    char ButtonStatusBackup[2]={0};
    int LoopCount=0;
    //printf("count:%d\n",LoopCount);




    //open buttons
    int button_fds[2]={0};
	button_fds[0] = open("/dev/100ask_button0", O_RDWR);
    button_fds[1] = open("/dev/100ask_button1", O_RDWR);
    //printf("button0:%d,button1:%d",button_fds[0],button_fds[1]);
	if (button_fds[0] == -1 || button_fds[1]==-1)
	{
		printf("cannot open buttons. button0_fd:%d, button1_fd:%d\n",button_fds[0],button_fds[1]);
        return -1;
    }


    //open leds
    int led_fds[4];
    led_fds[0]=open("/dev/100ask_led0",O_RDWR);
    led_fds[1]=open("/dev/100ask_led1",O_RDWR);
    led_fds[2]=open("/dev/100ask_led2",O_RDWR);
    led_fds[3]=open("/dev/100ask_led3",O_RDWR);
    if (led_fds[0]==-1 || led_fds[1]==-1 || led_fds[2]==-1 || led_fds[3]==-1)
    {
        printf("led open error, %d%d%d%d\n",led_fds[0],led_fds[1],led_fds[2],led_fds[3]);
        return -1;
    }



   while(1)
    {
        usleep(25000);
        //printf("count:%d\n",LoopCount);
        //读取button状态，若改动则 1.count归0 2.备份改动后的值
        ReadButtonStatus(button_fds,ButtonStatus);
        // printf("buttonStatus:           buttonstatus[0]:%d,buttonstatus[1]:%d\n",ButtonStatus[0],ButtonStatus[1]);
        // printf("butttonstatus backup:   backup[0]:%d,backup[1]:%d\n",ButtonStatusBackup[0],ButtonStatusBackup[1]);
        // printf("compare result:%d",strcmp(ButtonStatus,ButtonStatusBackup));
        if(strcmp(ButtonStatus,ButtonStatusBackup))
        {
            LoopCount=0;
            ButtonStatusBackup[0]=ButtonStatus[0];
            ButtonStatusBackup[1]=ButtonStatus[1];
        }
        else
        {
            LoopCount++;
            if(LoopCount>200)LoopCount=0;
        }
         

        //判断按键状态
        if(ButtonStatus[1]==0 && ButtonStatus[0]==0)Ledmode(led_fds,0,LoopCount);
        else if(ButtonStatus[1]==0 && ButtonStatus[0]==1)Ledmode(led_fds,1,LoopCount);
        else if(ButtonStatus[1]==1 && ButtonStatus[0]==0)Ledmode(led_fds,2,LoopCount);
        else if(ButtonStatus[1]==1 && ButtonStatus[0]==1)Ledmode(led_fds,3,LoopCount);
        else printf("ButtonStatus error, amazing!\n");

    }
    



    close(button_fds[0]);
    close(button_fds[1]);
    close(led_fds[0]);
    close(led_fds[1]);
    close(led_fds[2]);
    close(led_fds[3]);
}











int ReadButtonStatus(int* button_fds,char* ButtonStatus)
{
    //write buttons
    read(button_fds[0],&ButtonStatus[0],1);
    read(button_fds[1],&ButtonStatus[1],1);
}



int Ledmode(int* led_fds, int num, int LoopCount)
{
    char on=1,off=0;

    switch (num)
    {
    case 0:
        write(led_fds[0],&on,1);
        write(led_fds[1],&on,1);
        write(led_fds[2],&on,1);
        write(led_fds[3],&on,1);
        break;
    case 1:
        if (LoopCount<=100)
        {
            write(led_fds[0],&on,1);
            write(led_fds[1],&off,1);
            write(led_fds[2],&on,1);
            write(led_fds[3],&off,1);
        }
        else if (LoopCount<=200)
        {
            write(led_fds[0],&off,1);
            write(led_fds[1],&on,1);
            write(led_fds[2],&off,1);
            write(led_fds[3],&on,1);
        }
        break;
    case 2:
        if (LoopCount<=50)
        {
            write(led_fds[0],&on,1);
            write(led_fds[1],&off,1);
            write(led_fds[2],&off,1);
            write(led_fds[3],&off,1);
        }
        else if(LoopCount<=100)
        {
            write(led_fds[0],&off,1);
            write(led_fds[1],&on,1);
            write(led_fds[2],&off,1);
            write(led_fds[3],&off,1);
        }
        else if(LoopCount<=150)
        {
            write(led_fds[0],&off,1);
            write(led_fds[1],&off,1);
            write(led_fds[2],&on,1);
            write(led_fds[3],&off,1);
        }
        else if(LoopCount<=200)
        {
            write(led_fds[0],&off,1);
            write(led_fds[1],&off,1);
            write(led_fds[2],&off,1);
            write(led_fds[3],&on,1);
        }
        break;
    case 3:
        write(led_fds[0],&off,1);
        write(led_fds[1],&off,1);
        write(led_fds[2],&off,1);
        write(led_fds[3],&off,1);
        break;
    default:printf("ledmode error\n");
        break;
    }
}