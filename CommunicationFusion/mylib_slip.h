#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* modifyArray(unsigned char *buffer, size_t length) {
    // 创建新数组，长度为原数组长度加上固定头部和长度信息的长度
    size_t newLength = length + 4; // 2 bytes for fixed header + 2 bytes for length
    unsigned char *newBuffer = (unsigned char *)malloc(newLength);

    // 添加固定开头 {0x55, 0x10}
    newBuffer[0] = 0x55;
    newBuffer[1] = 0x00;

    // 添加数组长度，两个字节
    newBuffer[2] = (unsigned char)(length >> 8); // 高字节
    newBuffer[3] = (unsigned char)length;        // 低字节

    // 使用memcpy复制原数组内容
    memcpy(newBuffer + 4, buffer, length);

    // 打印结果，这里可以根据需要进行其他操作
    printf("Modified Array: { ");
    for (size_t i = 0; i < newLength; ++i) {
        printf("0x%02X ", newBuffer[i]);
    }
    printf("}\n");

    // 释放动态分配的内存
    // free(newBuffer);
    return newBuffer;
}


void generateframe(char* serialframe,char* tapframe,unsigned int tapdata_length){
    serialframe[0]=0xc0;
    serialframe[tapdata_length+1+4+2]=0xc0;

    char *newframe=0;
    newframe=modifyArray(tapframe,tapdata_length);//获得加上头部的帧

    memcpy(&serialframe[1],newframe,tapdata_length+6);

    unsigned short crc=0;
    crc=getCRC(newframe,tapdata_length+4);
    printf("crc:%x\n",crc);
    serialframe[tapdata_length+1+4]=(unsigned char)(crc >> 8);
    serialframe[tapdata_length+1+4+1]=(unsigned char)crc;

    free(newframe);
}