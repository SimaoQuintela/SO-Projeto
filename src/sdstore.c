#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>
#define MAX_BUFF 1024

// I need to optimize this
void write_on_config_file(int argc, char *argv[]){
	int fd = open("config_file.txt", O_CREAT | O_WRONLY, 0666);
	char buffer[MAX_BUFF];
	int size;

	for(int i=0; i<argc; i+=1){
		if(strcmp(argv[i], "nop") == 0){
			size = snprintf(buffer, MAX_BUFF, "%s 3\n", argv[i]);
			write(fd, buffer, size);

		} else if (strcmp(argv[i], "bcompress") == 0) {
			size = snprintf(buffer, MAX_BUFF, "%s 4\n", argv[i]);
			write(fd, buffer, size);

		} else if(strcmp(argv[i], "bdecompress") == 0) {
			size = snprintf(buffer, MAX_BUFF, "%s 4\n", argv[i]);
			write(fd, buffer, size);

		} else if(strcmp(argv[i], "gcompress") == 0) {
			size = snprintf(buffer, MAX_BUFF, "%s 2\n", argv[i]);
			write(fd, buffer, size);

		} else if(strcmp(argv[i], "gdecompress") == 0) {
			size = snprintf(buffer, MAX_BUFF, "%s 2\n", argv[i]);
			write(fd, buffer, size);

		} else if(strcmp(argv[i], "bdecompress") == 0) {
			size = snprintf(buffer, MAX_BUFF, "%s 2\n", argv[i]);
			write(fd, buffer, size);

		} else if(strcmp(argv[i], "encrypt") == 0) {
			size = snprintf(buffer, MAX_BUFF, "%s 2\n", argv[i]);
			write(fd, buffer, size);
		} else if(strcmp(argv[i], "decrypt") == 0) {
			size = snprintf(buffer, MAX_BUFF, "%s 2\n", argv[i]);
			write(fd, buffer, size);
		}
	}

	close(fd);
}




int main(int argc, char *argv[]){
	char buffer[128];
	
    // write on the config file the server exec functions
    write_on_config_file(argc, argv);
    
	





	return 0;
}