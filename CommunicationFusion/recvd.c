#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "mylib_tap.h"
#include "mylib_serialport.h"




int main() {

    //做一堆初始化，包括：1.串口2.tap网卡

    // 打开串口，拿到串口的fd
    int serial_port=serial_port_init();
    int serial_frame_length=0;
    unsigned char serial_frame[BUFFER_SIZE];

    //打开tap，拿到tap的fd
    int tap_fd=tap_init();
    unsigned char tap_frame[1600]={0};


    //进入死循环，包括：1.从串口读数据 2.解包数据 3.写到网卡中
    while(1){
    serial_frame_length=read_serial_frame(serial_port,serial_frame);
    memcpy(tap_frame,&serial_frame[5],serial_frame_length-8);
    
    
    if(DEBUG_FLAG){
        printf("\n\ntap_frame_length:%d\n",serial_frame_length-8);
        printf("tap frame:\n");
        for(int i=0;i<serial_frame_length-8;i++){
            printf(" %02x",tap_frame[i]);
        }
    }


    write_tap_frame(tap_fd, tap_frame,serial_frame_length-8);

    }
        
    return 0;
}