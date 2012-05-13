
#ifndef CORE_H_
#define CORE_H_

#include <stdlib.h>
#include <stdio.h>

#define CAPACITY_FOOT 6
#define false 0
#define true 1

typedef struct global_var {
	int verbose;
	int debug;
} global_var;

typedef unsigned int uint;

extern global_var global;

void info(const char* str,...);
void debug(const char* str,...);

#endif // CORE_H_

