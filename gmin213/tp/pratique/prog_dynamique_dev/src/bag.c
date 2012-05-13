// bag.c

#include "bag.h"

bag* createBag(const uint n, bag* dest) {
	if (!n)
		return dest;
	if (!dest)
		dest = malloc(sizeof(bag));
	else if (dest->n > 0)
		free(dest->items);
	dest->v = 0;
	dest->n = n;
	dest->items = malloc(sizeof(bag_item)*n);
	return dest;
}

bag* generateBag(const uint n, const uint v,
				 const uint uMin, const uint uMax, 
				 const uint vMin, const uint vMaxt, 
				 bag* dest) {
	uint vMax = vMaxt;
	if ((vMin == 0) 
	|| (vMin >= vMax) 
	|| (uMin >= uMax))
		return 0;
	dest = createBag(n,dest);
	if (!dest)
		return dest;
	if (vMax > v)
		vMax = v;
	uint uRng = uMax - uMin;
	uint vRng = vMax - vMin;
	dest->v = v;
	uint i;
	for (i = 0 ; i < n ; i++) {
		dest->items[i].u = rand() % uRng + uMin;
		dest->items[i].v = rand() % vRng + vMin;
	}
	return dest;
}

void freeBag(bag* b) {
	if (b->n > 0)
		free(b->items);
	free(b);
}

void printBag(bag* b) {
	if (!b)
		return;
	if (b->n == 0) {
		printf("Empty problem.\n");
		return;
	}
	uint i;
	printf("volume : %i\n",b->v);
	printf("%i items :\n",b->n);
	printf("item\tvolume\tutility\n");
	for (i = 0 ; i < b->n ; i++)
		printf("%i\t%i\t%i\n",i+1,b->items[i].v,b->items[i].u);
}

bag_sol* createBagSol(const uint v, bag_sol* dest) {
	debug("Creating bag solution...");
	if (!dest)
		dest = malloc(sizeof(bag_sol));
	else if (dest->n > 0)
		freeList(dest->q);
	dest->u = 0;
	dest->v = v;
	dest->n = 0;
	dest->q = 0;
	debug("created!");
	return dest;
}

bag_sol* copyBagSol(bag_sol* src, bag_sol* dest, bag* b) {
	if (global.debug)
		printf("[DEBUG] Copying bag sol from %li to %li\n",(long int)src,(long int)dest);
	dest = createBagSol(0, dest);
	bag_cell* current = src->q;
	while (current) {
		dest = addToSol(dest,b,current->item,current->nb);
		current = current->next;
	}
	if (global.debug) {
		printf("[DEBUG] Copyied bag sol from %li to %li\n",(long int)src,(long int)dest);
		printf("[DEBUG] src = \n");
		printBagSol(b,src);
		printf("[DEBUG] dest = \n");
		printBagSol(b,dest);
	}
	return dest;
}

void freeBagSol(bag_sol* b) {
	debug("Freeing bag sol...");
	if (!b) {
		debug("no pointer");
		return;
	}
	if (b->q)
		freeList(b->q);
	free(b);
	debug("Bag sol freed!");
}

void freeList(bag_cell* cell) {
	debug("Freeing list...");
	if (!cell)
		return;
	bag_cell* current = cell;
	bag_cell* next = cell->next;
	while (current) {
		free(current);
		current = next;
		if (current)
			next = current->next;
	}
	debug("Freed list!");
}

bag_sol* addToSol(bag_sol* sol, bag* b, const uint item, const uint q) {
	bag_cell* prevHead = 0;
	if (sol->q)
		prevHead = sol->q;
	sol->q = 0;
	sol->q = malloc(sizeof(bag_cell));
	sol->q->item = item;
	sol->q->nb = q;
	sol->q->next = prevHead;
	sol->u += q * b->items[item].u;
	sol->n += q;
	sol->v += q * b->items[item].v;
	if (global.debug) {
		printf("added to sol\n");
		printBagSol(b,sol);
	}
	return sol;
}

void printBagSol(bag* b, bag_sol* sol) {
	if ((!b) || (!sol))
		return;
	if (b->n == 0) 
		printf("Empty problem.\n");
	uint i = 0;
	printf("Bag of volume %i : %i with %i items\n",sol->v,sol->u,sol->n);
	printf("quantity\titem\tvolume\tutility\n");
	bag_cell* current = sol->q;
	uint item;
	while (current) {
		item = current->item;
		printf("\t%i\t%i\t%i\t%i\n",current->nb,item+1,b->items[item].v,b->items[item].u);
		current = current->next;
	}
}

bag_sol* bag_resolution(bag* b) {

	info("Bag resolution begins...");
		
	bag_sol** prev = malloc(sizeof(bag_sol*)*b->v);
	bag_sol** current = malloc(sizeof(bag_sol*)*b->v);
	uint i,j,k;
	uint v,vk;

	// init
	debug("\tinitializing...");
	for (j = 1 ; j <= b->v ; j++) {
		current[j-1] = 0;
		current[j-1] = createBagSol(0,current[j-1]);
		prev[j-1] = 0;
		if (global.debug)
			printBagSol(b,current[j-1]);
	}
	debug("\tinitialized!");

	// process
	debug("\tprocessing...");
	for (i = 0 ; i < b->n ; i++) {
		while (b->items[i].u <= 0)
			i++;
		// copy current in prev 
		debug("\t\tcopying current in prev...");
		for (j = 0 ; j < b->v ; j++) {
			freeBagSol(prev[j]);
			prev[j] = current[j];
			current[j] = 0;
		}
		debug("\t\tcopied current in prev!");
		// each different volume
		for (j = 1 ; j <= b->v ; j++) {
			vk = j / b->items[i].v;
			if (global.debug)
				printf("\t\t\ti = %i j = %i vi = %i vk = %i\n",i,j,b->items[i].v,vk);
			if (j > 1)
				current[j-1] = copyBagSol(current[j-2],current[j-1],b);
			for (k = 0 ; k <= vk ; k++) {
				v = j - (b->items[i].v * k) - 1;
				if (global.debug) printf("\t\t\t\tv = %i\n",v);
				if (v == -1) {
					 if ((!current[j-1]) 
	     			 || (current[j-1]->u < (b->items[i].u*k))) {
					debug("\t\t\t\tnew solution found");
					freeBagSol(current[j-1]);
					current[j-1] = 0;
					debug("\t\t\t\taffectation...");
					current[j-1] = createBagSol(j,current[j-1]);
					if (k)
						current[j-1] = addToSol(current[j-1],b,i,k);
					}
					if (global.debug) {
						debug("Current solution :");
						printBagSol(b,current[j-1]);
					}
				}
				else if ((!current[j-1]) 
				|| (current[j-1]->u < (prev[v]->u + (b->items[i].u*k)))) {
					debug("\t\t\t\tnew solution found");
					freeBagSol(current[j-1]);
					current[j-1] = 0;
					debug("\t\t\t\taffectation...");
					current[j-1] = copyBagSol(prev[v],current[j-1],b);
					if (k)
						current[j-1] = addToSol(current[j-1],b,i,k);
				}
				else
					debug("\t\t\t\tbad solution");
			}
			if (global.debug) {
				printf("[DEBUG]\t\t\tcurrent[%i]\n",j-1);
				printBagSol(b,current[j-1]);
			}
		}
		if (global.debug) {
			printf("[DEBUG] solutions for i = %i\n",i);
			for (j = 0 ; j < b->v ; j++) {
				printf("[DEBUG] v = %i\n",j+1);
				printBagSol(b,current[j]);
			}
		}
	}
	debug("\tprocessed!");

	// return solution
	debug("\tchoosing solution...");
	bag_sol* sol = 0;
	sol = copyBagSol(current[b->v-1],sol,b);
	//printBagSol(b,sol);
	for (j = 0 ; j < b->v ; j++) {
		if (global.debug)
			printf("[DEBUG]\tj = %i\n",j);
		freeBagSol(prev[j]);
		prev[j] = 0;
		freeBagSol(current[j]);
		current[j] = 0;
	}

	free(prev);
	free(current);
	if (global.debug) {
		debug("\tsolution found!");
		debug("solution final");
		printBagSol(b,sol);
	}
	info("Bag resolution ended!\n");
		
	return sol;
}

