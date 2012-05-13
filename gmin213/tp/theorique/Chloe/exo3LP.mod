/* Couplage max de poids min */


param n, integer, >= 2;
/* number of nodes */

set V, default {1..n};
/* set of nodes */

set E, within V cross V;
/* set of arcs */

param a{(i,j) in E}, > 0;
/* a[i,j] is weight of arc (i,j) */

var X{(i,j) in E}, >=0 <=1;

s.t. couplage{i in V}:
	sum{(i,j) in E} X[i,j] + sum{(j,i) in E} X[j,i] = 1;
	
minimize poids: sum{(i,j) in E} (X[i,j] * a[i,j]);

solve;

printf "Poids total optimal : %d.\n", poids;
printf "Aretes retenues : ";
printf{(i,j) in E: X[i,j] != 0}: "{(%d,%d) ; %.3f} ", i, j, X[i,j];
printf "\n";

data;


param n := 6;

param : E :   a :=
       1 2   1
       1 3   1
       2 3   1
       2 6   1000
       1 5   1000
       3 4   1000
       4 6   1
       6 5   1
       4 5   1;

end;
