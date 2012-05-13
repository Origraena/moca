set term png 
set output "./res/bag.png"

set dgrid3d 100,100,7
set pm3d
set ticslevel 0
set xrange [0:25000]
set yrange [0:25000]
set xtics 5000
set ytics 5000
set ztics 50
set xlabel "Nombre d'objets" offset -5
set ylabel "Volume" 
set zlabel "Temps d'execution" rotate by 90
set title "Temps d'execution sur le probleme du sac a dos"
#set style function lp
set view 60,30
#set contour

splot "./res/bag.output" notitle with dots

