awk -f makekol.awk > kolgen.bat
call kolgen
awk -f kol.awk > kol.gpi
gnuplot kol.gpi
magick convert kol_*.png kol.gif
rem #convert -loop 0 -delay 50 -out_*.png out.gif
