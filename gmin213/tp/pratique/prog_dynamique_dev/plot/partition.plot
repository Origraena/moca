set term png font "/Library/Fonts/Arial.ttf" 10
set term png 
set output "./res/partition.png"

set dgrid3d 100,100,7
set pm3d
set ticslevel 0
#set xrange [1:20000]
set yrange [0:250000]
set xtics 4000 offset -1
set ytics 50000 offset 2
#set ztics 25
set xlabel "Nombre d'objets" offset -4
set ylabel "Somme des poids"  offset 4
set zlabel "Temps d'exécution" rotate by 90.0
set title "Temps d'exécution sur le probleme de la partition"
#set style function lp
set view 60,30
#set contour

splot "./res/partition.output" using 1:3:2 notitle with dots

