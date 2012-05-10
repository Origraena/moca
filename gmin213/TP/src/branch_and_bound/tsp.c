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

gpointer hungAlgo (const gpointer tsp, gpointer tsp_sol);

gpointer lightestString (const gpointer tsp, gpointer tsp_sol);
gpointer opt2 (const gpointer tsp, gpointer tsp_sol);
gpointer opt3 (const gpointer tsp, gpointer tsp_sol);

