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
	tsp_t *t;
	FILE *in = fopen ("graph", "r");
	int i;

	initTSPFromFile (&t, in);

	lightestString(t);
	printTSP(t);
	printf ("Valeur de la solution : %d\n", compCurVal(t));

	for (i=0; i<t->nb_node; t->sol[i++]=0);

	findACPM(t);
	printTSP(t);
	printf ("Valeur de la solution : %d\n", compCurVal(t));


	freeData((void *) t);

	fclose(in);
	return 0;
}
