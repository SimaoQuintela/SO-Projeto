#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>
#define MAX_BUFF 1024

char * itoa(int n, char * buffer, int radix);

int main(int argc, char *argv[]){
	char buffer[MAX_BUFF];

	if(argc == 1){
		int size = snprintf(buffer, MAX_BUFF, "./sdstore status\n./sdstore proc-file <priority> input-filename output-filename transformation-id1 transformation-id2 ...\n");
		write(1, buffer, size);
	} else {
	/*
		char pid[10];
		snprintf(pid,10,"%d\0", getpid());
		printf("Pid: %d\n", getpid());
	
		int write_pid = open("pipe_pids", O_WRONLY, 0666);
		if(write_pid == -1){
			perror("Erro ao abrir o pipe dos pids");
			return 2;
		}

		write(write_pid, pid, 10);
		close(write_pid);
	*/
		int wr = open("main_pipe", O_WRONLY, 0666);
		if(wr == -1){
			perror("Erro ao abrir o main_pipe");
			return 3;
		}
		printf("Abertura do pipe de escrita com sucesso\n");


		int i=0;
		int tamanho_total=0;
		int tamanho=0;
		for(i=0; i<argc-1; i+=1){
			strcpy(buffer+tamanho, argv[i]);
			tamanho += strlen(argv[i]);
			strcpy(buffer+tamanho, " ");
			tamanho += 1;
		//	write(wr, argv[i], strlen(argv[i]));
		//	write(wr, " ", 1);
		}

		strcpy(buffer+tamanho, argv[i]);
		tamanho += strlen(argv[i]);
		strcpy(buffer+tamanho+1, "\0");
		tamanho +=1;


		//write(wr, argv[i], strlen(argv[i]));
		//write(wr, "\0", 1);
		write(wr, buffer, tamanho);
		printf("Escrevi no pipe\n");
		close(wr);
	}




	return 0;
}