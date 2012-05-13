/*
 * =====================================================================================
 *
 *       Filename:  tsp.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2012 16:23:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tsp.h"

static inline int nbvoisin (tsp_t *t, int s) {
	int vois = 0, i;
	for (i=0; i<t->nb_node; i++)
		if (t->mat[s][i] > 0)
			vois ++;
	return vois;
}

void createEmptyTSP (tsp_t **tsp, int n) {
	tsp_t *a = (tsp_t *) malloc (sizeof(tsp_t));
	a->nb_node = n;
	a->sol = (int *) calloc (n, sizeof(int));
	a->mat = (int **) malloc (n*sizeof(int *));
	int i;
	for (i=0; i<n; a->mat[i] = (int *) malloc (n*sizeof(int)), i++);
	*tsp = a;
}

int findACPM (tsp_t *t) {
	char *mark = (char *) malloc (t->nb_node * sizeof(char));
	char *src = (char *) malloc (t->nb_node * sizeof(char));
	char *visited = (char *) calloc (t->nb_node, sizeof(char));

	int curnode = 0, nb_marked = 1, i;
	int next[2] = {-1};

	while (curnode < t->nb_node && !nbvoisin(t, curnode)) {
		curnode ++;
		nb_marked ++;
	}

	for (i=0; i<t->nb_node; i++)
		mark[i] = -1;

	while (nb_marked < t->nb_node && curnode != -1) {
		next[0] = -1;
		next[1] = -1;

		for (i=0; i<t->nb_node; i++){
			if (!visited[i]) {
				if (t->mat[curnode][i] != -1) {
					if (next[0] == -1){
						next[0] = t->mat[curnode][i];
						next[1] = i;
					}
					if (t->mat[curnode][i] < mark[i] || mark[i] == -1) {
						src[i] = curnode;
						mark[i] = t->mat[curnode][i];
					}
					if (mark[i] < next[0]) {
						next[0] = mark[i];
						next[1] = i;
					}
				}
			}
		}

		visited[curnode] ++;
		if (next[1] != -1)
			t->sol[nb_marked -1] = src[next[1]] * t->nb_node + next[1];
		curnode = next[1];
		nb_marked++;
	}
	free(visited);
	free(src);
	free(mark);

	return nb_marked == t->nb_node ? 1 : 0;
}

void initTSPFromFile (tsp_t **tsp, FILE *in) {
	tsp_t *t = (tsp_t *) malloc (sizeof(tsp_t));
	int i, j, n;

	fscanf(in, "%d\n", &n);
	t->nb_node = n;

	t->sol = (int *) calloc (n, sizeof(int));
	t->mat = (int **) malloc (n * sizeof(int *));

	for (i=0; i<t->nb_node; t->mat[i++] = (int *) malloc (n * sizeof(int)));

	for (i=0; i<t->nb_node; i++) {
		for (j=i+1; j<t->nb_node; j++) {
			fscanf (in, "%d ", &(t->mat[i][j]));
			t->mat[j][i] = t->mat[i][j];
		}
		t->mat[i][i] = -1;

		fscanf (in, "\n");
	}
	*tsp = t;
}

int compCurVal (void *s) {
	tsp_t *t = (tsp_t *) s;
	int value = 0, i;
	for (i=0; i<t->nb_node; i++) 
		value += t->mat[t->sol[i]/t->nb_node][t->sol[i]%t->nb_node] > 0 ? t->mat[t->sol[i]/t->nb_node][t->sol[i]%t->nb_node] : 0;

	return value;
}

int stratBranch (void *branchpoint, void **newbranch, size_t *size) {
	tsp_t *bp = (tsp_t *)branchpoint;
	*size = sizeof (tsp_t *);

	// Find number of children
	int *degre = (int *) calloc (bp->nb_node, sizeof (int)), i, j;
	for (i=0; i<bp->nb_node; i++) {
		degre[bp->sol[i]/bp->nb_node]++;
		degre[bp->sol[i]%bp->nb_node]++;
	}

	int max = 0, ind_max = 0;
	for (i=0; i<bp->nb_node; i++)
		if (degre[i] > max) {
			max = degre[i];
			ind_max = i;
		}

	tsp_t **nb = (tsp_t **) malloc (max * sizeof(tsp_t *));
	tsp_t *tmp;
	for (i=0; i< max; i++) {
		tmp = (tsp_t *) malloc (sizeof(tsp_t));
		tmp->nb_node = bp->nb_node;
		tmp->sol = (int *) malloc (bp->nb_node * sizeof(int));
		tmp->mat = (int **) malloc (sizeof(int*) *bp->nb_node);
		for (j=0; j<bp->nb_node; tmp->mat[j++] = (int *) malloc (sizeof(int)*bp->nb_node));
		copyData(tmp, bp);
		printTSP (tmp);
		printTSP (bp);
		nb[i] = tmp;
	}
	*newbranch = nb;
	free (degre);
//	free(nb);

	return max;
}

int acceptableSol (void *data) {
	tsp_t *t = (tsp_t *) data;

	int *node_deg = (int *) calloc (t->nb_node, sizeof(int)), i;

	for (i=0; i<t->nb_node; i++)
		if (++node_deg[t->sol[i]/t->nb_node] > 2 || ++node_deg[t->sol[i]%t->nb_node] > 2) {
			free (node_deg);
			return 0;
		}

	free(node_deg);
	return 1;
}

void copyData (void *d1, void *d2) {
	tsp_t *tdest = (tsp_t *) d1, *tsrc = (tsp_t *) d2;
	int i, j;
	tdest->nb_node = tsrc->nb_node;

	for (i=0; i<tsrc->nb_node; i++) {
		tdest->sol[i] = tsrc->sol[i];
		for (j=0; j<tsrc->nb_node; tdest->mat[i][j] = tsrc->mat[i][j++]);
	}
}

void freeData (void *d) {
	int i=0;
	tsp_t *t = (tsp_t *) d;

	for (i=0; i<t->nb_node; free(t->mat[i]), i++);

	free(t->mat);
	free(t->sol);
//	free(t);
}

int lightestString (void *data) {
	tsp_t *t = (tsp_t *) data;
	char *visited = (char *) calloc (t->nb_node, sizeof(char));
	int i, nb_visited = 1, curnode = 0, sum = 0;
	visited[0]++;

	while (nb_visited < t->nb_node && curnode != -1) {
		int min = -1, next = -1;
		for (i=1; i<t->nb_node; i++) {
			if (!visited[i]) {
				if (min == -1){
					min = t->mat[curnode][i];
					next = i;
				}
				else if (t->mat[curnode][i] < min) {
					min = t->mat[curnode][i];
					next = i;
				}
			}
		}
		t->sol[nb_visited -1] = curnode*t->nb_node + next;
		if (min != -1)
			sum+=min;
		curnode = next;
		visited[curnode]++;
		nb_visited++;
	}

	free(visited);

	if (curnode != -1) {
		t->sol[t->nb_node-1] = curnode * t->nb_node;
		sum += t->mat[curnode][0];
		return sum;
	} 
		
	fprintf (stderr, "Erreur\n");
	return -1;
}
int opt2 (void *data) {}
int opt3 (void *data) {}

void printTSP (tsp_t *t) {
	printf ("Nombre de noeuds : %d\n", t->nb_node);
	int i, j;
	printf ("Graphe : \n");
	for (i=0; i<t->nb_node; i++, printf ("\n"))
		for(j=0; j<t->nb_node; printf ("%d ", t->mat[i][j++]));
	printf ("Solution :\n");
	for (i=0; i<t->nb_node; i++)
		printf ("%d -- %d\n", t->sol[i]/t->nb_node, t->sol[i]%t->nb_node);
}

