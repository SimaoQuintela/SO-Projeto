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

void add_to_struct(config_file config[], int i,char* key, int value){
	config[i].key = key;
	config[i].value = value;
}

void fill_the_array(config_file config[]){

	add_to_struct(config, 0, "nop", 3);
	add_to_struct(config, 1, "bcompress", 4);
	add_to_struct(config, 2, "bdecompress", 4);
	add_to_struct(config, 3, "gcompress", 2);
	add_to_struct(config, 4, "gdecompress", 2);
	add_to_struct(config, 5, "encrypt", 2);
	add_to_struct(config, 6, "decrypt", 2);
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