#set yrange[0.0:1.5] 
plot 'euler.txt' using 1:2 with lines, \
    'euler.txt' using 1:3 with lines
pause -1
