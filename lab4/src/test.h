#include <stdio.h>
#include <string.h>

struct GPSinfo{
   int year,month,day;
   char hh,mm,ss;
   char n_or_s;
   int longtitude,longtitude_hh,longtitude_ss;
   char w_or_e;
   int latitude,latitude_hh,latitude_ss;
   char direction[10];
   char speed[10];
   char altitude[10];
};

int Parser_GPGGA(char * LineBuffer,struct GPSinfo* info){
    char altitude[10]={0};
    int result = sscanf(LineBuffer, "$GPGGA,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%10[^,]", altitude);
    if (result!=1){
        printf("sscanf error");
        return -1;
    }
    strncpy(info->altitude,altitude,strlen(altitude));
    // printf("%s",info->altitude);
}

//$GPRMC,213035.000,A,3447.5084,N,11339.3580,E,0.22,341.45,141111,,*0D
int Parser_GPRMC(char * LineBuffer,struct GPSinfo* info){
    int year,month,day;
    int hh,mm,ss;
    char n_or_s;
    int longtitude_and_longtitude_hh,longtitude,longtitude_hh,longtitude_ss;
    char w_or_e;
    int latitude_and_latitude_hh,latitude,latitude_hh,latitude_ss;
    char speed[10]={0};
    char direction[10]={0};


    int result=sscanf(LineBuffer,"$GPRMC,%2d%2d%2d.%*[^,],%*[^,],%5d.%4d,%c,%5d.%4d,%c,%9[^,],%9[^,],%2d%2d%2d,%*[^,],%*[^,]",\
    &hh,&mm,&ss,&longtitude_and_longtitude_hh,&longtitude_ss,&n_or_s,&latitude_and_latitude_hh,&latitude_ss,&w_or_e,\
    speed,direction,&year,&month,&day);

    if(result!=14){
        printf("gprmc parser error");
        return -1;
    }
    

    info->year=year;
    info->month=month;
    info->day=day;
    info->hh=hh;
    info->mm=mm;
    info->ss=ss;
    info->latitude=latitude_and_latitude_hh/100;
    info->latitude_hh=latitude_and_latitude_hh%100;
    info->latitude_ss=latitude_ss;
    info->w_or_e=w_or_e;
    info->longtitude=longtitude_and_longtitude_hh/100;
    info->longtitude_hh=longtitude_and_longtitude_hh%100;
    info->longtitude_ss=longtitude_ss;
    strcpy(info->speed,speed);
    strcpy(info->direction,direction);
    

    return 0;

}


void PrintInfo(struct GPSinfo* info){
    printf("时间：%d年%02d月%02d日，%02d时%02d分%02d秒\n",info->year+2000,info->month,info->day,info->hh,info->mm,info->ss);
    
    printf("位置：");
    if (info->w_or_e=='W')printf("西经");
    else printf("东经");
    printf("%d度%d分%d秒，",info->latitude,info->latitude_hh,info->latitude_ss/100);
    if (info->n_or_s=='N')printf("北纬");
    else printf("南纬");
    printf("%d度%d分%d秒\n",info->longtitude,info->longtitude_hh,info->longtitude_ss/100);

    //info->directoin的格式是：222.22
    char direction_degree[5]={0};
    char direction_minute[5]={0};
    sscanf(info->direction,"%5[^.].%5[^.]",direction_degree,direction_minute);
    printf("方向：北%s度%s分\n",direction_degree,direction_minute);

    printf("航速：%.1f公里/小时\n",float(*(info->speed))*1.852);
    printf("高度：%s米\n\n\n",info->altitude);
}


void GetInfo(){
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
      }
    }


}