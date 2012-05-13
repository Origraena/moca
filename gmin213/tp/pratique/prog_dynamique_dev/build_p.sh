#!/bin/sh
gcc -c src/main_partition.c src/partition.c src/core.c
mv *.o target/obj/
gcc -o target/gmin213_p target/obj/main_partition.o target/obj/partition.o target/obj/core.o

