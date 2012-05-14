

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
	sleep(1);
	const long seed = time(0);
	srand(seed);
	if (global.verbose)
		printf("seed = %li\n",seed);
	
	FILE* output;
	uint n,v,nb;
	long timeS;
	if (argc < 6) {
		printf("Argument error !\n");
		printf("syntax : %s filename n V\nn = number of items, V = bag volume\n",argv[0]);
		return 0;
	}
	uint nMin,nMax,vMin,vMax;
	graph* g = 0;
	path* p = 0;
	nMin = atoi(argv[2]);
	nMax = atoi(argv[3]);
	vMin = atoi(argv[4]);
	vMax = atoi(argv[5]);
	n = rand() % (nMax - nMin) + nMin;
	g = generateGraph(n,vMin,vMax);
	fprintGraph(g,argv[1]);
	freeGraph(g);
	freePath(p);
	g = 0;
	p = 0;
	
	return EXIT_SUCCESS;
}

