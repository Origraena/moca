#include "partition.h"

int* partition(int weights[], const int n) {
	int wMax = 0;
	uint i,j;
	for (i = 0 ; i < n ; i++)
		wMax += weights[i];
	if (wMax % 2)
		return 0;
	wMax /= 2;
	int* T[n];
	for (i = 0 ; i < n ; i++)
		T[i] = malloc(sizeof(int)*(wMax+1));
	// init
	for (j = 0 ; j <= wMax ; j++) {
		T[0][j] = false;
		if (j == 0)
			T[0][j] = true;
	}
	// process
	for (i = 1 ; i < n ; i++) {
		for (j = 0 ; j <= wMax ; j++) {
			T[i][j] = ((j == 0)
					|| (j == weights[i]) 
					|| (T[i-1][j]) 
					|| ((j - weights[i] >= 0) && (T[i-1][j-weights[i]])));
		}
	}
	// result
	int* sol = malloc(sizeof(int)*n);
	for (i = 0 ; i < n ; i++)
		sol[i] = 0;
	if (T[n-1][wMax])
		sol = partitionSol(T,weights,n,n,wMax,sol);
	else {
		free(sol);
		sol = 0;
	}
	for (i = 0 ; i < n ; i++)
		free(T[i]);
	return sol;
}

int* partitionSol(int** T, int weights[], const int n, int i, int j, int* sol) {
	if ((i == 0) || (j == 0))
		return sol;
	if (T[i-1][j])
		return partitionSol(T,weights,n,i-1,j,sol);
	sol = partitionSol(T,weights,n,i-1,j-weights[i],sol);
	sol[i] = 1;
	return sol;
}

