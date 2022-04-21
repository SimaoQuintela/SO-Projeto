#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>
#define MAX_BUFF 1024

/*
void write_on_config_file(char* args[], int num_args){
	int fd = open("config_file.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	char* transformation;
	int process_limit;
	char buffer[MAX_BUFF];
	int size;

	for(int i=1 ; i<num_args; i+=1){
		// process_limit = 3 -> nop
		if(!strcmp(args[i], "nop")){
			process_limit = 3;
	    
	    // process_limit = 4 -> bcompress, bdecompress
		} else if (!strcmp(args[i], "bcompress") || !strcmp(args[i], "bdecompress")) {
			process_limit = 4;
		
		// process_limit = 2 -> gcompress, gdecompress, encrypt, decrypt 
		} else if(!strcmp(args[i], "gcompress") || !strcmp(args[i], "gdecompress") || !strcmp(args[i], "encrypt") || !strcmp(args[i], "decrypt")) {
			process_limit = 2;
		}
		


		transformation = args[i];
		size = snprintf(buffer, MAX_BUFF, "%s %d\n", transformation, process_limit);
		write(fd, buffer, size);
		
	}

	close(fd);
}
*/

void parse_args(char *args[], int num_args){
	char buffer[MAX_BUFF];

	// info about client sintax 
	if(num_args == 1){
		int size = snprintf(buffer, MAX_BUFF, "./sdstore status\n./sdstore proc-file <priority> input-filename output-filename transformation-id1 transformation-id2 ...\n");
		write(1, buffer, size);
	// info about process status
	} else if(num_args == 2) {
		write(1, "descobrir como se faz isto\n", sizeof("descobrir como se faz isto"));
	} else {
		char* path_to_process_file;
		char* path_to_output_folder;

		path_to_process_file = args[2];
		path_to_output_folder = args[3];

	//	write_on_config_file(args, num_args);
	//	printf("%s\n", path_to_process_file);
	//	printf("%s\n", path_to_output_folder);
	}



}


int main(int argc, char *argv[]){
	parse_args(argv, argc);	

	int wr = open("main_pipe", O_WRONLY, 0666);
	if(wr == -1){
		perror("Erro ao abrir o main_pipe");
		return 1;
	}
	printf("Abertura do pipe de escrita com sucesso\n");


	int i=0;
	for(i=0; i<argc-1; i+=1){
		write(wr, argv[i], strlen(argv[i]));
		write(wr, " ", 1);
	}
	write(wr, argv[i], strlen(argv[i]));
//    write(wr, "\n", 1);
	write(wr, "\0", 1);
	close(wr);




	return 0;
}