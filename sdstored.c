#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <string.h>

#define MAX_BUFF 1024
#define READ 0
#define WRITE 1

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
	char pid[10];
	char* line[MAX_BUFF];			// descrição da task	
	int in_process;					// 0 -> por processar 1-> em processamento
	struct tasks_running *prox_task;
} *tasks_running;

// variáveis globais
	tasks_running tasks;
	config_file conf_file[7];

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
void add_task(tasks_running *tasks, int running_tasks, char* input[], char pid[], int N, int status){
	tasks_running nova;
	nova = malloc(sizeof(struct tasks_running));
	for(int i=0; i<N; i+=1){
		nova->line[i] = malloc(strlen(input[i]));
		strcpy(nova->line[i],input[i]);
	}
	nova->task_num = running_tasks;
	nova->in_process = status;
	nova->prox_task = NULL;
	strcpy(nova->pid, pid);

	while(*tasks != NULL && (*tasks)->task_num < running_tasks){
		tasks = &((*tasks)->prox_task);
	}
	nova->prox_task = *tasks;
	*tasks = nova;

}

void print_linked_list(tasks_running *tasks, int N){
	tasks_running temp = *tasks;
	while(temp != NULL){
		printf("Linha: ");
		for(int i=0; i<N; i+=1){
			printf("%s ", temp->line[i]);
		}
		printf("\nPid: %s\n", temp->pid);
		printf("Task #%d\n", temp->task_num);
		printf("Em processamento: %d\n", temp->in_process);
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

char* cria_transf(char* transf, char* path_transf_folder, char* transformations[], int index){
	int length_path_folder = strlen(path_transf_folder);
	int length_transf = strlen(transformations[index]);
	
	transf = malloc(sizeof(length_path_folder + length_transf));
	strcpy(transf, path_transf_folder);
	strcpy(transf + length_path_folder, transformations[index]);

	return transf;
//	printf("%s\n", transf);
}


void transformations(char* line[], int num_args, char* path_transf_folder, int fixed_args){
	
	char *transformations[num_args-fixed_args];
	int num_transfs = num_args-fixed_args-1;

	char* input_file = malloc(sizeof(line[fixed_args-2]));
	strcpy(input_file, line[fixed_args-2]);

	char* output_file = malloc(sizeof(line[fixed_args-1]));
	strcpy(output_file, line[fixed_args-1]);

	/**
	 * Coloca no array transformations as transformações a executar
	 */
	for(int i=0; i<(num_args-fixed_args); i+=1){
		transformations[i] = malloc(sizeof(line[i+fixed_args]));
		strcpy(transformations[i],line[i+fixed_args]);
	//	printf("%s\n", transformations[i]);
	}

	int p[num_transfs-1][2];
	char *transf;

	for(int i =0; i<num_transfs; i+=1){
		printf("num transfs: %d\n", num_transfs);
		// transf ----> SDStore-transf/transformação
		transf = cria_transf(transf, path_transf_folder, transformations, i);
		printf("%s\n", transf);
		
		if(i == 0){
			pipe(p[i]);
			if(fork() == 0){
				close(p[i][READ]);
				printf("executei no i = %d\n", i);
				dup2(p[i][WRITE], WRITE);
				close(p[i][WRITE]);
				printf("\nTransf: %s\n", transf);
				printf("Input File: %s\n", input_file);
				printf("Output File: %s\n", output_file);
				execlp(transf, transf, input_file, output_file,NULL);
				perror("Algo de errado aconteceu");
			} else {
				close(p[i][WRITE]);
			}
		} else if (i == num_transfs-1) {
			if(fork() == 0){
				printf("executei no i = %d", i);
				dup2(p[i-1][READ], READ);
				close(p[i-1][READ]);
				printf("\nTransf: %s\n", transf);
				printf("Input File: %s\n", input_file);
				printf("Output File: %s\n", output_file);
				execlp(transf, transf, input_file, output_file, NULL);
				perror("Algo de errado aconteceu");
			} else {
				close(p[i-1][READ]);
			}
		} else {
			pipe(p[i]);
			if(fork() == 0){
				printf("executei no i = %d", i);
				close(p[i][READ]);
				dup2(p[i-1][READ], READ);
				close(p[i-1][READ]);
				dup2(p[i][WRITE], WRITE);
				close(p[i][WRITE]);
				printf("\nTransf: %s\n", transf);
				printf("Input File: %s\n", input_file);
				printf("Output File: %s\n", output_file);
				execlp(transf, transf, input_file, output_file, NULL);
				perror("Algo de errado aconteceu");
			} else {
				close(p[i-1][READ]);
				close(p[i][WRITE]);
			}
		}
		
	}
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

int available(config_file conf_file[], char* line[], int num_args, int fixed_args){
           
    int i, ind;
	int r=1;
	int arr[7] = {0,0,0,0,0,0,0};   //array onde estará guardado, no respetivo indice da transformaçao, o numero de transformaçoes a executar

	for(i=fixed_args; i<(num_args); i++){
	    ind = getIndice(line[i]);
	    arr[ind]++;  
   
	}
	
	
	for(i=0; i<7; i++){   
		if((conf_file[i].current_num_transf + arr[i]) > conf_file[i].max_exec_transf) {
			r = 0; //se os que estao a correr mais os que queremos correr for maior que o maximo, retorna 0	
			break;
		}
	}

	if(r == 1){
		for(i=0; i<7; i+=1){
			conf_file[i].current_num_transf += arr[i];
		}
	}

	return r;     
}


void write_line_to_execute(char* line_to_execute[]){

}



int main(int argc, char *argv[]){

	// dá informação de como se deve arrancar o server caso este seja inicializado incorretamente
	if(argc != 3){
		write(1, "./sdstored config-filename transformations-folder\n", sizeof("./sdstored config-filename transformations-folder\n"));
		return 1;
	}

	char buffer[MAX_BUFF];
	char pid[20];
	char buffer_pids[MAX_BUFF];
	char buffer_aux[MAX_BUFF];
	char *exec_args[MAX_BUFF];

	char* token;
	int i, fixed_args;

	int task_numero = 1;


	fill_struct_conf_file(conf_file, argv[1]);

	if(mkfifo("main_pipe", 0666) == -1){
		perror("Erro ao criar o pipe");
		return 2;
	}
	printf("Main pipe criado com sucesso\n");

	printf("Pipe de pids criado com sucesso\n");

	//signal(SIGTERM, close_pipe);
	while(1){
		i = 0;

		int rd = open("main_pipe", O_RDONLY, 0666);
		if(rd == -1){
			perror("Erro ao abrir o pipe de leitura");
			return 4;
		}
		while(read(rd, buffer+i, 1) > 0){  
			i+=1;
		}
	    buffer[i] = '\0';
		close(rd);
    

        int tamanho_input = strlen(buffer);
		strcpy(buffer_aux, buffer);
		printf("linha: %s\n", buffer_aux);
		int k = 0;
	 	token = strtok(buffer, " ");
        while(token != NULL){
            exec_args[k] = token;
        	token = strtok(NULL, " ");
        	k++;
        }
        exec_args[k] = "\0";
        char* line[k-1];
        char *pid;
        for(i=0; i<k-1; i+=1){
        	line[i] = malloc(strlen(exec_args[i]));
        	strcpy(line[i],exec_args[i]);
        }
        strcpy(line[k-1], "\0");
        strcpy(pid,exec_args[k]);

        if(k!=3 && strcmp(line[2], "-p") == 0){
        	fixed_args = 6;
        } else {
        	fixed_args = 4;
        }
        int num_args = k-1;
        char* line_to_execute[MAX_BUFF];

        if(k!=3){
        	if(available(conf_file, line, num_args, fixed_args) == 1){
			    // verificamos caso seja o status
			   	add_task(&tasks, task_numero, line, pid, num_args, 1);
		    
        	} else {
        		add_task(&tasks, task_numero, line, pid, num_args, 0);
        	}
        }
        task_numero += 1;
    
    //    write_line_to_execute(line_to_execute);
       
		if(fork() == 0){
		//	kill(getppid(), SIGUSR1);
        	if(num_args == 2){
        		print_linked_list(&tasks, k-1);
        	} else {
        	//	transformations(line_to_execute, num_args, argv[2], fixed_args);
        	}
        	_exit(-1);  
        }
	}

	unlink("main_pipe");
	return 0;
}
