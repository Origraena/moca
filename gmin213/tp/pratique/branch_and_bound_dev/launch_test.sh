#!/bin/sh
for i in `seq 1 $1`; do
../prog_dynamique_dev/target/graph_gen autogenerated 14 15 1 25
solver_2opt autogenerated
done