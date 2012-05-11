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

typedef struct b_list_sol {
	struct list_sol *next;
	char *var;
} bls_t;

#define SIZE_BLS sizeof(bls_t *) + sizeof(char *)

typedef struct list_sol {
	int bound;
	struct list_sol *next;
	bls_t *first;
} ls_t;

#define SIZE_LS sizeof(int) + sizeof(ls_t *) + sizeof(bls_t *)

typedef struct pb {
	int bestsol;
	ls_t *curnode;
	int (*compInitVal) (struct pb *p);
	int (*compCurVal) (struct pb *p, bls_t *s);
	int (*selBraVar) (struct pb *p, bls_t *s);
	int (*stratBranch) (struct pb *p);
} pb_t;

#define SIZE_PB sizeof(int) + sizeof(ls_t *) + 4*sizeof(int *)

bls_t *newSol(int n);
void insSol (ls_t *lsol, bls_t *blsol, int b);
bls_t *popSol(ls_t *lsol, int b);

void freeBSol(bls_t *b);
void freeSol(ls_t* s);

pb_t *initPb (int (*compInitVal) (struct pb *), int (*compCurVal) (struct pb *, bls_t *), int (*selBraVar) (struct pb*, bls_t*), int (*stratBranch) (struct pb *));
void freePb (pb_t *p);
#endif
