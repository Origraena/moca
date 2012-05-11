/*
 * =====================================================================================
 *
 *       Filename:  bb.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2012 15:43:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

//#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _BB_H
#define _BB_H

#define NON_ACC -1
#define ACC			-2

typedef enum opt {
	MIN = -1,
	MAX =  1
} opt_t;

// List of current solution relative to a given bound
typedef struct b_list_sol {
	struct b_list_sol *next;
	void *var;
} bls_t;

#define SIZE_BLS sizeof(bls_t *) + sizeof(char *)

// List of accpetable bound, containing pointer on list of current solution
typedef struct list_sol {
	int bound;
	struct list_sol *next;
	bls_t *first;
} ls_t;

#define SIZE_LS sizeof(int) + sizeof(ls_t *) + sizeof(bls_t *)

// data structure representing a branch and bound problem
typedef struct pb {
	int bestsol;
	ls_t *curnode;
	opt_t order;
	int (*compInitVal) (struct pb *p, void *s);
	int (*compCurVal) (struct pb *p, void *s);
	int (*selBraVar) (struct pb *p, void *s);
	int (*stratBranch) (struct pb *p, void *branchnode, void *newnodes, void *var);
	void (*copyData) (void *d1, void *d2);
	void (*freeData) (void *d);
} pb_t;

#define SIZE_PB sizeof(int) + sizeof(ls_t *) + 4*sizeof(int *)

// Create a new solution
bls_t *newSol(void *variables);
// Insert a solution at the right place in the list of acceptable bound
void insSol (ls_t **lsol, bls_t *blsol, int b, opt_t order);
// Retrieve the best current solution
bls_t *popSol(ls_t **lsol, int *b);

void freeBSol(bls_t **b, void (*freeData)(void *));
void freeSol(ls_t **s, void (*freeData)(void *));

pb_t *initPb (int (*compInitVal) (struct pb *, void *), int (*compCurVal) (struct pb *, void *), int (*selBraVar) (struct pb*, void *), int (*stratBranch) (struct pb *, void *, void *, void *), opt_t order, bls_t *initData(void *), void *data, void (*copyData) (void *, void*), void (*freeData) (void *));
void freePb (pb_t *p);

int resolve_pb (pb_t *pb, void *sol);
#endif
