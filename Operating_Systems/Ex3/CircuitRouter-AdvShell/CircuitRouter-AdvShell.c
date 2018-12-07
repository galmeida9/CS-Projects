
/*
// Projeto SO - exercise 3, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
*/
 
#include "lib/commandlinereader.h"
#include "lib/vector.h"
#include "CircuitRouter-AdvShell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include "lib/timer.h"

#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"
#define SERVER_PATH "/tmp/"

#define MAXARGS 3

int runningChildren = 0;
vector_t *children;

int main (int argc, char** argv) {

	char *args[MAXARGS + 1], buffer[BUFFER_SIZE], pathPipe[BUFFER_SIZE], pathServer[BUFFER_SIZE];
	char *msg_serv = "Starting SERVER pipe.\n",
		 *msg_wait = "Wainting for results.\n",
		 *commandNotSupported = "Command not supported.\n",
         *extension = ".pipe";
	int MAXCHILDREN = -1, fserv, fcli, maxFD;
	fd_set readset;
	struct sigaction handle_child;
	handle_child.sa_handler = childTime;
	handle_child.sa_flags = SA_RESTART;

    strcpy(pathServer, "");
    strcat(pathServer, SERVER_PATH);
    strcat(pathServer, argv[0]);
    strcat(pathServer, extension);

	sigemptyset(&handle_child.sa_mask);
	sigaction(SIGCHLD, &handle_child, NULL);

	if(argv[1] != NULL){
		MAXCHILDREN = atoi(argv[1]);
	}
	
	children = vector_alloc(MAXCHILDREN);

	unlink(pathServer);

    printf("%s\n", pathServer);
	if (mkfifo(pathServer, 0777)<0){
		printf("Error creating server pipe.\n");
		exit(EXIT_FAILURE);
	}

	printf("Welcome to CircuitRouter-AdvShell\n\n");
	
	if ((write(1, msg_serv, strlen(msg_wait))) < 0) exit(EXIT_FAILURE);
	if ((fserv = open(pathServer, O_RDONLY | O_NONBLOCK))<0){
		printf("Error initializing server pipe.\n");
		exit(EXIT_FAILURE);
	}
	
	FD_ZERO(&readset);
	FD_SET(fserv, &readset);
	FD_SET(fileno(stdin), &readset);
	maxFD = fileno(stdin) > fserv ? fileno(stdin) : fserv;

	if ((write(1, msg_wait, strlen(msg_wait))) < 0) exit(EXIT_FAILURE);

	while (1) {
		int numArgs, hasClient=0, result;
		Message_Protocol msg;
		strcpy(pathPipe, "");
		pathPipe[0] = '\0';
		buffer[0] = '\0';

		FD_SET(fserv, &readset);
		FD_SET(0, &readset);

		result = select(maxFD+1, &readset, NULL, NULL, NULL);
		if (result == -1){
			continue;
		}
		else if (result){
			if (FD_ISSET(fileno(stdin), &readset)){
                fgets(buffer, BUFFER_SIZE, stdin);
			}
			else if (FD_ISSET(fserv, &readset)){
				if (read(fserv, &msg, sizeof(Message_Protocol))<=0) continue;
				strcpy(pathPipe, msg.pipe);
				strcpy(buffer, msg.command);

				if((fcli = open(pathPipe, O_WRONLY)) < 0 ){
					printf("Error opening client pipe.\n");
					exit(EXIT_FAILURE);
				}
				hasClient=1;
			}
		}

		if (hasClient==1) printf("%s\n", buffer);
		numArgs = parseArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
		/* EOF (end of file) do stdin ou comando "sair" */
		if (numArgs < 0 || (numArgs > 0 && (hasClient==0) && (strcmp(args[0], COMMAND_EXIT) == 0))) {
			printf("CircuitRouter-AdvShell will exit.\n--\n");

			/* Espera pela terminacao de cada filho */
			while (runningChildren > 0) waitForChild(children);

			printChildren(children);
			printf("--\nCircuitRouter-AdvShell ended.\n");
			close(fserv);
			unlink(pathServer);
			break;
		}

		else if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0){
			int pid;
			if (numArgs < 2) {
				printf("%s: invalid syntax. Try again.\n", COMMAND_RUN);
				if (hasClient==1) 
						if ((write(fcli, commandNotSupported, strlen(commandNotSupported)+1)) < 0) exit(EXIT_FAILURE);
				continue;
			}
			if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN) waitForChild(children);

			pid = fork();
			if (pid < 0) {
				perror("Failed to create new process.");
				exit(EXIT_FAILURE);
			}

			if (pid > 0) {
				runningChildren++;
				child_t *child = malloc(sizeof(child_t));
				if (child == NULL) {
					perror("Error allocating memory");
					exit(EXIT_FAILURE);
				}
				child->pathPipe = strdup(pathPipe);
				child->pid = pid;
				TIMER_T startTime;
				TIMER_READ(startTime);
				child->start_time = startTime;
				vector_pushBack(children, child);
				printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
				continue;
			} else {
				char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
				char *newArgs[4] = {seqsolver, args[1], strcmp(pathPipe, "") == 0 ? NULL : pathPipe, NULL};

				execv(seqsolver, newArgs);
				perror("Error while executing child process"); // Nao deveria chegar aqui
				if (hasClient == 1 && (write(fcli, commandNotSupported, strlen(commandNotSupported)+1)) < 0) exit(EXIT_FAILURE);
				exit(EXIT_FAILURE);
			}
		}

		else if (numArgs == 0 && hasClient==0){
			/* Nenhum argumento; ignora e volta a pedir */
			continue;
		}
		
		else if (hasClient == 1){
			if ((write(fcli, commandNotSupported, strlen(commandNotSupported)+1)) < 0) exit(EXIT_FAILURE);
			close(fcli);
		}
		else
			printf("%s", commandNotSupported);
	}

	for (int i = 0; i < vector_getSize(children); i++) {
		child_t *child = vector_at(children, i);
		free(child->pathPipe);
		free(vector_at(children, i));
	}
	
	vector_free(children);

	return EXIT_SUCCESS;
}

/*Auxiliary*/

void childTime(int sig){
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	TIMER_T stopTime;
	TIMER_READ(stopTime);
	for (int i = 0; i < vector_getSize(children); ++i) {
		child_t *child = vector_at(children, i);
		if((child->pid) == pid) {
			child->status = status;
			child->stop_time = stopTime;
			break;
		}		
	}
	runningChildren--;
}


int parseArguments(char **argVector, int vectorSize, char *buffer, int bufferSize){
	int numTokens = 0;
	char *s = " \r\n\t";
	int i;
	char *token;

	if (argVector == NULL || buffer == NULL || vectorSize <= 0 || bufferSize <= 0)
		return 0;

	/* get the first token */
	token = strtok(buffer, s);

	/* walk through other tokens */
	while( numTokens < vectorSize-1 && token != NULL ) {
		argVector[numTokens] = token;
		numTokens ++;
		token = strtok(NULL, s);
	}

	for (i = numTokens; i<vectorSize; i++) {
		argVector[i] = NULL;
	}

	return numTokens;
}

void waitForChild(vector_t *children) {
	while (runningChildren > 0);
}

void printChildren(vector_t *children) {
	for (int i = 0; i < vector_getSize(children); ++i) {
		child_t *child = vector_at(children, i);
		int status = child->status;
		pid_t pid = child->pid;
		if (pid != -1) {
			const char* ret = "NOK";
			if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
				ret = "OK";
			}
			printf("CHILD EXITED: (PID=%d; return %s; %.2f s)\n", pid, ret, TIMER_DIFF_SECONDS(child->start_time, child->stop_time));
		}
	}
	puts("END.");
}
