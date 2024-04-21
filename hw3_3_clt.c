#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SMS 512

int main(int argc, char *argv[]){
	if(argc<2){
		printf("usage %s", argv[0]);
		exit(1);
	}
	
	printf("Welcome to my remote shell client!\n");
	char text[SMS] = "";	
	const char *shm_file = argv[1];
	int shm_fd;
	char *shm_ptr;
	shm_fd = shm_open(shm_file, O_RDWR| O_CREAT, 0666);
	if(shm_fd == -1){
		printf("error in shm_open");
		exit(1);
	}
	shm_ptr = mmap(NULL,SMS, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(shm_ptr == MAP_FAILED){
		printf("error in mmap");
		exit(1);
	}
	printf("Connected to server via shared memory.\n");
	while(1){
		printf("$ ");
		fgets(text, SMS, stdin);
		size_t len = strlen(text);
		if(len > 0 && text[len -1] == '\n'){
			text[len-1] = '\0';
		}
		if(len > 0){
			strcpy(shm_ptr, text);
			if(strcmp(text, "exit_svr") == 0){
				break;
			}	
		}
	printf("Goodbye!\n");
	return 0;
	}
}
