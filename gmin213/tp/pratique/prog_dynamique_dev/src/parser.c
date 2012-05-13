// parser.c

#include <stdio.h>
#include <stdlib.h>
#include "core.h"

global_var global;

int main(int argc, char** argv) {
	global.debug = 0;
	global.verbose = 0;

	FILE* input;
	FILE* output;

	if (argc != 3) {
		printf("Syntax : %s input output\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((input = fopen(argv[1],"r")) == -1) {
		printf("Error opening file in read mode : %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	if ((output = fopen(argv[2],"w")) == -1) {
		printf("Error opening file in write mode : %s\n",argv[2]);
		fclose(input);
		exit(EXIT_FAILURE);
	}

	uint n,v;
	long int s;
	char *ns, *vs, *ss;
	
	while (fscanf(input,"%u %li %u",&n,&s,&v) > 0) {
		utoa(n,&ns);
		utoa(s,&ss);
		utoa(v,&vs);
		fprintf(output,"%s %s %s\n",ns,vs,ss);
		free(ns);
		free(ss);
		free(vs);
	}

	return EXIT_SUCCESS;

}


