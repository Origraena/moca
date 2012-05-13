// core.c

#include "core.h"

void info(const char* str,...) {
	if (global.verbose)
		printf("[INFO] %s\n",str);
}

void debug(const char* str,...) {
	if (global.debug)
		printf("[DEBUG] %s\n",str);
}


