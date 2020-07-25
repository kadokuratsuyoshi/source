#!/usr/bin/awk -f
#
#	progname:	wf.awk
#	contents:	plot the water-fall results
#	usage:		awk -f wf.awk > gowf
#				chmod +x gowf
#				gowf
#				gnuplot 
#				set pm3d map
#				set log x
#				set cbrange[0:3e10]
#				splot 'all.kol' using 1:2:3 with pm3d
#
BEGIN{
    for (i=0; i<=1000; i+=5) {
        #printf("../kol out%d\n", i)
		printf("cat out%d.kol | awk '{if(NR!=1){print $1 \" \" %d \" \" $2}}' > aout%d.kol\n", i, i, i)
		printf("cat aout%d.kol all.kol >> all.kol\n", i)
		printf("awk 'BEGIN{printf(\"\\n\")}' >> all.kol\n")
    }
}
