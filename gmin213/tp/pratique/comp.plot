set term png font "/Library/Fonts/Arial.ttf" 10
set term png 
set output "./comp.png"

#set xrange [0:25000]
#set yrange [0:25000]
#set xtics 5
#set ytics 30
set xlabel "Nombre de sommets"
set ylabel "Temps d'exécution"
set title "Temps d'exécution sur le probleme du TSP"
#set style function lp
#set contour

plot "./branch_and_bound_dev/tsp_bb_2opt_lissage.txt" title "branch&bound" with linespoints, "./prog_dynamique_dev/lissage.txt" title "programmation dynamique" with linespoints
