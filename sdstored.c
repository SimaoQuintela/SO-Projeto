#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>

#define MAX_BUFF 1024

int main(int argc, char *argv[]){
	char buffer[MAX_BUFF];
	char* token;
	char *exec_args[15];
	int i, j;
	
	/* dá informação de como se deve arrancar o server caso este seja inicializado incorretamente
	if(argc != 3){
		write(1, "./sdstored config-filename transformations-folder\n", sizeof("./sdstored config-filename transformations-folder\n"));
		return 1;
	}*/
	

	// abertura do pipe que faz a ligação cliente-servidor
	if(mkfifo("main_pipe", 0666) == -1){
		perror("Erro ao criar o pipe");
		return 2;
	}

	printf("pipe criado com sucesso\n");

	// se dois clientes mandarem o pedido ao mesmo tempo
	// só por sorte é que funciona
	while(1){
		j = 0;
		i = 0;
		int rd = open("main_pipe", O_RDONLY, 0666);
		if(rd == -1){
			perror("Erro ao abrir o pipe de leitura");
			return 3;
		}

		while(read(rd, buffer+i, 1) > 0){  
			i+=1;
		}

		//printf("%s\n", buffer);    

	 	token = strtok(buffer, " ");
        
        while(token != NULL){
            exec_args[j] = token;
        	token = strtok(NULL, " ");
        	j++;
        }

        exec_args[j] = "end";

		
		for(i=0; exec_args[i] !=  "end"; i+=1){
			printf("%s\n", exec_args[i]);
		}
	

	//	printf("Mete outra linha men\n");
	}

	unlink("main_pipe");

	return 0;
}