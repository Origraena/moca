// bag.h

#ifndef BAG_H_
#define BAG_H_

#include <stdlib.h>
#include <stdio.h>
#include "core.h"

extern global_var global;

typedef struct bag_item {
	uint u;
	uint v;
} bag_item;

typedef struct bag {
	uint n;
	uint nb;
	uint v;
	bag_item* items;
} bag;

typedef struct bag_cell {
	uint item;
	uint nb;
	struct bag_cell* next;
} bag_cell;

typedef struct bag_sol {
	uint u;
	uint v;
	uint n;
	bag_cell* q;
} bag_sol;

bag* createBag(const uint n, bag* dest);
bag* generateBag(const uint n, const uint v, const uint, const uint, const uint, const uint, bag*);
void freeBag(bag* b); 
void printBag(bag* b);

bag_sol* createBagSol(const uint n, bag_sol* dest);
bag_sol* copyBagSol(bag_sol* src, bag_sol* dest, bag* b);
void freeBagSol(bag_sol* b);
void freeList(bag_cell* cell);
bag_sol* addToSol(bag_sol* sol, bag* b, const uint item, const uint nb);
void printBagSol(bag*, bag_sol* sol);
bag_sol* bag_resolution(bag* b);

#endif // BAG_H_

