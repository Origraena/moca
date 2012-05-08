
#ifndef GRAPHS_H_
#define GRAPHS_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "core.h"

typedef struct graph {
	uint n;
	int** edges;
} graph;

typedef struct path {
	uint n;
	int length;
	uint capacity;
	uint* vertices;
} path;

/* random */
float randBetween(int min, int max);
float frand();
int distance(const int x1, const int y1, const int x2, const int y2);
void randomizePoints(int points[][2], const int n, const int minWidth, const int maxWidth, const int minHeight, const int maxHeight);
void printPoints(int points[][2],const int n);

/* graphs */
graph* createGraph(uint n);
graph* generateGraph(uint n, int minWeight, int maxWeight);
graph* generateGeoGraph(const uint n, int points[][2]);
void freeGraph(graph*);
void printGraph(graph* g);
void setEdge(graph* g, const uint u, const uint v, const int w);

/* paths */
path* createPath(uint capacity);
void freePath(path* p);
path* copyPath(path* src, path* dest);
path* addToPath(path* p, graph* g, uint u);
void printPath(path* p);

/* paths functions */
int contains(path* p, uint u);
uint closest(graph* g, uint u);
path* shortestChainRooted(graph* g, uint root);
path* shortestChain(graph* g);
path* shortestChainToCycle(graph* g);

/* prog dyn */
path* tsp(graph* g);

#endif

