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

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _TSP_H
#define _TSP_H

#define ROW 0
#define COL 1

typedef struct tsp {
	int nb_node;
	int **mat;
} tsp_t;

void initTSPFromFile (tsp_t *tsp, FILE *in);

int validTSP (const gpointer tsp);
int bestSol (const gpointer val1, const gpointer val2);
gpointer hungAlgo (const gpointer tsp, gpointer tsp_sol);

gpointer lightestString (const gpointer tsp, gpointer tsp_sol);
gpointer opt2 (const gpointer tsp, gpointer tsp_sol);
gpointer opt3 (const gpointer tsp, gpointer tsp_sol);

#endif
