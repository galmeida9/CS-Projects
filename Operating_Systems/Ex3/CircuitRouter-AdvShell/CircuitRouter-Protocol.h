#ifndef CIRCUITROUTER_PROTOCOL_H
#define CIRCUITROUTER_PROTOCOL_H

#define BUFFER_SIZE 1024

typedef struct {
	char pipe[BUFFER_SIZE];
	char command[BUFFER_SIZE];
} Message_Protocol;

#endif /* CIRCUITROUTER_PROTOCOL_H */

