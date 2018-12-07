#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "CircuitRouter-Protocol.h"

#define CLI "/tmp/client"

int fcli, fserv;
char path[BUFFER_SIZE];

void apanhaINT(int sig){
	close(fserv);
	close(fcli);
	unlink(path);
	exit(0);
}

void leComando(char* ptr, int size){
	char c;
	int i=0, j;
	for (j=0; j<size; j++) ptr[j] = '\0';
	while ((c=getchar())!='\n' && i<=(size-1)){
		ptr[i++] = c;
	}
	ptr[i++] = '\n';
	ptr[i] = '\0';
}

int main(int argc, char** argv){
	pid_t pid;
	char buffer[BUFFER_SIZE], buffer_aux[BUFFER_SIZE], pidNumber[BUFFER_SIZE], *extention = ".pipe";
	strcpy(path, CLI);
	pid = getpid();
	sprintf(pidNumber, "%d", pid);
	strcat(path, pidNumber);
	strcat(path, extention);
	
	signal(SIGINT, apanhaINT);
	signal(SIGPIPE, apanhaINT);

	if (argc!=2){
		printf("Server pipe is missing.\n");
		exit(EXIT_FAILURE);
	}

	if ( (fserv = open(argv[1], O_WRONLY))<0){
		printf("Error opening server pipe.\n");
		exit(EXIT_FAILURE);
	}
	
	unlink(path);
	
	if (mkfifo(path, 0777) < 0){
		printf("Error creating client pipe.\n");
		exit(EXIT_FAILURE);		 
	}

	Message_Protocol msg;
	memset(&msg, 0, sizeof(Message_Protocol));

	while (1){

		buffer_aux[0] = '\0';
		leComando(buffer, BUFFER_SIZE);
		if (strcmp(buffer, "leave\n")==0) break;

		strcpy(msg.pipe, path);
		strcpy(msg.command, buffer);

		if ((write(fserv, &msg, sizeof(Message_Protocol))) < 0) exit(EXIT_FAILURE);
		if ((fcli = open(path, O_RDONLY))<0) exit(EXIT_FAILURE);
		if ((read(fcli, buffer_aux, BUFFER_SIZE))<0) exit(EXIT_FAILURE);
		if ((write(1, buffer_aux, strlen(buffer_aux))) < 0) exit(EXIT_FAILURE);
		close(fcli);
	}

	close(fserv);
	close(fcli);
	unlink(path);
	exit(EXIT_SUCCESS);
}
