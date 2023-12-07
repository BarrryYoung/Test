#include <linux/if.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "tunclient.h"
#include <unistd.h>
#include <stdlib.h>
// #include <net/if.h>


int main(){
    char dev_name[IFNAMSIZ]="tap0";
    int tun_fd=0;

    tun_fd=tun_alloc(dev_name,IFF_TAP);
    if(tun_fd < 0){
      perror("Allocating error");
      exit(1);
    }


    char buffer[6000]={0};
    int nread=0;
    while (1)
    {
      memset(buffer,0,sizeof(buffer));
      nread=read(tun_fd,buffer,sizeof(buffer));
      if(nread < 0){
        perror("reading from interface");
        close(tun_fd);
        exit(1);
      }

      printf("read %d bytes data:\n",nread);
      // printf("%0*x\n",nread,buffer);
      for (int i = 0; i < nread; i++) {
        printf("%02x ", buffer[i]);
      }
      printf("\n");
    }


    

}