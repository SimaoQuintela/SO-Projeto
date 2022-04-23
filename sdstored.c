#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>

#define MAX_BUFF 1024

/**
 * Dicionário com as transformações
 */
typedef struct config_file {
	char* transformation;
	int max_exec_trans;
} config_file;


void transformations(char* exec_args[], int num_args){
	char *transformations[num_args-4];
	for(int i=0; i<(num_args-4); i+=1){
		transformations[i] = exec_args[i+4];
		//strcpy(transformations[i], exec_args[num_args-4]);
		printf("%s\n", transformations[i]);
	}

	/*
	* Falta executar as transformações
	* Marcar reunião com o grupo para fazer isto pois é a parte mais "difícil"
	*/
}

// !!!  lembrar-me de ler o config file e meter no dicionário
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

	/// se dois clientes mandarem o pedido ao mesmo tempo
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

	 	token = strtok(buffer, " ");
        
        while(token != NULL){
            exec_args[j] = token;
        	token = strtok(NULL, " ");
        	j++;
        }

        exec_args[j] = "\0";
        int num_args = j;

        if(num_args == 2){
        	// status()
        } else {
        	transformations(exec_args, num_args);
        }

		/*
		for(i=0; exec_args[i] !=  "\0"; i+=1){
			printf("%s\n", exec_args[i]);
		}
		*/
		
	
	}

	unlink("main_pipe");

	return 0;
}
