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

void allocMem (void **a, void *b) {
	tsp_t *t1, *t2 = (tsp_t *) b;
	createEmptyTSP(&t1, t2->nb_node);
	*a = t1;
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

	int curnode = 1, nb_marked = 1, i;
	int next[2] = {-1};
	int mark_sol = 0;
	int rate = 0;

	for (i=0; i<t->nb_node; i++) {
		mark[i] = -1;
		t->sol[i] = -1;
	}

	visited[0]++;

	while (nb_marked < t->nb_node && !rate) {
		visited[curnode] ++;
		nb_marked++;
		// Update Cost of edges
		for (i=0; i<t->nb_node; i++) 
			if (!visited[i]) 
				if (t->mat[curnode][i] != -1 && (t->mat[curnode][i] < mark[i] || mark[i] == -1)) {
					mark[i] = t->mat[curnode][i];
					src[i] = curnode;
				}

		next[0] = -1;
		next[1] = -1;
		for (i=0; i<t->nb_node; i++) 
			if (!visited[i]) 
				if (mark[i] != -1) 
					if (next[0] == -1 || mark[i] < next[0]) {
						next[0] = mark[i];
						next[1] = i;
					}

		if (next[1] == -1 || next[1] == 0) 
			++rate;
		else
			t->sol[mark_sol++] = src[next[1]] * t->nb_node + next[1];

		curnode = next[1];
	}
	free(src);
	free(mark);

	for (i=0; i<t->nb_node; i++)
		if (!visited[i]) {
			free(visited);
			return 0;
		}

	free(visited);
	return 1;
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

	if (max <= 2) {
		free (degre);
		return 0;
	}

	int *nodes = (int *) malloc (sizeof(int) * max);
	int zz = 0;
	for (i=0; i<bp->nb_node; i++) {
		if (bp->sol[i]/bp->nb_node == ind_max)
			nodes[zz++] = bp->sol[i]%bp->nb_node;
		else if (bp->sol[i]%bp->nb_node == ind_max)
			nodes[zz++] = bp->sol[i]/bp->nb_node;
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
		tmp->mat[nodes[i]][ind_max] = -1;
		tmp->mat[ind_max][nodes[i]] = -1;
		if (compPartSolFromACPM(tmp, 0)) 
			nb[i] = tmp;
		else {
			i--;
			max--;
			freeData(tmp);
		}
	}
	*newbranch = nb;
	free (degre);
	free(nodes);
	//	free(nb);

	return max;
}

int acceptableSol (void *data) {
	tsp_t *t = (tsp_t *) data;

	int *node_deg = (int *) calloc (t->nb_node, sizeof(int)), i;

	for (i=0; i<t->nb_node; i++){
		if (t->sol[i] == -1){
			free (node_deg);
			return 0;
		}
		if (++node_deg[t->sol[i]/t->nb_node] > 2 || ++node_deg[t->sol[i]%t->nb_node] > 2) {
			free (node_deg);
			return 0;
		}
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
	free(t);
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

int opt2 (void *data) {
	int res = lightestString(data);
	tsp_t *t = (tsp_t *) data;
	printTSP(t);
	int i, j, better = 1;
	int sub;
	while (better) {
		better = 0;
		for (i=0; i<t->nb_node; i++) {
			for (j=0; j<t->nb_node; j++)
				if (j!=i) {
					int k = 0;
					int jsrc = t->sol[j]/t->nb_node, jdest = t->sol[j]%t->nb_node;
					int isrc = t->sol[i]/t->nb_node, idest = t->sol[i]%t->nb_node;
					if (jsrc == isrc || jsrc == idest || jdest == isrc || jdest == idest)
						continue;
					for (k=0; k<t->nb_node; k++) {
						if (t->sol[k] == isrc*t->nb_node + jdest || t->sol[k] == isrc*t->nb_node + jsrc || t->sol[k] == jsrc*t->nb_node + idest || t->sol[k] == jsrc*t->nb_node + isrc || t->sol[k] == idest*t->nb_node + jsrc || t->sol[k] == idest*t->nb_node + jdest || t->sol[k] == jdest*t->nb_node + isrc || t->sol[k] == jdest*t->nb_node + idest){
							k=-1;
							break;
						}
					}
					if (k==-1)
						continue;
					sub = t->mat[isrc][idest] + t->mat[jsrc][jdest] > t->mat[isrc][jdest] + t->mat[jsrc][jdest];
					if(sub > 0) {
						t->sol[i] = isrc*t->nb_node + jdest;
						t->sol[j] = jsrc*t->nb_node + idest;
						better++;
						res -= sub;
						printf ("%d %d\n", res, sub);
						printTSP(t);
					}
				}
		}
	}
	printf ("Fin init\n");
	return res;
}

int opt3 (void *data) {}

void printTSP (void *s) {
	tsp_t *t = (tsp_t *)s;
	printf ("Nombre de noeuds : %d\n", t->nb_node);
	int i, j;
	printf ("Graphe : \n");
	for (i=0; i<t->nb_node; i++, printf ("\n"))
		for(j=0; j<t->nb_node; printf ("%d ", t->mat[i][j++]));
	printf ("Solution :\n");
	for (i=0; i<t->nb_node; i++)
		printf ("%d -- %d\n", t->sol[i]/t->nb_node, t->sol[i]%t->nb_node);
}

int compPartSolFromACPM (void *data, int a) {
	int i;
	tsp_t *t = (tsp_t *) data, *s;
	createEmptyTSP(&s, t->nb_node);
	copyData(s, t);

	for (i=0; i<t->nb_node; t->mat[i][a] = -1, t->mat[a][i++] = -1);
	int ret = findACPM(t);

	int mina = -1, minb = -1, inda = -1, indb = -1;

	for (i=0; i<t->nb_node; i++) {
		t->mat[i][a] = s->mat[i][a];
		t->mat[a][i] = s->mat[a][i];
		if (mina == -1) {
			mina = s->mat[a][i];
			inda = i;
		} else if (minb == -1) {
			minb = s->mat[a][i];
			indb = i;
		}
		if (s->mat[a][i] > 0) {
			if (s->mat[a][i] < mina) {
				minb = mina;
				indb = inda;
				mina = s->mat[a][i];
				inda = i;
			} else if (s->mat[a][i] < minb) {
				indb = i;
				minb = s->mat[a][i];
			}
		}
	}

	t->sol[t->nb_node -1] = a*t->nb_node + inda;
	t->sol[t->nb_node -2] = a*t->nb_node + indb;

	freeData(s);
	if (!ret)
		return 0;
	return (compCurVal(t));
}

int initData (void *s) {
	return compPartSolFromACPM(s, 0);
}
