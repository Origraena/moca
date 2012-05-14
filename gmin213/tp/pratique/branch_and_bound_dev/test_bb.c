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

#include <time.h>

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
	strat_t str = WIDTH_FIRST;

	pb_t *p = initPb(opt2, compCurVal, stratBranch, ord, t, copyData, freeData, str, acceptableSol, sizeof(tsp_t), initData, allocMem, printTSP);
	
	time_t cur = time(0);
	resolve_pb(p, sol);
	cur = time(0) - cur;
	
	int nbSommets = 0;
	in = fopen(fil, "r");
	fscanf(in, "%d",  &nbSommets);
	fclose(in);
	FILE *exec_time = fopen("tsp_bb_2opt.output", "a");
	fprintf(exec_time, "%d %d\n", nbSommets, cur);
	fclose(exec_time);

	if (acceptableSol(sol))
		printTSP((tsp_t*)sol);

	printf("Valeur solution : %d\n", compCurVal(sol));

	freePb(p);
	freeData(t);
	freeData(sol);
	return 0;
	
}
