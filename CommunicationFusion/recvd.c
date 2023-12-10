#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "mylib_tap.h"
#include "mylib_serialport.h"
#include "mylib_slip.h"




int main() {

    //做一堆初始化，包括：1.串口2.tap网卡

    // 打开串口，拿到串口的fd
    int serial_port=serial_port_init("/dev/pts/2");
    int serial_frame_length=0;
    unsigned char serial_frame[BUFFER_SIZE];

    //打开tap，拿到tap的fd
    int tap_fd=tap_init("tap1");
    unsigned char tap_frame[1600]={0};


    //进入死循环，包括：1.从串口读数据 2.解包数据 3.写到网卡中
    while(1){
    serial_frame_length=read_serial_frame(serial_port,serial_frame);
    int decoded_data_length=slip_decode(&serial_frame[1],serial_frame_length-2);
    memcpy(tap_frame,&serial_frame[5],decoded_data_length-6);
    
    
    if(DEBUG_FLAG){
        printf("\ntap_frame_length:%d\n",decoded_data_length-6);
        printf("tap frame:\n");
        for(int i=0;i<decoded_data_length-6;i++){
            printf("%02x ",tap_frame[i]);
        }
        printf("\n");
    }


    write_tap_frame(tap_fd, tap_frame,decoded_data_length-6);
    printf("\n\n\n\n\n");

    }
        
    return 0;
}
