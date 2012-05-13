/*
 * =====================================================================================
 *
 *       Filename:  tsp.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2012 16:10:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef _TSP_H
#define _TSP_H

typedef struct tsp {
	int nb_node;
	int *sol;
	int **mat;
} tsp_t;

#define SIZE_TSP sizeof(tsp_t)

void initTSPFromFile (tsp_t **tsp, FILE *in);
void createEmptyTSP (tsp_t **tsp, int);

int compCurVal (void *s);
int stratBranch (void *branchpoint, void **newbranch, size_t *size);
int acceptableSol (void *data);

void copyData (void *d1, void *d2);
void freeData (void *d);

int lightestString (void *data);
int opt2 (void *data);
int opt3 (void *data);

void printTSP (tsp_t *t);

int findACPM (tsp_t *t);
#endif
