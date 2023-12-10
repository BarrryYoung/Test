#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mylib_serialport.h"
#include "mylib_slip.h"
#include "mylib_tap.h"

int main() {

    int serial_port=serial_port_init("/dev/pts/1");


    unsigned char tapframe[1600]={0};
    unsigned int tapframe_length=0;
    

    //tap初始化,返回file descriptor
    int tap_fd=tap_init("tap0");
    unsigned char serialframe[1600]={0};

while(1){


    tapframe_length=get_tap_frame(tap_fd,tapframe);


    //组装串口帧，也就是加头加尾
    //serialframe就是最终获得的帧，长度就是tapframe_length+1+2+2+2+1=tapframe_length+8
    
    generateframe(serialframe,tapframe,tapframe_length);

    printf("frame generated:\n");
    for(int i=0;i<tapframe_length+8;i++){
        printf("%02x ",serialframe[i]);
    }
    printf("\n");

    write(serial_port, serialframe, tapframe_length+8);
    printf("\n\n\n\n");
}

    close(serial_port);

    return 0;

}


// \xc0\x55\x00\x00\x01\x00\x9e\x1b\xc0

// echo -e "\xc0\x55\x00\x00\x01\x00\x9e\x1b\xc0" >/dev/ttyS5
// echo  -e "\xc0\x55\x00\x00\x08\x01\x00\x0e\x31\xb9\x56\x34\x2a\x6a\x98\xc0\r\n" >/dev/ttyS5
























// #include <unistd.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <termios.h>

// #include <linux/if.h>
// #include <linux/if_tun.h>
// #include <sys/ioctl.h>
// #include "tunclient.h"

// #define BUFFER_SIZE 1024




// int serial_port_init(){
//     // 打开串口
//     int serial_port = open("/dev/ttyUSB0", O_RDWR);

//     // 检查串口是否打开成功
//     if (serial_port < 0) {
//         printf("Error %i from open: %s\n", serial_port, strerror(errno));
//         return 1;
//     }

//     // 配置串口参数（如果尚未配置）
//     struct termios tty;

//     memset(&tty, 0, sizeof(tty));
//     if (tcgetattr(serial_port, &tty) != 0) {
//         printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
//     } else {

//         tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
//         tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
//         tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
//         tty.c_cflag |= CS8; // 8 bits per byte (most common)
//         tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
//         tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

//         tty.c_lflag &= ~ICANON;
//         tty.c_lflag &= ~ECHO; // Disable echo
//         tty.c_lflag &= ~ECHOE; // Disable erasure
//         tty.c_lflag &= ~ECHONL; // Disable new-line echo
//         tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
//         tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
//         tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

//         tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
//         tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
//         // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
//         // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

//         tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
//         tty.c_cc[VMIN] = 0;

//         // Set in/out baud rate to be 9600
//         cfsetispeed(&tty, B115200);
//         cfsetospeed(&tty, B115200);
        
//         if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
//             printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
//         }
//     }

//     return serial_port;
// }


// int tap_init(){
//     char dev_name[IFNAMSIZ]="tap0";
//     int tap_fd=0;

//     tap_fd=tun_alloc(dev_name,IFF_TAP);
//     if(tap_fd < 0){
//       perror("Allocating error");
//       exit(1);
//     }
  
//     return tap_fd;
// }


// int get_tap_frame(int tap_fd,char* tap_frame){
//     static buffer[2000];
//     memset(buffer,0,sizeof(buffer));
//     int nread=0;

//     nread=read(tap_fd,buffer,sizeof(buffer));
//     if(nread < 0){
//       perror("reading from interface");
//       close(tap_fd);
//       exit(1);
//     }

//     printf("read %d bytes data:\n",nread);
//     // printf("%0*x\n",nread,buffer);
//     for (int i = 0; i < nread; i++) {
//       printf("%02x ", buffer[i]);
//     }
//     printf("\n");

//     memcpy(tap_frame,buffer,nread);
//     return nread;

// }