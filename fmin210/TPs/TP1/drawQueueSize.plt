set terminal png
set grid
set xlabel "temps (s)" ; set ylabel "Nombre d'octets dans la file"
#set xrange[0:10.2]
set yrange[0:110]

set output 'queueSize.png'
plot 'queueSize1.txt' using 1:2 with lines title "frequence 0.01" 1, 'queueSize5.txt' using 1:2 with lines title "frequence 0.05" 2