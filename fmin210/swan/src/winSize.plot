set output "./res/winsize.png"
set term png 

set yrange [0:22]
set ytics 1
set xtics 0.5

set xlabel "Temps"
set ylabel "Taille de fenetre"
set title "Comparaison des tailles de fenetre TCP"

set style line 1 lt 2 lw 2
set style line 2 lt 3 lw 2
set style line 3 lt 4 lw 2


plot \
"res/tp1_winsize1.tr" title "Fenetre TCP 1" with lines ls 1, \
"res/tp1_winsize2.tr" title "Fenetre TCP 2" with lines ls 2, \
"res/tp1_winsize3.tr" title "FenetreTCP3" with lines ls 3

