#!/bin/sh
gcc -c src/*.c
mv *.o target/obj/
gcc -o target/gmin213 target/obj/*.o

