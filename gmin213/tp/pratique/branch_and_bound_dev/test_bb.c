/*
 * =====================================================================================
 *
 *       Filename:  test_bb.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13/05/2012 13:39:21
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

int main (int argc, char *argv) {
	tsp_t *t;
	void *sol;

	FILE *in = fopen("graph", "r");
	initTSPFromFile(&t, in);
	fclose (in);

	allocMem(&sol, t);

	opt_t ord = MIN;
	strat_t str = WIDTH_FIRST;

	pb_t *p = initPb(lightestString, compCurVal, stratBranch, ord, t, copyData, freeData, str, acceptableSol, sizeof(tsp_t), initData, allocMem, printTSP);

	resolve_pb(p, sol);

	printTSP((tsp_t*)sol);
	printf("Valeur solution : %d\n", compCurVal(sol));

	freePb(p);
	freeData(t);
	freeData(sol);
	return 0;
	
}
