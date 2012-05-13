#!/bin/sh
gcc -c src/main_bag.c src/bag.c src/core.c
mv *.o target/obj/
gcc -o target/gmin213_b target/obj/main_bag.o target/obj/bag.o target/obj/core.o

