/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This code is an adaptation of the Lee algorithm's implementation originally included in the STAMP Benchmark
 * by Stanford University.
 *
 * The original copyright notice is included below.
 *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) Stanford University, 2006.  All Rights Reserved.
 * Author: Chi Cao Minh
 *
 * =============================================================================
 *
 * Unless otherwise noted, the following license applies to STAMP files:
 *
 * Copyright (c) 2007, Stanford University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of Stanford University nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * =============================================================================
 *
 * CircuitRouter-SeqSolver.c
 *
 * =============================================================================
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"
#include "maze.h"
#include "router.h"
#include "lib/timer.h"
#include "lib/types.h"
#include <unistd.h>

enum param_types {
    PARAM_BENDCOST = (unsigned char)'b',
    PARAM_XCOST    = (unsigned char)'x',
    PARAM_YCOST    = (unsigned char)'y',
    PARAM_ZCOST    = (unsigned char)'z',
};

enum param_defaults {
    PARAM_DEFAULT_BENDCOST = 1,
    PARAM_DEFAULT_XCOST    = 1,
    PARAM_DEFAULT_YCOST    = 1,
    PARAM_DEFAULT_ZCOST    = 2,
};

/*bool_t global_doPrint = TRUE;*/
char* global_inputFile = NULL;
long global_params[256]; /* 256 = ascii limit */


/* =============================================================================
 * displayUsage
 * =============================================================================
 */
static void displayUsage (const char* appName){
    printf("Usage: %s [options]\n", appName);
    puts("\nOptions:                            (defaults)\n");
    printf("    b <INT>    [b]end cost          (%i)\n", PARAM_DEFAULT_BENDCOST);
    /*printf("    p          [p]rint routed maze  (false)\n");*/
    printf("    x <UINT>   [x] movement cost    (%i)\n", PARAM_DEFAULT_XCOST);
    printf("    y <UINT>   [y] movement cost    (%i)\n", PARAM_DEFAULT_YCOST);
    printf("    z <UINT>   [z] movement cost    (%i)\n", PARAM_DEFAULT_ZCOST);
    printf("    h          [h]elp message       (false)\n");
    exit(1);
}


/* =============================================================================
 * setDefaultParams
 * =============================================================================
 */
static void setDefaultParams (){
    global_params[PARAM_BENDCOST] = PARAM_DEFAULT_BENDCOST;
    global_params[PARAM_XCOST]    = PARAM_DEFAULT_XCOST;
    global_params[PARAM_YCOST]    = PARAM_DEFAULT_YCOST;
    global_params[PARAM_ZCOST]    = PARAM_DEFAULT_ZCOST;
}


/* =============================================================================
 * parseArgs
 * =============================================================================
 */
static int parseArgs (long argc, char* const argv[]){
    long i;
    long opt;
    int file_i=-1;

    opterr = 0;

    setDefaultParams();

    while ((opt = getopt(argc, argv, "hb:px:y:z:")) != -1) {
        switch (opt) {
            case 'b':
            case 'x':
            case 'y':
            case 'z':
                global_params[(unsigned char)opt] = atol(optarg);
                break;
            case 'p':
                /*global_doPrint = TRUE;*/
                break;
            case '?':
            case 'h':
            default:
                opterr++;
                break;
        }
    }

    for (i = optind; i < argc; i++) {
    	FILE* fp=NULL;
    	fp = fopen(argv[i], "r");
    	if (fp!=NULL){
    		fclose(fp);
    		if (file_i != -1){
      			return -1; /*More than one file crashes*/
    		}
    		file_i=i;
    	}
    	else{
        	fprintf(stderr, "Non-option argument: %s\n", argv[i]);
        	opterr++;
        }
    }
    if (file_i != -1) return file_i;

    if (opterr) {
    	return -1;
        displayUsage(argv[0]);
    }
    return -1;
}

/* =============================================================================
 * getOutputFile
 * =============================================================================
 */

FILE* getOutputFile(char* old_path){
	FILE* output=NULL;
	char *path = strdup(old_path), *path_aux;
	const char *res=".res", *old=".old";
	strcat(path, res); 			/*Creates path for the new file*/
	path_aux = strdup(path); 	/*To check if .old exists*/
	if (access(path, F_OK)==0){
		strcat(path_aux, old);
		if (access(path_aux, F_OK)==0) remove(path_aux);
		rename(path, path_aux);
	}
	output = fopen(path, "w");
	return output;

}

/* =============================================================================
 * main
 * =============================================================================
 */
int main(int argc, char** argv){
    /*
     * Initialization
     */
	FILE *file_input=NULL, *file_output=NULL;
	int file_i;

    file_i = parseArgs(argc, (char** const)argv);
    if (file_i<0 && argc!=2 && argc!=3) exit(1);
    else file_input = fopen(argv[file_i], "r");

    file_output = getOutputFile(argv[file_i]);

    maze_t* mazePtr = maze_alloc();
    assert(mazePtr);

    long numPathToRoute = maze_read(mazePtr, file_input, file_output);
    router_t* routerPtr = router_alloc(global_params[PARAM_XCOST],
                                       global_params[PARAM_YCOST],
                                       global_params[PARAM_ZCOST],
                                       global_params[PARAM_BENDCOST]);
    assert(routerPtr);
    list_t* pathVectorListPtr = list_alloc(NULL);
    assert(pathVectorListPtr);

    router_solve_arg_t routerArg = {routerPtr, mazePtr, pathVectorListPtr};
    TIMER_T startTime;
    TIMER_READ(startTime);

    router_solve((void *)&routerArg);

    TIMER_T stopTime;
    TIMER_READ(stopTime);

    long numPathRouted = 0;
    list_iter_t it;
    list_iter_reset(&it, pathVectorListPtr);
    while (list_iter_hasNext(&it, pathVectorListPtr)) {
        vector_t* pathVectorPtr = (vector_t*)list_iter_next(&it, pathVectorListPtr);
        numPathRouted += vector_getSize(pathVectorPtr);
	}
 	
	fprintf(file_output, "Paths routed    = %li\n", numPathRouted);
    fprintf(file_output, "Elapsed time    = %f seconds\n", TIMER_DIFF_SECONDS(startTime, stopTime));

    /*
     * Check solution and clean up
     */
    assert(numPathRouted <= numPathToRoute);

    bool_t status = maze_checkPaths(mazePtr, pathVectorListPtr, file_output); /*OUTPUT*/

    assert(status == TRUE);
    fprintf(file_output,"Verification passed.");

    maze_free(mazePtr);
    router_free(routerPtr);

    list_iter_reset(&it, pathVectorListPtr);
    while (list_iter_hasNext(&it, pathVectorListPtr)) {
        vector_t* pathVectorPtr = (vector_t*)list_iter_next(&it, pathVectorListPtr);
        vector_t* v;
        while((v = vector_popBack(pathVectorPtr))) {
            // v stores pointers to longs stored elsewhere; no need to free them here
            vector_free(v);
        }
        vector_free(pathVectorPtr);
    }
    list_free(pathVectorListPtr);

    fclose(file_input);
    fclose(file_output);
    exit(0);
}


/* =============================================================================
 *
 * End of CircuitRouter-SeqSolver.c
 *
 * =============================================================================
 */
