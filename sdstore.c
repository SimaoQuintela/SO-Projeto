#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>
#define MAX_BUFF 1024


int main(int argc, char *argv[]){
	char buffer[MAX_BUFF];

	if(argc == 1){
		int size = snprintf(buffer, MAX_BUFF, "./sdstore status\n./sdstore proc-file <priority> input-filename output-filename transformation-id1 transformation-id2 ...\n");
		write(1, buffer, size);
	} else {
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
		write(wr, "\0", 1);
		close(wr);
	}




	return 0;
}