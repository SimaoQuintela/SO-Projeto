#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>

#define MAX_BUFF 1024

/**
 * Estrutura com as transformações e informação sobre as mesmas
 */
typedef struct config_file {
	char* transformation;			// transformação
	int current_num_transf;  		// número de transformações a correr 
	int max_exec_transf;			// número máximo de transformações que podem correr
} config_file;

/**
 * Lista ligada que representa as tasks que estão a correr de momento
 */
typedef struct tasks_running {
	int task_num;
	char* line;
	struct tasks_running *prox_task;
} *tasks_running;


/*
void add_task_to_linked_list(tasks_running *tasks, int running_tasks, char* input[]){
	tasks_running nova;
	nova = malloc(sizeof(struct tasks_running));
	nova->line;
	for(int i=0; input; i+=1){
		nova->line[i] = input[i];
	}
	nova->task_num = running_tasks;
	nova->prox_task = NULL;

	while(*tasks != NULL && (*tasks)->task_num < running_tasks){
		tasks = &((*tasks)->prox_task);
	}
	nova->prox_task = *tasks;
	*tasks = nova;

}
*/

/*
void print_linked_list(tasks_running *tasks){
	tasks_running temp = *tasks;
	while(temp != NULL){
		for(int i = 0; i<MAX_BUFF; i+=1){
			printf("%s\n", temp->line[i]);
		}
		printf("%d\n", temp->task_num);
		temp = temp->prox_task;
	}
}
*/

void fill_struct_conf_file(config_file conf_file[], char* path_to_conf_file){
	char buffer[MAX_BUFF];
	int i = 0;
	int line = 0;

	int fd = open(path_to_conf_file, O_RDONLY, 0666);
	if(fd == -1){
		perror("Erro ao abrir o ficheiro de configuração");
	}

	while(read(fd, buffer+i, 1)){
		if( buffer[i] == ' '){
			buffer[i] = '\0'; 
			printf("%s\n", buffer);
			conf_file[line].transformation = (char *)malloc(sizeof(i));
			strcpy(conf_file[line].transformation,buffer);
			i = 0;
		} else if(buffer[i] == '\n'){
			buffer[i] = '\0';
			printf("%s\n", buffer);
			conf_file[line].max_exec_transf = atoi(buffer);
			conf_file[line].current_num_transf = 0;
			i = 0;
			line+=1;
		} else {
			i+=1;
		}

	}

	for(int i = 0; i<7; i+=1){
		printf("\nTransformation: %s\nCurrent_num_transf: %d\nMax_exec_transf: %d\n", conf_file[i].transformation, conf_file[i].current_num_transf, conf_file[i].max_exec_transf);
	}
}


void transformations(char* exec_args[], int num_args, char* path_transf_folder){
	char *transformations[num_args-4];
	for(int i=0; i<(num_args-4); i+=1){
		transformations[i] = exec_args[i+4];
	//	printf("%s\n", transformations[i]);
	}

	/*
	* Falta executar as transformações
	* Marcar reunião com o grupo para fazer isto pois é a parte mais "difícil"
	*/
}

int main(int argc, char *argv[]){

	// dá informação de como se deve arrancar o server caso este seja inicializado incorretamente
	if(argc != 3){
		write(1, "./sdstored config-filename transformations-folder\n", sizeof("./sdstored config-filename transformations-folder\n"));
		return 1;
	}

	char buffer[MAX_BUFF];
	char* token;
	char *exec_args[MAX_BUFF];
	int i, j;
	config_file conf_file[7];

//	int running_tasks = 0;


	fill_struct_conf_file(conf_file, argv[1]);

	// abertura do pipe que faz a ligação cliente-servidor
	if(mkfifo("main_pipe", 0666) == -1){
		perror("Erro ao criar o pipe");
		return 2;
	}

	printf("pipe criado com sucesso\n");

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

		/**
		 * Fazer pipe de pids para passar o pid do processo
		 */

	 	token = strtok(buffer, " ");
        
        while(token != NULL){
            exec_args[j] = token;
        	token = strtok(NULL, " ");
        	j++;
        }

        exec_args[j] = "\0";
        int num_args = j;
    //    running_tasks += 1;

        if(num_args == 2){
    //    	add_task_to_linked_list(tasks, running_tasks, exec_args);
    //    	print_linked_list(tasks);
        //	status(tasks);
        } else {
        	transformations(exec_args, num_args, argv[2]);
        }
    //    running_tasks -= 1;

		
		for(i=0; exec_args[i] !=  "\0"; i+=1){
			printf("%s\n", exec_args[i]);
		}
		
	
	
	}

	unlink("main_pipe");

	return 0;
}
