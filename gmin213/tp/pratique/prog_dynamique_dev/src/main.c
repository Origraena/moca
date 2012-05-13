

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
/*	exemple pour exo 7
	b = createBag(5,b);
	b->v = 13;
	b->items[0].v = 2;
	b->items[0].u = 3;
	b->items[1].v = 1;
	b->items[1].u = 1;
	b->items[2].v = 5;
	b->items[2].u = 8;
	b->items[3].v = 4;
	b->items[3].u = 1;
	b->items[4].v = 3;
	b->items[4].u = 5;
*/

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

/*	graph* g;// = generateGraph(3,1,10);
//	printGraph(g);
	path* p = 0;
/*	p = tsp(g);
	printPath(p);
	freePath(p);
	freeGraph(g);

	// ex prog dyn
/*	g = createGraph(5);
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
	freeGraph(g);*/

	// tests
//	for (i = 0 ; i < argc ; i++)
//		printf("arg[%i] = %s\n",i,argv[i]);
/*	if (argc > 4) {
		uint n = atoi(argv[2]);
		int min = atoi(argv[3]);
		int max = atoi(argv[4]);
		int points[n][2];
		int k = atoi(argv[1]);
		int x = 0;
		printf("Nb tests : %i\n",k);
		printf("Nb vertices : %i\n",n);
		printf("Min coordinate : %i\n",min);
		printf("Max coordinate : %i\n",max);
		for (k = atoi(argv[1]) ; k > 0 ; k--) {
			x++;
			printf("test %i begins...\n",x);
			randomizePoints(points,n,min,max,min,max);
			g = generateGeoGraph(n,points);
			p = tsp(g);
    //		printf("Vertices :\n");
    //		printPoints(points,n);
    //		printf("Edges :\n");
    //		printGraph(g);
    //		printf("TSP :\n");
    //		printPath(p);
			freePath(p);
			freeGraph(g);
			printf("test %i ends!\n",x);
		}
	}

/*	printf("Partition\n");
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
	int cpt1 = 0;
	int cpt2 = 0;
	if (result2) {
		for (i = 0 ; i < 2*nb2 ; i++) {
			printf("Item %i in partition %i\n",i,result2[i]);
			if (result2[i] == 0)
				cpt1+=weights2[i];
			else
				cpt2+=weights2[i];
		}
		printf("Partition 1 : %i\nPartition 2 : %i\n",cpt1,cpt2);
		free(result2);
	}
	else 
		printf("No solution\n");
*/
	return 0;
}

