#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFERSIZE 100 /*Default size for the buffer*/
#define ARRAYSIZE 10 /*Default size for the array of processes*/

typedef struct process {
	int pid, status;
} *Process;

/*Initialization of variables and pointers*/
/* maxChildren has the value -1 if we dont start the programm with MAXCHILDREN 
	argument*/

int main(int argc, char *argv[]) {
	int pid, maxChildren = -1, pos = 0, i, size = ARRAYSIZE;
	char *buffer, *token, *path;
	int children = 0, status;
	Process *array;

	if (argc == 2)
		sscanf(argv[1], "%d", &maxChildren);
	if ((argc == 2) && maxChildren == -1)
		exit(-1); /* programm exits if we gave one argument that is not an int value*/
	array = (Process*) malloc(sizeof(Process) * size);
	buffer = (char *) malloc(sizeof(char) * BUFFERSIZE);
	
	/*Shell commands and fork*/

	do { 
		fgets(buffer,BUFFERSIZE, stdin);
		token = strtok(buffer, "\n");
		token = strtok(buffer, " ");
		if (strcmp(token, "run") == 0 && (token = strtok(NULL, " ")) != NULL) {
			path = strdup(token);
			if ((token = strtok(NULL, " ")) != NULL)
				continue;
			children++;
			if (children > size) {
				size += ARRAYSIZE;
				array = (Process *) realloc(array, size);
			}
			pid = fork();
			if ((maxChildren != -1) && (children > maxChildren)) {
				pid = wait(&status);
				for (i = 0; i < pos; i++)
					if (array[i]->pid == pid) {
						array[i]->status = status;
						break;
					}
    			children--;
			}
			if (pid == 0) {
				execl("CircuitRouter-SeqSolver", "CircuitRouter-SeqSolver", path, NULL);
				exit(0);
			}
			else if (pid > 0) {
				array[pos] = (Process) malloc(sizeof(struct process));
				array[pos]->pid = pid;
    			pos++;
			}
			free(path);
		}
	} while (strcmp(buffer, "exit") != 0);
	free(buffer);
	
	/*Exit command*/

	for (i = 0; i < children; i++) {
		pid = wait(&status);
		for (i = 0; i < pos; i++)
			if (array[i]->pid == pid) {
				array[i]->status = status;
				break;
			}
	}
	
	for (i = 0; i < pos; i++) {
		printf("CHILD EXITED (PID=%d; return %s)\n", array[i]->pid,array[i]->status == 0 ? "OK" : "NOK");
		free(array[i]);
	}
	
	free(array);
	printf("END.\n");
	return 0;
}