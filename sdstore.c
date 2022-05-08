#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>
#define MAX_BUFF 1024

void status(char buffer[], char pid[]){
	int i =0;

	if(mkfifo(pid, 0666) == -1){
		perror("Erro ao criar o pipe");
	}
	printf("Pipe com o pid %s aberto\n", pid);

	int pipe_pid = open(pid, O_RDONLY, 0666);

	while(read(pipe_pid, buffer+i, 1) > 0){
		i+=1;
	}

	write(1, buffer, i);


	close(pipe_pid);
	unlink(pid);
}

int main(int argc, char *argv[]){
	char buffer[MAX_BUFF];

	if(argc == 1){
		int size = snprintf(buffer, MAX_BUFF, "./sdstore status\n./sdstore proc-file <priority> input-filename output-filename transformation-id1 transformation-id2 ...\n");
		write(1, buffer, size);
	} else {
		int wr = open("main_pipe", O_WRONLY, 0666);
		if(wr == -1){
			perror("Erro ao abrir o main_pipe");
			return 3;
		}
		printf("Abertura do pipe de escrita com sucesso\n");

		char pid[10];
		snprintf(pid,10," %d\0", getpid());
		

		int i=0;
		int tamanho=0;
		for(i=0; i<argc-1; i+=1){
			strcpy(buffer+tamanho, argv[i]);
			tamanho += strlen(argv[i]);
			strcpy(buffer+tamanho, " ");
			tamanho += 1;
		}
		strcpy(buffer+tamanho, argv[i]);
		tamanho += strlen(argv[i]);
		// incluir o pid do processo na passagem de informação 
		strcpy(buffer+tamanho, pid);		
		tamanho += sizeof(pid);
		


		//write(wr, argv[i], strlen(argv[i]));
		//write(wr, "\0", 1);
		int s = write(wr, buffer, tamanho);
		printf("Escrevi no pipe %d bytes\n", s);
		close(wr);

		if(argc == 2){
			snprintf(pid,10,"%d\0", getpid());
			status(buffer, pid);
		}

	}


	return 0;
}