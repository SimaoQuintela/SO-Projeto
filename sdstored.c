#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>

#define MAX_BUFF 1024

int main(int argc, char *argv[]){
	char buffer[MAX_BUFF];

	if(mkfifo("fifo", 0666) == -1){
		perror("Erro ao criar o pipe\n");
		return 1;
	}
	printf("pipe criado com sucesso\n");
	int rd = open("fifo", O_RDONLY);
	printf("pipe de leitura aberto com sucesso\n");

	ssize_t size;
	int i = 0;
	while(read(rd, buffer+i, 1)){
		i+=1;
	}
	printf("%s\n", buffer);

// criar processo
//

	return 0;
}