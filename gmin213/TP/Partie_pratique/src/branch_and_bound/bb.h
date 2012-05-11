/*
 * =====================================================================================
 *
 *       Filename:  pb.h
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

#ifndef _BB_H
#define _BB_H

typedef enum opt {
	MIN = -1,
	MAX =  1
} opt_t;

// List of current solution relative to a given bound
typedef struct b_list_sol {
	struct list_sol *next;
	char *var;
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
	int (*compInitVal) (struct pb *p);
	int (*compCurVal) (struct pb *p, bls_t *s);
	int (*selBraVar) (struct pb *p, bls_t *s);
	int (*stratBranch) (struct pb *p);
} pb_t;

#define SIZE_PB sizeof(int) + sizeof(ls_t *) + 4*sizeof(int *)

// Create a new solution
bls_t *newSol(int n);
// Insert a solution at the right place in the list of acceptable bound
void insSol (ls_t **lsol, bls_t *blsol, int b, opt_t order);
bls_t *popSol(ls_t **lsol, int b);

void freeBSol(bls_t *b);
void freeSol(ls_t* s);

pb_t *initPb (int (*compInitVal) (struct pb *), int (*compCurVal) (struct pb *, bls_t *), int (*selBraVar) (struct pb*, bls_t*), int (*stratBranch) (struct pb *));
void freePb (pb_t *p);
#endif
