#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>
#include  "sdstore.h"
#define MAX_BUFF 1024


void write_on_config_file(config_file config[], int N){
	int fd = open("config_file.txt", O_CREAT | O_WRONLY, 0666);
	char buffer[MAX_BUFF];
	int size;

	for(int i=0; i<N; i+=1){
		size = snprintf(buffer, MAX_BUFF, "%s %d\n", config[i].key, config[i].value);
		write(fd, buffer, size);
	}

	close(fd);
}

void fill_the_array(config_file config[]){
	config[0].key = "nop";
	config[0].value = 3;

	config[1].key = "bcompress";
	config[1].value = 4;

	config[2].key = "bdecompress";
	config[2].value = 4;

	config[3].key = "gcompress";
	config[3].value = 2;

	config[4].key = "gdecompress";
	config[4].value = 2;

	config[5].key = "encrypt";
	config[5].value = 2;

	config[6].key = "decrypt";
	config[6].value = 2;	

}



int main(int argc, char *argv[]){
	char buffer[128];
	config_file config[7];
	
	// fill the array with data
	fill_the_array(config);

    // write on the config file the server exec functions
    write_on_config_file(config, 7);
    
	



	return 0;
}