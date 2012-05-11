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
void insSol (ls_t **lsol, bls_t *blsol, int b, opt_t order) {
	int relb = order * b;
	ls_t *tmpsol = *lsol;
	if (relb > order * tmpsol->bound) {
		ls_t *new = (ls_t *) malloc (SIZE_LS);
		new->bound = b;
		new->first = blsol;
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
pb_t *initPb (int (*compInitVal) (struct pb *, void *), int (*compCurVal) (struct pb *, void *), int (*selBraVar) (struct pb*, void *), int (*stratBranch) (struct pb *, void *, void *, void *, int), opt_t order, bls_t *initData(void *), void *data, void (*copyData) (void *, void*), void (*freeData) (void *)) {
	pb_t *new = (pb_t *) malloc (SIZE_PB);
	new->compInitVal = compInitVal;
	new->compCurVal = compCurVal;
	new->selBraVar = selBraVar;
	new->stratBranch = stratBranch;
	new->order = order;
	new->copyData = copyData;
	new->freeData = freeData;

	new->curnode = (ls_t *) malloc (SIZE_LS);
	new->curnode->first = initData(data);
	new->curnode->next = NULL;
	new->curnode->bound = compInitVal(new, new->curnode->first);
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
		int b = 0, ret = 0, nb_branch, size_data;
		void *d1, *d2;
		bls_t *tmp = popSol(&(pb->curnode), &b);
		ret = pb->selBraVar(pb, tmp->var);
		switch(ret) {
			// Acceptable Solution
			case ACC:
				// Update current solution
				pb->copyData(sol, tmp);
				// Update best bound
				pb->bestsol = b;
				freeBSol (&tmp, pb->freeData);
				break;
			case NON_ACC:
				freeBSol (&tmp, pb->freeData);
				break;
			default:
				// Create a new branch point
				// TODO Implement more than 2 branchpoints
				nb_branch = pb->stratBranch(pb, tmp, d1, d2, ret);
				if (d1) {
					bls_t *b1 = newSol(d1);
					int value = pb->compCurVal(pb, b1);
					pb->bestsol = pb->order * value > pb->order * pb->bestsol ? value : pb->bestsol;
					insSol(&(pb->curnode), b1, value, pb->order);
				}
				if (d2) {
					bls_t *b2 = newSol(d2);
					int value = pb->compCurVal(pb, b2);
					pb->bestsol = pb->order * value > pb->order * pb->bestsol ? value : pb->bestsol;
					insSol(&(pb->curnode), b2, value, pb->order);
				}	
				break;	
		}
	}
	return pb->bestsol;
}


