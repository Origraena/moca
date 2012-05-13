/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2012 16:48:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tsp.h"
#include "bb.h"

int main (int argc, char **argv) {
	tsp_t *t, *tmp;
	FILE *in = fopen ("graph", "r");
	int i;

	initTSPFromFile (&t, in);

//	tmp = (tsp_t *) malloc (sizeof(SIZE_TSP));
//	tmp->mat = (int **) malloc (sizeof(int *) * t->nb_node);
//	tmp->sol = (int *) malloc (sizeof(int) * t->nb_node);
//	for (i=0; i<t->nb_node; i++)
//		tmp->mat[i] = (int *) malloc (sizeof(int) *t->nb_node);
//
	createEmptyTSP(&tmp, t->nb_node);
	copyData(tmp, t);

	lightestString(t);
	printTSP(t);
	printf ("Valeur de la solution : %d\n", compCurVal(t));

	for (i=0; i<t->nb_node; t->mat[1][i] = -1, t->mat[i][1] = -1, t->sol[i++]=0);


	findACPM(t);

	for (i=0; i<t->nb_node; t->mat[1][i] = tmp->mat[1][i], t->mat[i][1] = tmp->mat[i++][1]);

	int mina = -1, minb = -1, inda = -1, indb = -1;
	for (i=0; i<t->nb_node; i++) {
		if (mina == -1) {
			mina = tmp->mat[i][1];
			inda = i;
		}
		else if (minb == -1) {
			minb = tmp->mat[i][1];
			indb = i;
		}

		if (tmp->mat[i][1] != -1 && tmp->mat[i][1] < mina) {
			minb = mina;
			indb = inda;
			mina = tmp->mat[i][1];
			inda = i;
		}
		else if (tmp->mat[i][1] != -1 && tmp->mat[i][1] < minb) {
			minb = tmp->mat[i][1];
			indb = i;
		}
	}
	t->sol[t->nb_node-1] = t->nb_node + inda;
	t->sol[t->nb_node-2] = t->nb_node + indb;
	printTSP(t);
	printf ("Valeur de la solution : %d\n", compCurVal(t));

	void *ta, *tc, *td;
	int **tb;
	size_t mouhahaha;

	int nbbranch = stratBranch (t, &ta, &mouhahaha);
	td = ta;

	tsp_t *f;
	tsp_t **z = (tsp_t **)ta;

	for (i=0; i<nbbranch; i++) {
		tb = (int **)td;
		tc = (void *) *tb;
		f = (tsp_t *) tc;
		printf ("==========> %d\n", i);
		printTSP(f);
		freeData(f);
		td += mouhahaha;
	}

	freeData((void *) t);
	freeData(tmp);
	free(ta);

	fclose(in);
	return 0;
}
