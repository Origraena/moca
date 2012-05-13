set output "./res/queuesize.png"
set term png 

set yrange [0:120]
set ytics 20
set xtics 0.5

set xlabel "Temps"
set ylabel "Taille de file"
set title "Evolution de la taille de la file"


plot \
"res/tp1_queue001.tr" title "freq 0.01" with lines lt 3, \
"res/tp1_queue005.tr" title "freq 0.05" with points lt 1
