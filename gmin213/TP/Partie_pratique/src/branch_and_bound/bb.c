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

#include "bb.c"

// Create a new solution
bls_t *newSol(int n) {
	bls_t *b = (bls_t *) malloc (SIZE_BLS);
	b->var = (char *) calloc (n,  sizeof(char));
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

		while (tmpsol->_next && relb <= order * tmpsol->next->bound)
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
}

// Recover the first sol of the list of solution
bls_t *popSol(ls_t **lsol, int b){
	bls_t *t = *lsol->first;
	if (!t->next){
		*lsol->first = NULL;
		freeSol (lsol);
	}
	else
		*lsol->first = t->next;

	return t;
}

// Free used memory
// It frees only on blsol, if this one has a next, the pointer is updated to point on the next
void freeBSol(bls_t **b) {
	bls_t *tmp = *b->next;
	free(*b->sol);
	free(*b);
	*b = tmp;
}

// It frees only on lsol, if this one has a next, the pointer is updated to point on the next
// It also frees all blsol contained in this structure
void freeSol(ls_t **s) {
	while (*s->first)
		freeBSol(&(*s->first));
	ls_t *tmp = *s->next;
	free(*s);
	*s = tmp;
}

// Initialize branch and bound problem
pb_t *initPb (int (*compInitVal) (struct pb *, bls_t *), int (*compCurVal) (struct pb *, bls_t *), int (*selBraVar) (struct pb*, bls_t*), int (*stratBranch) (struct pb *), opt_t order, bls_t *initData(void *), void *data) {
	pb_t *new = (pb_t *) malloc (SIZE_PB);
	new->compInitVal = compInitVal;
	new->compCurVal = compCurVal;
	new->selBraVar = selBraVar;
	new->stratBranch = stratBranch;
	new->order = order;

	new->curnode = (ls_t *) malloc (SIZE_LS);
	new->curnode->first = initData(data);
	new->curnode->next = NULL;
	new->curnode->bound = compInitVal(new, new->curnode->first);
	new->bestsol = new->curnode->bound; 
}

// Free Branch and bound problem
void freePb (pb_t *p);

