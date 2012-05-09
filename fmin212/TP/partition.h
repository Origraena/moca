// partition.h

#ifndef PARTITION_H_
#define PARTITION_H_
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

typedef struct problem {
	int sent;
	int processed;
	uint n;
	int* weights;
	int* result;
	int w1,w2;
	pthread_t thread_id;
} problem;

typedef struct cell {
	int value;
	struct cell* next;
} cell;

int* partition(int* weights, const int n);
void mix(int* weights,const int n);
void init_problem(char*);
void init_problem_resource(char*);
void free_problem();
void processingThreadFunction(void*);

extern problem this_problem;

#endif // PARTITION_H_
