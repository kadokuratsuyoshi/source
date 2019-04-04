BEGIN{
    print "set ter png";
	print "set log";
	print "set xrange[0.09:11]";
	print "set yrange[0.01:1e4]";
	#print "unset key";  ## filename does not display
	for ( i=0; i<=300; i+=1) {
	    printf( "set out 'kol_%d.png'\n", 1000 + i);
		printf( "plot 'out%d.kol' using 1:2 with lines, 2e2*x**(-5./3.), 2e3*x**(-3.)\n", i);
	}
}
