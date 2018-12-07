#ifndef CIRCUITROUTER_SHELL_H
#define CIRCUITROUTER_SHELL_H

#include "lib/vector.h"
#include <sys/types.h>
#include "lib/timer.h"
#include "CircuitRouter-Protocol.h"

typedef struct {
	pid_t pid;
	int status;
	char *pathPipe;
	TIMER_T start_time;
	TIMER_T stop_time;
} child_t;


void childTime(int sig);
int parseArguments(char **argVector, int vectorSize, char *buffer, int bufferSize);
void waitForChild(vector_t *children);
void printChildren(vector_t *children);

#endif /* CIRCUITROUTER_SHELL_H */
