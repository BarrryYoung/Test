#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void){
	char buffer[64];
	int count;
	int fd;

	printf("this is a test file.\n\n");

	fd = open("/dev/mydev", 0);
	if(fd < 0){
		printf("error open device file.\n");
		exit(1);
	}
	count = read(fd, buffer, 5);
	buffer[count] = '\0';
	printf("1: %s\n", buffer);
	count = read(fd, buffer, 5);
	buffer[count] = '\0';
	printf("2: %s\n\n\n\n", buffer);
	close(fd);
	return 0;
}

