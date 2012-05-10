// partition.c

#include "partition.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int* partition(int* weights, const int n) {
	if (!weights || (n <= 0))
		return 0;
	mix(this_problem.weights,this_problem.n);
	uint i;
	this_problem.result = malloc(sizeof(int)*n);
	for (i = 0 ; i < n ; i++) {
		if (this_problem.w1 <= this_problem.w2) {
			this_problem.result[i] = 1;
			this_problem.w1 += weights[i];
		}
		else {
			this_problem.result[i] = 2;
			this_problem.w2 += weights[i];
		}
	}
	return this_problem.result;
}

void mix(int* weights, const int n) {
	uint i;
	uint j = 0;
	uint cpt;
	cell* tmp;
	cell* w = malloc(sizeof(cell));
	cell* current = w;
	for (i = 0 ; i < n ; i++) {
		current->value = weights[i];
		if (i < n-1) {
			current->next = malloc(sizeof(cell));
			current = current->next;
		} 
		else
			current->next = 0;
	}
	for (i = 0 ; i < n ; i++) {
		cpt = rand() % (n-i);
		current = w;
		if (!cpt) {
			tmp = w;
			w = w->next;
		}
		else {
			for (j = 0 ; j < cpt-1 ; j++)
				current = current->next;
			tmp = current->next;
			current->next = tmp->next;
		}
		weights[i] = tmp->value;
		free(tmp);
	}
}

void init_problem(char* filename) {
	printf("Initializing problem...\n");
	this_problem.sent = 0;
	this_problem.processed = 0;
	this_problem.n = 10;
	this_problem.weights = malloc(sizeof(int)*this_problem.n);
	uint i;
	for (i = 0 ; i < this_problem.n ; i++)
		this_problem.weights[i] = i;
	this_problem.result = 0;
	this_problem.w1 = 0;
	this_problem.w2 = 0;
	printf("Initialized problem!\n");
}

void init_problem_resource(char* filename) {
	this_problem.sent = 0;
	this_problem.processed = 0;
	this_problem.n = 10;
	this_problem.weights = malloc(sizeof(int)*this_problem.n);
	uint i;
	for (i = 0 ; i < this_problem.n ; i++)
		this_problem.weights[i] = i;
	this_problem.result = 0;
	this_problem.w1 = 0;
	this_problem.w2 = 0;
}

void free_problem() {
	if (this_problem.weights)
		free(this_problem.weights);
	this_problem.weights = 0;
	if (this_problem.result)
		free(this_problem.result);
	if (!this_problem.processed)
		pthread_kill(this_problem.thread_id,SIGPIPE);
}

void processingThreadFunction(void* name) {
//	init_problem(0);
	char clef[2] = "c";
	partition(this_problem.weights,this_problem.n);
	this_problem.processed = 1;
	this_problem.sent = 0;
	if (write(pipeW2,clef,2*sizeof(char)) == -1)
		perror("write error...");
	else
		printf("write ok !\n");
	this_problem.thread_id = 0;
}

