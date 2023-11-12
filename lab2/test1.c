#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char ButtonStatus[2]={0,0};//当前status
    char ButtonStatusBackup[2]={1,1};
    int res=strcmp(ButtonStatus,ButtonStatusBackup);
    printf("%d",res);
}