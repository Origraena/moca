

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "core.h"
#include "graphs.h"

global_var global; 

int main(int argc, char** argv) {
	global.debug = 0;
	global.verbose = 0;
	uint i,j;
	const long seed = time(0);
	srand(seed);
	if (global.verbose)
		printf("seed = %li\n",seed);
	

	FILE* output;
	uint n,v,nb;
	long timeS;
	if (argc < 7) {
		printf("Argument error !\n");
		printf("syntax : %s filename n V\nn = number of items, V = bag volume\n",argv[0]);
		return 0;
	}
	uint nMin,nMax,vMin,vMax;
	graph* g = 0;
	path* p = 0;
	nb = atoi(argv[2]);
	nMin = atoi(argv[3]);
	nMax = atoi(argv[4]);
	vMin = atoi(argv[5]);
	vMax = atoi(argv[6]);
	if (!(output = fopen(argv[1],"a"))) {
		perror("File error.");
		exit(EXIT_FAILURE);
	}
	for (i = 0 ; i < nb ; i++) {
		n = rand() % (nMax - nMin) + nMin;
		g = generateGraph(n,vMin,vMax);
		timeS = time(0);
		p = tsp(g);
		timeS = time(0)-timeS;
		printf("[TEST] %i : n = %i (%li seconds)\n",i,n,timeS);
		fprintf(output,"%u %li\n",n,timeS,v);
		freeGraph(g);
		freePath(p);
		g = 0;
		p = 0;
	}
	fclose(output);
	
/*	// ex prog dyn
	g = createGraph(5);
	for (i = 0 ; i < 5 ; i++)
		setEdge(g,i,i,50);
	setEdge(g,0,1,1);
	setEdge(g,0,2,2);
	setEdge(g,0,3,1);
	setEdge(g,0,4,0);
	setEdge(g,1,2,3);
	setEdge(g,1,3,5);
	setEdge(g,1,4,0);
	setEdge(g,2,3,2);
	setEdge(g,2,4,1);
	setEdge(g,3,4,4);
	printGraph(g);
	p = tsp(g);
	printPath(p);
	freePath(p);
	freeGraph(g);
*/
	return EXIT_SUCCESS;
}

