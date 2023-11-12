#include <stdio.h>
#include <string.h>
#include "test.h"





int main() {
    
    FILE* file_ptr;
    char LineBuffer[100];
    struct GPSinfo info;
    memset(&info,0,sizeof(info));
    

    file_ptr=fopen("data.txt","r");
    if(file_ptr==NULL){
      perror("error when opening:");
    }


   char GPGGA_Parser_Flag=0;
   char GPRMC_Parser_Flag=0;
    while (fgets(LineBuffer,sizeof(LineBuffer),file_ptr)!=NULL)
    {
      // printf("%s",LineBuffer);
      if(strncmp(LineBuffer,"$GPGGA",6)==0){
        if(Parser_GPGGA(LineBuffer,&info)!=-1){
          GPGGA_Parser_Flag=1;
        }
      }
      
      if(strncmp(LineBuffer,"$GPRMC",6)==0){
        if(Parser_GPRMC(LineBuffer,&info)!=-1){
          GPRMC_Parser_Flag=1;          
        }
      }
      
      if(GPRMC_Parser_Flag==1&&GPGGA_Parser_Flag==1){
        GPRMC_Parser_Flag=0;
        GPGGA_Parser_Flag=0;
        PrintInfo(&info);
      }

    }
    
}

