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

int main (int argc, char **argv) {
	tsp_t *t;
	void *sol;
	char *fil = "graph";

	if (argc == 2)
		fil = argv[1];

	FILE *in = fopen(fil, "r");
	initTSPFromFile(&t, in);
	fclose (in);

	allocMem(&sol, t);

	opt_t ord = MIN;
	strat_t str = BEST_FIRST;

	pb_t *p = initPb(lightestString, compCurVal, stratBranch, ord, t, copyData, freeData, str, acceptableSol, sizeof(tsp_t), initData, allocMem, printTSP);

	resolve_pb(p, sol);

	if (acceptableSol(sol))
		printTSP((tsp_t*)sol);

	printf("Valeur solution : %d\n", compCurVal(sol));

	freePb(p);
	freeData(t);
	freeData(sol);
	return 0;
	
}
