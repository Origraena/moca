/*
 * =====================================================================================
 *
 *       Filename:  bb.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/05/2012 02:56:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "bb.h"

// Create a new solution
bls_t *newSol(void *variable) {
	bls_t *b = (bls_t *) malloc (SIZE_BLS);
	b->var = variable;
	b->next = NULL;

	return b;
}

// Insert given solution at the right place in the list of all solution
void insSol (ls_t **lsol, bls_t *blsol, int b, opt_t order, strat_t str) {
	int relb = order * b;
	ls_t *tmpsol = *lsol;
	switch (str) {
		case BEST_FIRST:
			if (relb > order * tmpsol->bound) {
				ls_t *new = (ls_t *) malloc (SIZE_LS);
				new->bound = b;
				new->first = blsol;
				new->last = blsol;
				new->next = tmpsol;
				*lsol = new;
				return;
			}

			while (tmpsol->next && relb <= order * tmpsol->next->bound)
				tmpsol = tmpsol->next;

			if (relb == tmpsol->bound) {
				blsol->next = tmpsol->first;
				tmpsol->first = blsol;
			}
			else {
				ls_t *new = (ls_t *) malloc (SIZE_LS);
				new->bound = b;
				new->first = blsol;
				new->next = tmpsol->next;
				tmpsol->next = new;
			}
			break;
		case DEPTH_FIRST:
			blsol->next = tmpsol->first;
			tmpsol->first = blsol;
			break;
		case WIDTH_FIRST:
			tmpsol->last->next = blsol;
			tmpsol->last = blsol;
			break;
	}
}

// Recover the first sol of the list of solution
bls_t *popSol(ls_t **lsol, int *b){
	bls_t *t = (*lsol)->first;
	*b = (*lsol)->bound;
	if (!t->next){
		(*lsol)->first = NULL;
		freeSol (lsol, NULL);
	}
	else
		(*lsol)->first = t->next;

	return t;
}

// Free used memory
// It frees only on blsol, if this one has a next, the pointer is updated to point on the next
void freeBSol(bls_t **b, void (*freeData) (void *)) {
	bls_t *tmp = (*b)->next;
	freeData((*b)->var);
	free(*b);
	*b = tmp;
}

// It frees only on lsol, if this one has a next, the pointer is updated to point on the next
// It also frees all blsol contained in this structure
void freeSol(ls_t **s, void (*freeData)(void *)) {
	while ((*s)->first)
		freeBSol(&((*s)->first), freeData);
	ls_t *tmp = (*s)->next;
	free(*s);
	*s = tmp;
}

// Initialize branch and bound problem
pb_t *initPb (int (*compInitVal) (void *), int (*compCurVal) (void *), int (*stratBranch) (void *, void **, size_t *), opt_t order, bls_t *initData(void *), void *data, void (*copyData) (void *, void*), void (*freeData) (void *), strat_t str, int (*acceptableSol) (void *)) {
	pb_t *new = (pb_t *) malloc (SIZE_PB);
	new->compInitVal = compInitVal;
	new->compCurVal = compCurVal;
	new->stratBranch = stratBranch;
	new->order = order;
	new->strat = str;
	new->acceptableSol = acceptableSol;
	new->copyData = copyData;
	new->freeData = freeData;

	new->curnode = (ls_t *) malloc (SIZE_LS);
	new->curnode->first = initData(data);
	new->curnode->next = NULL;
	new->curnode->bound = compInitVal(new->curnode->first);
	new->bestsol = new->curnode->bound; 
}

// Free Branch and bound problem
void freePb (pb_t *p) {
	while (p->curnode)
		freeSol(&(p->curnode), p->freeData);
	free (p);
}

int resolve_pb (pb_t *pb, void *sol) {
	if (!pb)
		return -1;

	pb->copyData(sol, pb->curnode->first->var);

	int i = 0;

	while (pb->curnode) {
		int b = 0, nb_branch;
		size_t size_data;
		void *d1;
		// Choose a node
		bls_t *tmp = popSol(&(pb->curnode), &b);
		// Generate sons
		nb_branch = pb->stratBranch(tmp, &d1, &size_data);
		void *dtmp = d1;
		for (i=0; i<nb_branch; i++) {
			dtmp += size_data;
			int heur = pb->compCurVal(dtmp);
			if (heur * pb->order > pb->order * pb->bestsol) {
				if (pb->acceptableSol(dtmp)) {
					pb->copyData(sol, dtmp);
					pb->bestsol = heur;
				}
				else
					insSol(&(pb->curnode), dtmp, heur, pb->order, pb->strat);
			}
		}
		freeBSol(&tmp, pb->freeData);
	}
	return pb->bestsol;
}


