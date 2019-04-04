awk -f anm.awk > anm.gpi
gnuplot anm.gpi
magick convert out_*.png out.gif
rem #convert -loop 0 -delay 50 -out_*.png out.gif
