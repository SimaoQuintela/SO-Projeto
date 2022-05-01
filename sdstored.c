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
	int task_num;					// número da task
	char line[MAX_BUFF];			// descrição da task	
	int in_process;					// 0 -> por processar 1-> em processamento
	struct tasks_running *prox_task;
} *tasks_running;


/**
 * tou a confiar que tá bem definida, testar quando tiver a concorrência a funcionar
 */ 
void remove_task(tasks_running *l, int num){
    
    for (; *l && (*l)->task_num < num; l = &(*l)->prox_task);
    
    if (*l && (*l)->task_num == num) {
        tasks_running temp = *l;
        *l = (*l)->prox_task;
        free(temp);
    }
    
}

/**
 * falta adicionar o in_process
*/
void add_task(tasks_running *tasks, int running_tasks, char input[]){
	tasks_running nova;
	nova = malloc(sizeof(struct tasks_running));
	strcpy(nova->line, input);
	nova->task_num = running_tasks;
	nova->prox_task = NULL;

	while(*tasks != NULL && (*tasks)->task_num < running_tasks){
		tasks = &((*tasks)->prox_task);
	}
	nova->prox_task = *tasks;
	*tasks = nova;

}

void print_linked_list(tasks_running *tasks){
	tasks_running temp = *tasks;
	while(temp != NULL){
		printf("%s\n", temp->line);
		printf("%d\n", temp->task_num);
		temp = temp->prox_task;
	}
}


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
		//	printf("%s\n", buffer);
			conf_file[line].transformation = (char *)malloc(sizeof(i));
			strcpy(conf_file[line].transformation,buffer);
			i = 0;
		} else if(buffer[i] == '\n'){
			buffer[i] = '\0';
		//	printf("%s\n", buffer);
			conf_file[line].max_exec_transf = atoi(buffer);
			conf_file[line].current_num_transf = 0;
			i = 0;
			line+=1;
		} else {
			i+=1;
		}

	}

	/* print à struct com os valores lidos no config file
	for(int i = 0; i<7; i+=1){
		printf("\nTransformation: %s\nCurrent_num_transf: %d\nMax_exec_transf: %d\n", conf_file[i].transformation, conf_file[i].current_num_transf, conf_file[i].max_exec_transf);
	}
	*/
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

int getIndice(char* transf){   //funçao auxiliar que apenas nos da o indice de ocorrencia da transformaçao no conf_file[]
	int r;

	if(strcmp(transf, "nop") == 0) r = 0;
	else if(strcmp(transf, "bcompress") == 0) r = 1;
	else if(strcmp(transf, "bdecompress") == 0) r = 2;
	else if(strcmp(transf, "gcompress") == 0) r = 3;
	else if(strcmp(transf, "gdecompress") == 0) r = 4;
	else if(strcmp(transf, "encrypt") == 0) r = 5;
	else if(strcmp(transf, "decrypt") == 0) r = 6;
        else r=-1;

        return r;

}

int available(config_file conf_file[], char* exec_args[], int num_args){
           
           int i, ind;
           int r=1; //assumo que há espaço

	   for(i=4; i<(num_args); i++){        //ciclo começa em i=4 pois as transformaçoes apenas começam no indice 4 (descartando a palha do ./sdstore procfile in out)
	       	  ind = getIndice(exec_args[i]);
	       	  if(conf_file[ind].current_num_transf >= conf_file[ind].max_exec_transf) r=0; //numero maximo atingido, na comparaçao bastaria ==, apenas meti >= porque why not
      
	   }

	   return r;

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

	int running_tasks = 0;
	tasks_running tasks;


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
        int tamanho_input = strlen(buffer);
		char buffer_aux[tamanho_input];
		strcpy(buffer_aux, buffer);

        running_tasks += 1;
		if(strcmp(buffer, "./sdstore status") != 0){
			add_task(&tasks, running_tasks, buffer);
		}

	 	token = strtok(buffer, " ");
        while(token != NULL){
            exec_args[j] = token;
        	token = strtok(NULL, " ");
        	j++;
        }
        exec_args[j] = "\0";
        int num_args = j;
        
        while(available(conf_file, exec_args, num_args) != 0){
        	//	printf("Não posso executar\n");

        }

		if(fork() == 0){
        	if(strlen(buffer_aux) % 2 == 0){
        		sleep(5);
        	}	 
        	printf("%s\n", buffer_aux);
        //	printf("running tasks: %d\n", running_tasks);
        	if(j == 2){
        		print_linked_list(&tasks);
    	//		status(tasks);
        	} else {
        		transformations(exec_args, num_args, argv[2]);
        	}
        	running_tasks -= 1;   
        	_exit(-1);  
        }   
	}

	unlink("main_pipe");

	return 0;
}
