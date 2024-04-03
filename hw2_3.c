#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]){
	const char *src_file = argv[1];
	const char *dest_file = argv[2];
	if(argc != 3){
		printf("There are too less inputs\n");
		return -1;
	}
	struct stat src_stat;
	if(stat(src_file, &src_stat) != 0){
		printf("can't get attributes from <%s> \n", argv[1]);
		return -1;
	}
	mode_t mode = src_stat.st_mode;
	uid_t uid = src_stat.st_uid;
	gid_t gid = src_stat.st_gid;
	struct utimbuf times;
	times.actime = src_stat.st_atime;
	times.modtime = src_stat.st_mtime;

	if(chmod(dest_file, mode) != 0 || chown(dest_file, uid, gid) != 0 || utime(dest_file, &times) != 0){
		printf("Error in setting\n");
		return -1;
	}

	struct tm *modified_time = localtime(&src_stat.st_mtime);
	printf("attirbutes of file \"%s\"\n", src_file);
	printf("st_dev = %ld\n", src_stat.st_dev);
	printf("st_mode = %o\n", src_stat.st_mode & 0777);
	printf("st_uid = %d\n", src_stat.st_uid);
	printf("st_gid = %d\n", src_stat.st_gid);
	printf("st_size = %ld\n", src_stat.st_size);
	printf("st_mtime = %ld\n", src_stat.st_mtime);
	printf("modified time = %d/%d/%d %d:%d:%d\n",
		modified_time->tm_year + 1900,
		modified_time->tm_mon +1,
		modified_time->tm_mday,
		modified_time->tm_hour,
		modified_time->tm_min,
		modified_time->tm_sec);
	printf("File attributes were successfully copied\n");

	return 0;
}
