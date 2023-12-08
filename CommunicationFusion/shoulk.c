#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include "tunclient.h"

#define BUFFER_SIZE 1600
#define END 0xc0


int serial_port_init(){
    int serial_port = open("/dev/ttyS5", O_RDWR);

    // 检查串口是否打开成功
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", serial_port, strerror(errno));
        return 1;
    }

    // 配置串口参数（如果尚未配置）
    struct termios tty;

    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    } else {
        // cfsetospeed(&tty, B9600); // 设置波特率为9600
        // cfsetispeed(&tty, B9600);

        // tty.c_cflag &= ~PARENB; // 关闭奇偶校验
        // tty.c_cflag &= ~CSTOPB; // 使用1个停止位
        // tty.c_cflag &= ~CSIZE; // 清除当前数据位数设置
        // tty.c_cflag |= CS8; // 使用8位数据位

        // tty.c_cflag &= ~CRTSCTS; // 关闭硬件流控制
        // tty.c_cflag |= CREAD | CLOCAL; // 打开读取接收器并忽略调制解调器状态线

        // tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL); // 非规范模式，关闭回显
        // tty.c_lflag &= ~ISIG; // 关闭信号字符

        // tty.c_iflag &= ~(IXON | IXOFF | IXANY); // 关闭软件流控制
        // tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

        // tty.c_oflag &= ~OPOST; // 关闭所有特殊输出处理
        tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
        tty.c_cflag |= CS8; // 8 bits per byte (most common)
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO; // Disable echo
        tty.c_lflag &= ~ECHOE; // Disable erasure
        tty.c_lflag &= ~ECHONL; // Disable new-line echo
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

        tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        tty.c_cc[VMIN] = 0;

        // Set in/out baud rate to be 9600
        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);

        if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
    }
    return serial_port;

}


int read_serial_frame(int serial_port,char* serial_frame){
    unsigned char read_buf[BUFFER_SIZE];
    unsigned char frame_buf[BUFFER_SIZE];
    int frame_index = 0; // SLIP帧缓冲区中的索引
    int num_bytes, i;
    int receiving_frame = 0; // 是否正在接收帧的标志

    for(int a = 0; a < 100; a++) {
        printf("read:\n");
        num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

        if (num_bytes < 0) {
            printf("Error reading: %s\n", strerror(errno));
        } else if (num_bytes == 0) {
            continue;
        } else {
            for(i = 0; i < num_bytes; i++) {
                unsigned char byte = read_buf[i];

                if (byte == END) {
                    if (receiving_frame) {
                        // 发现帧结尾，处理已经接收的完整帧
                        receiving_frame = 0;
                        frame_buf[frame_index] = END; // 添加帧结尾
                        frame_index++; // 增加索引以准备下次接收

                        // 处理 SLIP 帧 - 这里你可以将 frame_buf 传递给其他函数进行处理
                        printf("SLIP frame received: ");
                        for (int j = 0; j < frame_index; j++) {
                            printf("%02x ", frame_buf[j]);
                        }
                        printf("\n");

                        memcpy(serial_frame,frame_buf,frame_index);
                        frame_index = 0; // 重置索引为下一个 SLIP 帧做准备
                        return frame_index;
                    } else {
                        // 发现帧开始
                        receiving_frame = 1;
                        frame_buf[frame_index] = END; // 添加帧开始
                        frame_index = 1; // 设置索引到帧缓冲区的下一个位置
                    }
                } else if (receiving_frame) {
                    // 如果我们处于接收帧的状态，将数据添加到 frame_buf
                    frame_buf[frame_index] = byte;
                    frame_index++;
                    if (frame_index >= BUFFER_SIZE) {
                        // 缓冲区溢出，这通常表明出错，重置状态等待下一个帧开始
                        receiving_frame = 0;
                        frame_index = 0;
                        printf("Error: Frame buffer overflow\n");
                    }
                }
                // 如果不是在接收帧，忽略所有非 END 字节
            }
        }
    }

    close(serial_port); // 关闭串口

    return 0;
}






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
//     static unsigned char buffer[2000];
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

void write_tap_frame(char* tap_frame,int tap_frame_length){
    int tap_fd=tap_init();
    int nwrite=write(tap_fd,tap_frame,tap_frame_length);
    if(nwrite < 0){
      perror("writing from interface");
      close(tap_fd);
      exit(1);    
    }
    printf("read %d bytes to tap", nwrite);

}

int main() {
    // 打开串口
    int serial_port=serial_port_init();

    int serial_frame_length=0;
    unsigned char serial_frame[BUFFER_SIZE];
    serial_frame_length=read_serial_frame(serial_port,serial_frame);
    // 读取数据

    for(int i=0;i<serial_frame_length;i++){
        printf(" %02x",serial_frame[i]);
    }
    unsigned char tap_frame[1600]={0};
    memcpy(tap_frame,&serial_frame[5],serial_frame_length-8);

    printf("tap frame:\n");
    printf("0:%02x\n",tap_frame[0]);
    printf("1:%02x\n",tap_frame[1]);
    printf("2:%02x\n",tap_frame[2]);
    printf("serial_frame_length:%d\n",serial_frame_length-8);
    for(int i=0;i<serial_frame_length-8;i++){
        printf(" %02x\n",tap_frame[i]);
    }

    write_tap_frame(tap_frame,serial_frame_length-8);    
    return 0;
//     memset(&read_buf, '\0', sizeof(read_buf));

    
//     for(int a=0; a<100; a++){
// 	printf("\nread:");
//     int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

//     // 检查读取的字节数
//     if (num_bytes < 0) {
//         printf("Error reading: %s\n", strerror(errno));
//     } 
//     else if(num_bytes==0)continue;
//     else
//     {
//         //printf("Read %i bytes. Received message: ", num_bytes);
//         for(int i = 0; i < num_bytes; i++)
//             printf("%02x ", read_buf[i]);
//         printf("\n");
//     }
// }
//     // 关闭串口
//     close(serial_port);

//     return 0;
}
