

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "partition.h"
#include "graphs.h"


int main(int argc, char** argv) {
	uint i,j;
	const long seed = time(0);
	srand(seed);
	graph* g = generateGraph(3,1,10);
	printGraph(g);
	path* p = 0;
	p = tsp(g);
	printPath(p);
	freePath(p);
	freeGraph(g);

	// ex prog dyn
	g = createGraph(5);
	for (i = 0 ; i < 5 ; i++)
		setEdge(g,i,i,0);
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

	// tests
	for (i = 0 ; i < argc ; i++)
		printf("arg[%i] = %s\n",i,argv[i]);
	if (argc > 4) {
		uint n = atoi(argv[2]);
		int min = atoi(argv[3]);
		int max = atoi(argv[4]);
		int points[n][2];
		int k;
		for (k = atoi(argv[1]) ; k >= 0 ; k--) {
			randomizePoints(points,n,min,max,min,max);
			g = generateGeoGraph(n,points);
			p = tsp(g);
    		printf("Vertices :\n");
    		printPoints(points,n);
    		printf("Edges :\n");
    		printGraph(g);
    		printf("TSP :\n");
    		printPath(p);
			freePath(p);
			freeGraph(g);
		}
	}

	printf("Partition\n");
	const uint nb = 4;
	int weights[nb];
	for (i = 0 ; i < nb ; i++) {
		weights[i] = i;
	}
	printf("Result :\n");
	int* result = partition(weights,nb);
	if (result) {
		for (i = 0 ; i < nb ; i++)
			printf("Item %i in partition %i\n",i,result[i]);
		free(result);
	}
	else 
		printf("No solution\n");
		
	const uint nb2 = 12;
	int weights2[2*nb2];
	for (i = 0 ; i < nb2 ; i++) {
		weights2[i] = i;
		weights2[i+nb2] = i;
	}
	printf("Result :\n");
	int* result2 = partition(weights2,2*nb2);
	if (result2) {
		for (i = 0 ; i < 2*nb2 ; i++)
			printf("Item %i in partition %i\n",i,result2[i]);
		free(result2);
	}
	else 
		printf("No solution\n");


	printf("\nseed = %li\n",seed);
	return 0;
}

