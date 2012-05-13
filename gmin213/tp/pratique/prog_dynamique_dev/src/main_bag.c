

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "core.h"
#include "bag.h"

global_var global; 


int main(int argc, char** argv) {
	global.debug = 0;
	global.verbose = 0;
	uint i,j;
	const long seed = time(0);
	srand(seed);
	if (global.verbose)
		printf("seed = %li\n",seed);
	
	bag* b = 0;
	bag_sol* sol = 0;

	FILE* output;
	uint n,v,nb;
	long timeS;
	if (argc < 7) {
		printf("Argument error !\n");
		printf("syntax : %s filename n V\nn = number of items, V = bag volume\n",argv[0]);
		return 0;
	}
	uint nMin,nMax,vMin,vMax;
	nb = atoi(argv[2]);
	nMin = atoi(argv[3]);
	nMax = atoi(argv[4]);
	vMin = atoi(argv[5]);
	vMax = atoi(argv[6]);
	if (!(output = fopen(argv[1],"a"))) {
		perror("File error.");
		exit(EXIT_FAILURE);
	}
	const uint uMin = 0;
	const uint uMax = vMax;
	for (i = 0 ; i < nb ; i++) {
		n = rand() % (nMax - nMin) + nMin;
		v = rand() % (vMax - vMin) + vMin;
		b = generateBag(n,v,uMin,uMax,1,v,b);
		timeS = time(0);
		sol = bag_resolution(b);
		timeS = time(0)-timeS;
		printf("[TEST] %i : n = %i, v = %i (%li seconds)\n",i,n,v,timeS);
		fprintf(output,"%u %li %u\n",n,timeS,v);
		if (global.verbose) {
			printf("PROBLEM\n");
			printBag(b);
			printf("SOLUTION\n");
			printBagSol(b,sol);
		}
		freeBag(b);
		freeBagSol(sol);
		b = 0;
		sol = 0;
	}
	fclose(output);

	return 0;
}

