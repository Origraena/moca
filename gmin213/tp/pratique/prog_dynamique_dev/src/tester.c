// test.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "core.h"

global_var global; 


int main(int argc, char** argv) {
	global.debug = 0;
	global.verbose = 0;
	uint i,j;
	const long seed = time(0);
	srand(seed);
	if (global.verbose)
		printf("seed = %li\n",seed);
	
	uint n,v,nb;
	uint nMin, nMax, vMin, vMax;
	const char* bin;
	const char* output;
	long timeS;
	
	if (argc < 4) {
		printf("Argument error !\n");
		printf("syntax : %s filename n V\nn = number of items, V = bag volume\n",argv[0]);
		return 0;
	}
	output = argv[1];
	nb = atoi(argv[2]);
	nMin = atoi(argv[3]);
	nMax = atoi(argv[4]);
	vMin = atoi(argv[5]);
	vMax = atoi(argv[6]);

	for (i = 0 ; i < nb ; i++) {
		n = rand() % (nMax - nMin) + nMin;
		v = rand() % (vMax - vMin) + vMin;

	}

	return 0;
}

