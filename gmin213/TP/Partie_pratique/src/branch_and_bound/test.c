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

int main (int argc, char **argv) {
	tsp_t t;

	t.nb_node = 4;
	t.mat = (int **) calloc (t.nb_node, sizeof (int *));

	int i;
	for (i=0; i< t.nb_node; i++)
		t.mat[i] = (int *) calloc (t.nb_node, sizeof (int));

	t.mat[0][1] = 1;
	t.mat[1][0] = 1;
	t.mat[2][3] = 1;
	t.mat[3][2] = 1;

	printf ("%d\n", validTSP((gpointer) &t));

	for (i=0; i< t.nb_node; i++)
		free (t.mat[i]);

	free(t.mat);

	int a = 0, b = 1;
	printf ("%d\n", bestSol((gpointer) &a, (gpointer) &b));

	return 0;
}
