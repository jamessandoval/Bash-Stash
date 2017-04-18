/**** SmallSh.c *******
*
* James Sandoval
* 23 Mqr 2017
* smallsh.c 
* - basic shell implemmentation
*  uses fork(), exec(), waitpid()
*
**********************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>



// Takes user input and populates input array
char **getInput(int *bg_processes){


	char **inputArr = malloc(sizeof(char*) * 512);
	char *input = malloc(sizeof(char*) * 2048);
	char *word = NULL;

	int arraySize = 100;


	printf("smallsh:");

	fgets(input, arraySize, stdin);

	word = strtok(input, "\n");

	// check the first argument
	// if no entry return null pointer.
	if(word == NULL){
		return inputArr = NULL;
	}else{

	// otherwise acquire the first argument.
	word = strtok(input, " ");

	}

	// check for additional arguments
	// if present add to inputArrs
	int i = 0;
	while(word != NULL){

		inputArr[i] = word;
		i++;
		word =strtok(NULL, " ");
	}

	// 
	if(strcmp(inputArr[i-1], "&") == 0){

		*bg_processes = 1;
		inputArr[i-1] = NULL;
	}


	return inputArr;
}


// Exec Processes
int processExec(char **inputArr, int *value, int *bg_processes){

	struct sigaction exec_process;
	exec_process.sa_flags = 0;

	exec_process.sa_handler = SIG_IGN;
	sigaction(SIGINT, &exec_process, NULL);

	int fd;
	int fork_fd;
	int child_pid;
	int cont_running = 5;

	int input = 0;
	int output = 0;
	int status = 0 ;
	int i = 0; 
	int error_msg = 0;
	int wpid= -5;

	// Check if comment
	if(strcmp(inputArr[0],"#") == 0){

		return cont_running;

	}
    // If no input
	if(inputArr[0] == NULL){
		printf("Return:\n");
		return cont_running;
	}
    
    // cd command
	if(strcmp(inputArr[0], "cd") == 0){
		// as in bash when cd is input, user is returned to home dir
		if(inputArr[1] == NULL){
			char *dir_path = getenv("HOME");
			chdir(dir_path);
		}else{

			char *dir_path = inputArr[1];
			chdir(dir_path);
		}

		*value = 0;
		return cont_running;
    // status command
	}else if(strcmp(inputArr[0], "status") == 0){

		if(*value == 1 || *value == 0){
			printf("EXIT VALUE %d\n", *value);

		}
		else{

			printf("Terminate by: %d\n", *value);
		}

		*value = 0;
		return cont_running;
    // exit command
	}else if(strcmp(inputArr[0], "exit") == 0){

		return 0;
    // process operators
	}else if(inputArr[0] != NULL && inputArr[1] != NULL){
		if(strcmp(inputArr[1], ">") == 0){

			output = 1;
			fd = open(inputArr[2], O_WRONLY|O_CREAT|O_TRUNC, 0664);
			if(fd == -1){

				printf("ERROR: NO FILE OR DIRECTORY\n");
				fflush(0);
				error_msg = 1;
			}
		}
		if(strcmp(inputArr[1], "<") == 0){

			input = 1;
			if(inputArr[2] == NULL){

				fd= open("/dev/null", O_RDONLY);
				inputArr[1] = NULL;
			}else{

				fd = open(inputArr[2], O_RDONLY);
				if(fd == -1){
					printf("ERROR: CANNOT OPEN %s\n", inputArr[2]);
					fflush(0);
					error_msg = 1;
				}
			}

		}

	}
	// Create a new Process -> becomes child process of caller.
	child_pid = fork();


	// check to see if fork is successfully created
	if(child_pid == -1){

		printf("ERROR: fork failed.\n");
		_exit(1);
	}
	if(child_pid == 0){

		if(output == 1){

			fork_fd = dup2(fd, 1);
			inputArr[1] = NULL;
			inputArr[2] = NULL;
		}
		if(input == 1){
			fork_fd = dup2(fd, 0);
			inputArr[1] = NULL;
			inputArr[2] = NULL;
			if(fd == -1){

				_exit(1);
			}
		}

		if(*bg_processes == 0){
			exec_process.sa_handler = SIG_DFL;
			exec_process.sa_flags = 0;
			sigaction(SIGINT, &exec_process, NULL);
		}

		int command_success = execvp(inputArr[0], inputArr);

		// if command_success fails
		if(command_success == -1 && inputArr[1] == NULL){

			perror("SMALLSH COMMAND ERROR");
			_exit(1);
		}

		if(input == 1 || output == 1){

			close(fork_fd);
		}

		_exit(0);

	}else{
		if(input ==1 || output == 1){

			close(fd);
		}

		if(*bg_processes == 0){

			wpid = waitpid(child_pid, &status, WUNTRACED);
		}else if(*bg_processes == 1){
			do{

				wpid = waitpid(-1, &status, WNOHANG);
			}while(!WIFEXITED(status) && !WIFSIGNALED(status));

			printf("Background PID is running on %d\n", child_pid);

		}
		if(WIFEXITED(status)){
			*value = WEXITSTATUS(status);
		}

		return cont_running;

	}

}


// Function Main
int main(int argc, char *argv[]){

	int cont_running = 5;
	int bg_processes = 0;
	int value = 0;
	int status;
	int pid;
	char ** inputArr;


	do{
		pid = waitpid(-1, &status, WNOHANG);
		
		if(pid > 0){
			
			if(status != 0){

				printf("ERROR: Background process with pid: %d terminated by %d\n", pid, status);
			}else{

				printf("Background process with pid %d exited with signal %d\n", pid, status); 
			}
		}

			//background process -> foreground
			bg_processes = 0;

			inputArr = getInput(&bg_processes);
			if(inputArr != NULL){
				cont_running = processExec(inputArr, &value, &bg_processes);
			}


			fflush(0);
		free(inputArr);
		

	}while(cont_running == 5);



	return 0;
}
