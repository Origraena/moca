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
ls_t *insSol (ls_t *lsol, bls_t *blsol, int b, opt_t order, strat_t str) {
	int relb = order * b;
	ls_t *tmpsol = lsol;
	if (!tmpsol) {
		tmpsol = (ls_t *) malloc (sizeof (ls_t));
		tmpsol->first = blsol;
		tmpsol->last = blsol;
		tmpsol->bound = b;
		return tmpsol;
	}

	switch (str) {
		case BEST_FIRST:
			if (relb > order * tmpsol->bound) {
				ls_t *new = (ls_t *) malloc (sizeof(ls_t));
				new->bound = b;
				new->first = blsol;
				new->last = blsol;
				new->next = tmpsol;
				return new;
			}

			while (tmpsol->next && relb <= order * tmpsol->next->bound)
				tmpsol = tmpsol->next;

			if (relb == tmpsol->bound) {
				blsol->next = tmpsol->first;
				tmpsol->first = blsol;
			}
			else {
				ls_t *new = (ls_t *) malloc (sizeof(ls_t));
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
	return tmpsol;
}

// Recover the first sol of the list of solution
bls_t *popSol(ls_t **lsol, int *b){
	bls_t *t = (*lsol)->first;
	*b = (*lsol)->bound;
	if (!t->next){
		(*lsol)->first = NULL;
		freeSol (lsol, NULL);
		*lsol = NULL;
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
pb_t *initPb (int (*compInitVal) (void *), int (*compCurVal) (void *), int (*stratBranch) (void *, void **, size_t *), opt_t order, void *data, void (*copyData) (void *, void*), void (*freeData) (void *), strat_t str, int (*acceptableSol) (void *), size_t size_data, int (*initData) (void *), void (*allocMem) (void **, void *), void (*printData) (void *)) {
	pb_t *new = (pb_t *) malloc (sizeof(pb_t));
	new->compInitVal = compInitVal;
	new->compCurVal = compCurVal;
	new->stratBranch = stratBranch;
	new->order = order;
	new->strat = str;
	new->acceptableSol = acceptableSol;
	new->copyData = copyData;
	new->freeData = freeData;
	new->size_data = size_data;
	new->allocMem = allocMem;
	new->printData = printData;

	void *tmp1, *tmp2;
	allocMem(&tmp1, data);
	allocMem(&tmp2, data);
	copyData (tmp1, data); copyData (tmp2, data);

	new->bestsol = compInitVal(tmp2);
	new->best =	tmp2;

	new->curnode = (ls_t *) malloc (sizeof(ls_t));
	new->curnode->first = newSol(tmp1);
	new->curnode->last = new->curnode->first;
	new->curnode->next = NULL;
	new->curnode->bound = initData(tmp1);
	return new;
}

// Free Branch and bound problem
void freePb (pb_t *p) {
	while (p->curnode)
		freeSol(&(p->curnode), p->freeData);
	freeData(p->best);
	free (p);
}

int resolve_pb (pb_t *pb, void *sol) {
	if (!pb)
		return -1;

	pb->copyData(sol, pb->best);

	int i = 0;

	while (pb->curnode) {
		printf ("Je rentre dans l'opt\n");
		int b = 0, nb_branch;
		size_t size_data;
		void *d1;
		// Choose a node
		bls_t *tmp = popSol(&(pb->curnode), &b);
		// Generate sons
		nb_branch = pb->stratBranch(tmp->var, &d1, &size_data);
		void *dtmp = d1, *y;
		int **z;
		printf ("Nombre de fils trouvés : %d\n", nb_branch);
		for (i=0; i<nb_branch; i++) {
			z = (int **)dtmp;
			y = (void *) *z;
			int heur = pb->compCurVal(y);
			if (heur * pb->order > pb->order * pb->bestsol) {
				if (pb->acceptableSol(y)) {
					pb->copyData(pb->best, y);
					pb->bestsol = heur;
				}
				else 
					pb->curnode = insSol(pb->curnode, newSol(y), heur, pb->order, pb->strat);
			}
			else
				freeData(y);
			dtmp += size_data;
		}
		free(d1);
		freeBSol(&tmp, pb->freeData);
	}
	copyData(sol, pb->best);
	return pb->bestsol;
}


