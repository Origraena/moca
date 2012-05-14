
#include "graphs.h"

int contains2(solution_list* l, uint id) {
	if (!l)
		return 0;
	solution_list* current = l;
	while (current) {
		if (current->id == id)
			return 1;
		current = current->next;
	}	
	return 0;
}

int containsLess(solution_list* l, solution_list* src, uint id) {
	debug("contains Less ?");
	if (global.debug) {
		printf("id = %u\n",id);
		debug("l = ");
		printList(l);
		debug("src = ");
		printList(src);
	}
	if (!l)
		return 0;
	solution_list* current = src;
	while (current) {
		if (global.debug) {
			printf("[DEBUG] current id = %i\n",current->id);
		}
		if (current->id != id)
			if (!contains2(l,current->id))
				return 0;
		else
			if (contains2(l,id))
				return 0;
		current = current->next;
	}
	debug("returns true");
	return 1;
}

solution_list* addToList(solution_list* l, uint id) {
	solution_list* prevHead = 0;
	if (l)
		prevHead = l;
	l = 0;
	l = malloc(sizeof(solution_list));
	l->id = id;
	l->next = prevHead;
	return l;
}

void freeList(solution_list* l) {
	solution_list* current = l;
	solution_list* next = l->next;
	while (current) {
		free(current);
		current = next;
		if (current)
			next = current->next;
	}
}

solution_list* copyList(solution_list* src, solution_list* dest) {
	solution_list* current = src;
	if (!src)
		return dest;
	if (dest)
		freeList(dest);
	dest = malloc(sizeof(solution_list));
	solution_list* current2 = dest;
	while (current) {
	//	debug("copylist while");
		current2->id = current->id;
		current2->next = 0;
		if (current->next) {
			current2->next = malloc(sizeof(solution_list));
			current2 = current2->next;
		}
		current = current->next;
	}
	return dest;
}

void printList(solution_list* l) {
	if (!l) {
		printf("\n");
		return;
	}
	printf("%i,",l->id);
	return printList(l->next);
}

column_list* addToColumnList(column_list* l, column* c) {
	column_list* prevHead = 0;
	if (l)
		prevHead = l;
	l = 0;
	l = malloc(sizeof(column_list));
	l->c = c;
	l->next = prevHead;
	return l;
}

void freeColumnList(column_list* l) {
	if (!l)
		return;
	column_list* current = l;
	column_list* next = l->next;
	while (current) {
		freeColumn(current->c);
		free(current);
		current = next;
		if (current)
			next = current->next;
	}
}

column_list* copyColumnList(column_list* src, column_list* dest) {
	column_list* current = src;
	if (dest)
		freeColumnList(dest);
	dest = malloc(sizeof(column_list));
	column_list* current2 = dest;
	while (current) {
		current2->c = current->c;
		current = current->next;
		if (current) {
			current2->next = malloc(sizeof(column_list));
			current2 = current2->next;
		}
	}
	return dest;
}

column* createColumn(column* l, const uint n) {
	uint i;
	if (!l)
		l = malloc(sizeof(column));
	l->n = n;
	l->p = malloc(sizeof(path*)*n);
	for (i = 0 ; i < n ; i++)
		l->p[i] = 0;
	l->s = 0;
	return l;
}

column* addToColumn(column* l, uint id) {
	if (!l)
		return 0;
	l->s = addToList(l->s,id);
	return l;
}

void freeColumn(column* l) {
	uint i;
	if (!l)
		return;
	freeList(l->s);
	for (i = 0 ; i < l->n ; i++) {
		freePath(l->p[i]);
		l->p[i] = 0;
	}
	free(l->p);
	l->p = 0;
	l->s = 0;
}


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

void fprintGraph(graph* g, const char* filename) {
	uint i,j;
	FILE* output = fopen(filename,"w");
	if ((!output) || (!g)) {
		perror("Opening file");
		return;
	}
	fprintf(output,"%u\n",g->n);
	for (i = 0 ; i < g->n ; i++) {
		for (j = i+1 ; j < g->n ; j++)
			fprintf(output,"%d ",g->edges[i][j]);
		fprintf(output,"\n");
	}
	fclose(output);
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
	if (!p)
		return;
	printf("size = %i\n",p->n);
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
	uint i,j,k,l;
	column_list* Cprev = 0;
	column_list* C = 0;
	solution_list* s = 0;
	s = addToList(s,0);
	column* current = 0;
	// init
	debug("TSP initializing...");
	for (i = 1 ; i < g->n ; i++) {
		current = createColumn(current,g->n-1);
		current->s = copyList(s,current->s);
		current = addToColumn(current,i);
		for (j = 1 ; j < g->n ; j++) {
			current->p[j-1] = 0;
			if (j != i) {
				current->p[j-1] = createPath(g->n);
				current->p[j-1] = addToPath(current->p[j-1],g,0);
				current->p[j-1] = addToPath(current->p[j-1],g,i);
				current->p[j-1] = addToPath(current->p[j-1],g,j);
				if (global.debug) {
					printf("[DEBUG] i = %u, j = %u\n",i,j);
					printPath(current->p[j-1]);
				}
			}
		}
		C = addToColumnList(C,current);
		current = 0;
	}
	debug("TSP initialized!");
	// process
	column_list* currentCol = 0;
	column_list* currentCol2 = 0;
	for (k = 2 ; k < (g->n-1) ; k++) {
		if (global.debug) {
			printf("[DEBUG] k = %u\n",k);
		}
		// copy C into Cprev
		if (Cprev)
			freeColumnList(Cprev);
		Cprev = C;
		currentCol = Cprev;
		C = 0;
		while (currentCol) {
			//debug("current col ++");
			for (i = currentCol->c->s->id+1 ; i < g->n ; i++) {
				current = 0;
				current = createColumn(current,g->n-1);
				current->s = copyList(currentCol->c->s,current->s);
				current->s = addToList(current->s,i);
				if (global.debug) {
					debug("current s ");				
					printList(current->s);
				}
		//		debug("processing column...");	
				// process column
				for (j = 1 ; j < g->n ; j++) {
					for (l = 1 ; l < g->n ; l++) {
						if (global.debug) {
							printf("[DEBUG] i = %u, j = %u, l = %u\n",i,j,l);
						}
						currentCol2 = Cprev;
						while (currentCol2) {
							if (global.debug) {
								debug("currentCol2->c->s");
								printList(currentCol2->c->s);
								debug("current->s");
								printList(current->s);
							}
							if (containsLess(currentCol2->c->s,current->s,l)) {
								if ((currentCol2->c->p[l-1])
								&& ((!current->p[j-1])
								|| (currentCol2->c->p[l-1]->length + g->edges[l][j] < current->p[j-1]->length))) {
									debug("new value found");
									if (current->p[j-1]) {
										freePath(current->p[j-1]);
										current->p[j-1] = 0;
									}
									current->p[j-1] = copyPath(currentCol2->c->p[l-1],current->p[j-1]);
									current->p[j-1] = addToPath(current->p[j-1],g,j);
								}
							}
				//			debug("currentCol2++");
							currentCol2 = currentCol2->next;
						}
					}
					if (global.debug) {
						printf("current = \n");
						printPath(current->p[j-1]);
					}
				}
			//	debug("column processed!");
				C = addToColumnList(C,current);
			}
			debug("column list processed!");
			currentCol = currentCol->next;
			debug("column list processed2!");
		}
	}
	debug("processed!");
	freeColumnList(Cprev);
	debug("Cprev freed!");
	path* solution = 0;
	for (l = 1 ; l < g->n ; l++) {
		if (global.debug) {
			printf("l = %u\n",l);
			printf("exists = %i\n",(int)C->c->p[l-1]);
		}
		if ((C->c->p[l-1])
		&& ((!solution)
		|| (C->c->p[l-1]->length + g->edges[l][0] < solution->length))) {
			debug("min found");
			if (solution) {
				debug("solution already exists");
				freePath(solution);
				solution = 0;
			}
			solution = copyPath(C->c->p[l-1],solution);
			solution = addToPath(solution,g,0);		
			debug("ok!");	
			if (global.debug)
				printPath(solution);
		}
	}
	debug("solution found!");
	if (global.debug)
		printPath(solution);
	debug("solution!");
	if (C)
		freeColumnList(C);
	return solution;
}

