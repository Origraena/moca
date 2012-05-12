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

}

int compCurVal (void *s) {
	tsp_t *t = (tsp_t *) s;
	int value = 0, i;
	for (i=0; i<t->nb_node; i++) 
		value += t->mat[t->sol[i]/t->nb_node][t->sol[i]%t->nb_node];

	return value;
}

int stratBranch (void *branchpoint, void **newbranch, size_t *size) {

	return 0;
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
	int i;
	tsp_t *t = (tsp_t *) data;

	for (i=0; i<t->nb_node; free(t->mat[i++]));
	free(t->sol);
	free(t);
}

int lightestString (void *data) {}
int opt2 (void *data) {}
int opt3 (void *data) {}


