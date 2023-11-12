#include<stdio.h>
#include"test.h"

int main(){
   struct GPSinfo info;
   char gprmc[]="$GPRMC,213035.000,A,3447.5084,N,11339.3580,E,0.22,341.45,141111,,*0D";
   Parser_GPRMC(gprmc,&info);
   printf("%d",11939/100);
}