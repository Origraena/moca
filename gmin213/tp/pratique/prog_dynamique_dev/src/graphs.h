
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

typedef struct solution_list {
	uint id;
	struct solution_list* next;
} solution_list;

typedef struct column {
	uint n;	
	solution_list* s;
	path** p;
} column;

typedef struct column_list {
	column* c;
	struct column_list* next;
} column_list;

/* solution_list */
solution_list* addToList(solution_list* l, uint id);
int contains2(solution_list* l, uint id);
void freeList(solution_list* l);
solution_list* copyList(solution_list* src, solution_list* dest);
void printList(solution_list* l);

column_list* addToColumnList(column_list* l, column* c);
void freeColumnList(column_list* l);
column_list* copyColumnList(column_list* src, column_list* dest);

column* createColumn(column* l, const uint n);
column* addToColumn(column* l, uint id);
void freeColumn(column* l);

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
void fprintGraph(graph* g, const char* filename);
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

