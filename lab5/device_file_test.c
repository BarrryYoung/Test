#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 256

int main() {
    const char *device_path = "/dev/mydev";  // 请替换为你要打开的设备文件路径
    int file_descriptor = open(device_path, O_RDONLY);

    if (file_descriptor == -1) {
        perror("无法打开设备文件");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead;

    // 读取一行数据
    bytesRead = read(file_descriptor, buffer, sizeof(buffer));

    if (bytesRead == -1) {
        perror("读取失败");
        close(file_descriptor);
        exit(EXIT_FAILURE);
    }

    // 在读取的数据后添加 null 终止字符，以便可以打印字符串
    buffer[bytesRead] = '\0';

    printf("\n\n读取的行数据：\n%s", buffer);

    bytesRead = read(file_descriptor, buffer, sizeof(buffer));

    if (bytesRead == -1) {
        perror("读取失败");
        close(file_descriptor);
        exit(EXIT_FAILURE);
    }

    // 在读取的数据后添加 null 终止字符，以便可以打印字符串
    buffer[bytesRead] = '\0';

    printf("\n\n读取的行数据：\n%s", buffer);

    close(file_descriptor);

    return 0;
}
