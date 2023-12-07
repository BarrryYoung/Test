#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

#define BUFFER_SIZE 1024

int main() {
    // 打开串口
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





  // Write to serial port
  unsigned char msg[] = {0xc0,0x55,0x00,0x00,0x01,0x00,0x9e,0x1b,0xc0};
  write(serial_port, msg, sizeof(msg));

  

//     // 读取数据
//     unsigned char read_buf[BUFFER_SIZE];
//     memset(&read_buf, '\0', sizeof(read_buf));

    
//     for(int a=0; a<100; a++){
// 	printf("read:");
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

    // 关闭串口
    close(serial_port);

    return 0;
}
