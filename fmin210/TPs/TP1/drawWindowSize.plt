set terminal png
set grid


set output 'windowSize.png'
plot 'windowSize.txt' using 1:2 with lines title 'Fenêtre TCP1' 2, 'windowSize.txt' using 1:3 with lines title 'Fenêtre TCP2' 6, 'windowSize.txt' using 1:4 with lines title 'Fenêtre TCP3' 8
