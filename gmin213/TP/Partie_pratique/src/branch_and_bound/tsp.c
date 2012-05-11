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
	int nb_visited = 0, *visited = (int *) calloc (t->nb_node, 0);
	int **mat = (int **) malloc (sizeof(int *) * t->nb_node), i;

	for (i=0; i<t->nb_node; mat[i++] = (int *) calloc (t->nb_node, sizeof(int)));
	for (i=0; i<t->nb_node; mat[t->sol[i]/t->nb_node][t->sol[i++]%t->nb_node]++);

	int node = 0, finchemin

	for (i=0; i<t->nb_node; i++) 
		if (t->mat[node][i]) {
			nb_visited ++; visited[node]++;
			node = i;
			break;
		}

	if (!node) {
		for (i=0; i<t->nb_node; i++)
			free(mat[i]);
		free (mat);
		return 0;
	}

	while (nb_visited < t->nb_node) {
		for (i=0; i<t->nb_node; i++)
			if (mat[node][i]) {

			}
	}
}

void copyData (void *d1, void *d2);
void freeData (void *d);

int lightestString (void tsp);
int opt2 (void * tsp);
int opt3 (void * tsp);


