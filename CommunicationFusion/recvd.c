#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "mylib_tap.h"
#include "mylib_serialport.h"
#include "mylib_slip.h"




typedef struct {
    int serial_port;
    int tap_fd;
} thread_args;


void* sendd(void *args){
    thread_args *actual_args = (thread_args *)args;
    int serial_port = actual_args->serial_port;
    int tap_fd = actual_args->tap_fd;

    unsigned char tapframe[1600]={0};
    unsigned int tapframe_length=0;
    

    unsigned char serialframe[1600]={0};

while(1){


    tapframe_length=get_tap_frame(tap_fd,tapframe);


    //组装串口帧，也就是加头加尾
    //serialframe就是最终获得的帧，长度就是tapframe_length+1+2+2+2+1=tapframe_length+8

    int encoded_data_length=0;
    encoded_data_length=generateframe(serialframe,tapframe,tapframe_length);

    printf("frame generated:\n");
    for(int i=0;i<encoded_data_length;i++){
        printf("%02x ",serialframe[i]);
    }
    printf("\n");

    write(serial_port, serialframe, encoded_data_length);
    printf("\n\n\n\n");
}

    close(serial_port);

}

void* recvd(void* args){
    thread_args *actual_args = (thread_args *)args;
    int serial_port = actual_args->serial_port;
    int tap_fd = actual_args->tap_fd;

    int serial_frame_length=0;
    unsigned char serial_frame[BUFFER_SIZE];

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

}

int main() {
    //初始化
    int serial_port=serial_port_init("/dev/pts/2");//serial_port初始化，返回fd
    int tap_fd=tap_init("tap1");//tap初始化,返回file descriptor


    //多线程
    pthread_t thread_recv, thread_send;

    thread_args args;
    args.serial_port = serial_port;
    args.tap_fd = tap_fd;

    if (pthread_create(&thread_recv, NULL, recvd, (void *)&args)) {
        fprintf(stderr, "Error creating recv thread\n");
        return 1;
    }

    // 创建发送数据的线程
    if (pthread_create(&thread_send, NULL, sendd, (void *)&args)) {
        fprintf(stderr, "Error creating send thread\n");
        return 1;
    }

    // 等待线程完成
    pthread_join(thread_recv, NULL);
    pthread_join(thread_send, NULL);
    
    return 0;

}
