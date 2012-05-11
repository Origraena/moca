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

void initTSPFromFile (tsp_t *tsp, FILE *in) {
	// Read value in a formatted file
}

int validTSP (const gpointer tspsol) {
	int i, j;
	tsp_t *t = (tsp_t *) tspsol;

	// Check the number of outcoming and outgoing edges
	for (i=0; i<t->nb_node; i++) {
		int sum_in =0, sum_out = 0;
		for (j=0; j<t->nb_node; j++){
			sum_out += t->mat[i][j];
			sum_in += t->mat[j][i];
			if (sum_in > 1 || sum_out > 1)
				return 0;
		}
		if (sum_in < 1 || sum_out < 1)
			return 0;
	}

	// Check if there are many cycles
	int *visited = (int *) calloc (t->nb_node, sizeof(int)), nb_vis = 0;
	i = 0;
	while (!visited[i]) {
		visited[i] ++;
		for (j=0; j<t->nb_node; j++)
			if (t->mat[i][j]){
				i = j;
				nb_vis ++;
				break;
			}
	}

	if (nb_vis != t->nb_node)
		return 0;

	return 1;
}

int bestSol (const gpointer val1, const gpointer val2) {
	int *a = (int *)val1, *b = (int *)val2;
	return (*a - *b);
}

gpointer hungAlgo (const gpointer tsp, gpointer tsp_sol) {
	tsp *t = (tsp *) tsp;
	tsp *ts = (tsp *) tsp_sol;
	// Make a copy of the matrix problem
	int i, j;
	for (i=0, i<t->nb_node; memset (ts->mat[i], 0, sizeof(int) * t->nb_node));

	int *u = (int *) malloc (sizeof (int) * ts->nb_node);
	int *p = (int *) malloc (ts->nb_node * sizeof(int));
	int *v = (int *) calloc (sizeof (int) * ts->nb_node);
	int *d = (int *) malloc (sizeof (int) * ts->nb_node);
	int *m = (int *) malloc (sizeof (int) * ts->nb_node);
	int *mate =(int *) malloc (ts->nb_node * sizeof (int));
	GList *Q = g_list_alloc();

	for (i=0; i<ts->nb_node; i++) {
		mate[i] = -1;
		i[u]=t->mat[i][0];
		for (j=1; j<t->nb_node; i[u] = t->mat[i][j] > i[u] ? t->max[i][j++] : i[u]);
	}

	int nrex = ts->nb_node;
	while (nrex) {
		for (i=0; i<ts->nb_node; m[i] = 0, p[i] = 0, d[i++] = -1);
		int aug = 0;
		for (i=0; i<ts->nb_node; i++)
			if (mate[i] == -1)
				Q = g_list_append (Q, GINT_TO_POINTER(i));

		do {
			int k = GPOINTER_TO_INT(g_list_first(Q)->data);
			Q = g_list_remove(Q, GINT_TO_POINTER(k));
			m(k) = 1;
			j = 0;
			while (!aug && j < ts->nb_node) {
				if (mate(k) != j) {
					if (u(j) + v(j) - t->mat[k][j] < d[j] || d[j] == -1) {
						d[j] = u[j] + v[j] - t->mat[k][j];
						p[j] = k;
						if (!d[j]){
							if (mate[j] == -1) {
								do {
									i = p[j];
									mate[j] = i;
									int next = mate[i];
									mate[i] = j;
									if (next == -1)
										j = next;
								} while (next == -1);
								aug = 1;
								nrex --;
							}
							else 
								Q = g_list_append (Q, GINT_TO_POINTER(mate[j]));
						}
					}
				}
				j = 
			}
		}
	}
}

gpointer lightestString (const gpointer tsp, gpointer tsp_sol);
gpointer opt2 (const gpointer tsp, gpointer tsp_sol);
gpointer opt3 (const gpointer tsp, gpointer tsp_sol);

