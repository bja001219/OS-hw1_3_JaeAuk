#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SMS 512

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("type in the shared memory");
		exit(1);
	}
	printf("Welcome to my remote shell server!\n");
	char text[SMS] = "";
	char *args[SMS];
	const char *shm_file = argv[1];
	int shm_fd;
	char *shm_ptr;

	shm_fd = shm_open(shm_file, O_CREAT | O_RDWR, 0666);
	if(shm_fd == -1){
		printf("error in shm_open");
		exit(1);
	}
	if(ftruncate(shm_fd, SMS) == -1){
		printf("error in ftruncate");
		exit(1);
	}
	shm_ptr = mmap(0, SMS, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(shm_ptr == MAP_FAILED){
		printf("error in mmap");
		exit(1);
	}
	strcpy(shm_ptr, "");
	while(1){
		while (shm_ptr[0] == '\0'){
			usleep(100000);
		}
		strcpy(text, shm_ptr);
		strcpy(shm_ptr, "");
		if(strcmp(text, "exit_svr") == 0){
			break;
		}
		printf("Waiting for a command ... \n");
		if(strlen(text)>0){
			int argc = 0;
			char *token = strtok(text, " ");
			while(token != NULL && argc < SMS -1){
				args[argc++] = token;
				token = strtok(NULL, " ");
			}
			args[argc] = NULL;
			printf("argc = %d\n", argc);
			if(strcmp(args[0], "cd") == 0){
				if(argc == 2){
					if(chdir(args[1]) != 0){
						perror("chdir");
					}
				}
				continue;
			}
			if(argc > 0) {
				pid_t pid = fork();
				if(pid == -1){
					perror("fork");
					exit(1);
				}
				if(pid == 0){
					if(execvp(args[0], args) == -1){
						perror("execvp");
						exit(1);
					}
				} else{
					int status;
					if(wait(&status) == -1){
						perror("wait");
						exit(1);
					}
				}
			}
		}
	}
		

	if(shm_unlink(shm_file) == -1){
		printf("error in shm_unlink");
		exit(1);
	}
	return 0;
}
