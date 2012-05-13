set term png 
set output "./res/tsp.png"

#set xrange [0:25000]
#set yrange [0:25000]
set xtics 5
set ytics 30
set xlabel "Nombre de sommets"
set ylabel "Temps d'execution"
set title "Temps d'execution sur le probleme du voyageur de commerce"
#set style function lp
#set contour

plot "./res/tsp.output" with dots 

