
#ifndef PARTITION_H_
#define PARTITION_H_

#include <stdlib.h>
#include <stdio.h>
#include "core.h"

int* partition(int weights[], const int n);
int* partitionSol(int** T, int weights[], int n, int i, int j, int* sol);


#endif // PARTITION_H_

