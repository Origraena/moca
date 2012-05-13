#!/bin/sh
gcc -c src/main_tsp.c src/graphs.c src/core.c
mv *.o target/obj/
gcc -o target/gmin213_t target/obj/main_tsp.o target/obj/graphs.o target/obj/core.o

