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

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _BB_H
#define _BB_H

// This structure represents a B&B problem
typedef struct bandb {
	// Pointer on the tree of explorated solution
	GNode *sol_tree	;
	// Best valid solution
	// It is dynamically updated during the algotrithm
	gpointer curBestSol;
	// The value of the current best valid solution
	gpointer curSolVal;
	// It represents a kind of buffer (it's useful in order to avoid dynamic memory allocation)
	gpointer tempSol;
	// Check if given solution is an admissible one
	int (*checkSolValidity) (const gpointer data);
	// Function to compare value of two solution in order to determine which one is the best
	int (*compareSolution) (const gpointer data1, const gpointer data2);
	// Function to compute optimal value of relaxed problem, returns value of solution
	gpointer (*compCurSol) (const gpointer data, gpointer sol);
	// Function to compute initial solution for the b&b problem
	gpointer (*compInitSol) (const gpointer data, gpointer sol);
} bb_t;

// Init function
void initBAndB (bb_t *bb, gpointer pb, int (*checkSolValidity) (const gpointer data), double (*compCurSol) (const gpointer data, gpointer, sol), gpointer (*compareSolution) (const gpointer d1, const gpointer d2), gpointer (*compInitSol) (const gpointer data, gpointer sol));
// Free function
void freeBAndB (bb_t *bb);

// B&B algorithm
void execBAndB (bb_t *bb);
#endif
