
#include "graphs.h"

float randBetween(int min, int max) {
	int rng = max - min;
	float r = rand() % rng;
	r += min;
	r += frand();
	return r;
}

float frand() {
	float r;
	r = (float) rand();
	r /= RAND_MAX;
	return r;
}

graph* createGraph(const uint n) {
	uint i;
	graph* g = malloc(sizeof(graph));
	g->n = n;
	g->edges = malloc(n*sizeof(int*));
	for (i = 0 ; i < n ; i++)
		g->edges[i] = malloc(n*sizeof(int));
	return g;
}

void setEdge(graph* g, const uint u, const uint v, const int w) {
	g->edges[u][v] = w;
	g->edges[v][u] = w;
}

graph* generateGraph(const uint n, const int minWeight, const int maxWeight) {
	uint i,j;
	graph* g = createGraph(n);
	for (i = 0 ; i < n ; i++) {
		g->edges[i][i] = 0;
		for (j = i+1 ; j < n ; j++) {
			g->edges[i][j] = randBetween(minWeight,maxWeight);
			g->edges[j][i] = g->edges[i][j];
		}
	}
	return g;
}

void randomizePoints(int points[][2], const int n, 
					 const int minWidth, const int maxWidth, 
					 int minHeight, const int maxHeight) {
	uint i;
	for (i = 0 ; i < n ; i++) {
		points[i][0] = rand()%(maxWidth-minWidth) + minWidth;
		points[i][1] = rand()%(maxHeight-minHeight) + minHeight;
	}	
}

graph* generateGeoGraph(const uint n, int points[][2]) {
	uint i,j;
	graph* g = createGraph(n);
	for (i = 0 ; i < n ; i++) {
		g->edges[i][i] = 0;
		for (j = i+1 ; j < n ; j++) {
			g->edges[i][j] = distance(points[i][0],points[i][1],points[j][0],points[j][1]);
			g->edges[j][i] = g->edges[i][j];
		}
	}
	return g;
}

int distance(const int x1, const int y1, const int x2, const int y2) {
	//return floor(sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
	return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
}

void printPoints(int points[][2], const int n) {
	uint i;
	for (i = 0 ; i < n ; i++)
		printf("%i = (%i,%i)\n",i,points[i][0],points[i][1]);
}

//int* operator[](graph* g, const int i) {
//	return g->edges[i];
//}

void freeGraph(graph* g) {
	uint i;
	if (g) {
		for (i = 0 ; i < g->n ; i++)
			free(g->edges[i]);
		free(g->edges);
		free(g);
	}
}

void printGraph(graph* g) {
	uint i,j;
	for (i = 0 ; i < g->n ; i++) {
		printf("(");
		for (j = 0 ; j < g->n-1 ; j++)
			printf("%i,",g->edges[i][j]);
		printf("%i)\n",g->edges[i][g->n-1]);
	}
}

path* createPath(uint capacity) {
	path* p = malloc(sizeof(path));
	p->n = 0;
	p->capacity = capacity;
	p->vertices = malloc(capacity*sizeof(uint));
	p->length = 0;
	return p;
}

void freePath(path* p) {
	if (p) {
		free(p->vertices);
		free(p);
	}
}

path* addToPath(path* p, graph* g, uint u) {
	if (!p)
		p = createPath(6);
	if (p->n >= p->capacity) {
		p->capacity += CAPACITY_FOOT;
		p->vertices = realloc(p->vertices,(p->capacity+CAPACITY_FOOT)*sizeof(uint));
	}
	p->vertices[p->n] = u;
	if (p->n > 0)
		p->length += g->edges[p->vertices[p->n-1]][u];
	p->n++;
	return p;
}

void printPath(path* p) {
	uint i;
	printf("size = %i\n",p->n);
	if (!p)
		return;
	for (i = 0 ; i < p->n-1 ; i++)
		printf("%i --> ",p->vertices[i]);
	if (p->n > 0)
		printf("%i\n",p->vertices[i]);
	printf("length = %i\n",p->length);
}

uint closest(graph* g, uint u) {
	uint i,v;
	int min = -1;
	for (i = 0 ; i < g->n ; i++) {
		if (i != u) {
			if ((min < 0) || (g->edges[u][i] < min)) {
				min = g->edges[u][i];
				v = i;
			}
		}
	}
	return v;
}

uint closestUncolored(graph* g, uint u, short colored[]) {
	uint i,v;
	int min = -1;
	for (i = 0 ; i < g->n ; i++) {
		if (!colored[i]) {
			if ((min < 0) || (g->edges[u][i] < min)) {
				min = g->edges[u][i];
				v = i;
			}
		}
	}
	return v;
}

path* shortestChainRooted(graph* g, uint root) {
	uint v,i;
	path* p;
	short colored[g->n];
	p = createPath(g->n+1);
	v = root;
	for (i = 0 ; i < g->n ; i++)
		colored[i] = 0;
	for (i = 0 ; i < g->n ; i++) {
		colored[v] = 1;
		p = addToPath(p,g,v);
		v = closestUncolored(g,v,colored);
	}
	return p;
}

path* shortestChain(graph* g) {
	uint i;
	path *pmin, *p;
	pmin = shortestChainRooted(g,0);
	for (i = 1 ; i < g->n ; i++) {
		p = shortestChainRooted(g,i);
		if (p->length < pmin->length) {
			freePath(pmin);
			pmin = p;
		}
		else
			freePath(p);
	}
	return pmin;
}

path* shortestChainToCycle(graph* g) {
	path* p = shortestChain(g);
	p = addToPath(p,g,p->vertices[0]);
	return p;
}

int contains(path* p, uint id) {
	uint i;
	for (i = 0 ; i < p->n ; i++) {
		if (p->vertices[i] == id)
			return i;
	}
	return -1;
}

path* copyPath(path* src, path* dest) {
	uint i;
	if (dest)
		freePath(dest);
	if (!src)
		return 0;
	dest = createPath(src->n);
	dest->n = src->n;
	dest->length = src->length;
	for (i = 0 ; i < src->n ; i++)
		dest->vertices[i] = src->vertices[i];
	return dest;
}

path* tsp(graph* g) {
	uint i,j,k;
	int l;
	path* C[g->n-1];
	path* Cprev[g->n-1];
	path* result;
	// init
	for (i = 1 ; i < g->n ; i++) {
		Cprev[i-1] = createPath(g->n);
		Cprev[i-1] = addToPath(Cprev[i-1],g,0);
		Cprev[i-1] = addToPath(Cprev[i-1],g,i);
		C[i-1] = 0;
	}
	// process
	for (i = 2 ; i < g->n ; i++) {
		for (j = 0 ; j < g->n-1 ; j++) {
			l = -1;
			for (k = 0 ; k < g->n-1 ; k++) {
				if ((k != j)
				&& (Cprev[k])
				&& ((Cprev[k]->length + g->edges[k+1][j+1] < l) || (l < 0))
				&& (contains(Cprev[k],j+1) < 0)) {
					if (C[j]) {
						freePath(C[j]);
						C[j] = 0;
					}
					C[j] = copyPath(Cprev[k],C[j]);
					C[j] = addToPath(C[j],g,j+1);
					l = C[j]->length;
				}
			}
		}
		for (j = 0 ; j < g->n-1 ; j++) {
			if (Cprev[j]) {
				freePath(Cprev[j]);
				Cprev[j] = 0;
			}
			Cprev[j] = C[j];
			C[j] = 0;
		}
	}
	// select result	
	result = 0;
	for (i = 1 ; i < g->n ; i++) {
		if (Cprev[i-1]) {
			Cprev[i-1] = addToPath(Cprev[i-1],g,0);
			if ((!result)
			|| (Cprev[i-1]->length < result->length))
				result = Cprev[i-1];
		}
	}
	// free memory
	for (i = 0 ; i < g->n-1 ; i++) {
		if ((Cprev[i]) && (Cprev[i] != result))
			freePath(Cprev[i]);
		if ((C[i]) && (C[i] != result))
			freePath(C[i]);
	}
	return result;
}

