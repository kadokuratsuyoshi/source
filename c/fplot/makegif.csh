awk -f anm.awk > anm.cmd
gnuplot anm.cmd
#ffmpeg -r 9 -i out_1%3d.png -r 50 fig1d.mpg
convert out1*.png out.gif
