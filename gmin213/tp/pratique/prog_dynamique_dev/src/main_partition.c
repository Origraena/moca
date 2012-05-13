

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "core.h"
#include "partition.h"

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
		return 0;
	}
	uint nMin,nMax,vMin,vMax;
	int* weights;
	int* sol;
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
		n = rand() % (nMax - nMin) + nMin + 1;
		weights = malloc(sizeof(int)*n);
		v = 0;
		for (j = 0 ; j < n ; j++) {
			weights[j] = rand() % (vMax - vMin) + vMin + 1;
			v += weights[j];
		}
		if (v % 2) {
			weights[0]++;
			v++;
		}
		timeS = time(0);
		sol = partition(weights,n);
		timeS = time(0)-timeS;
		printf("[TEST] %i : n = %i, p = %i (%li seconds)\n",i,n,v,timeS);
		fprintf(output,"%u %li %u\n",n,timeS,v);
		free(weights);
		free(sol);
		weights = 0;
		sol = 0;
	}
	fclose(output);

	return 0;
}

