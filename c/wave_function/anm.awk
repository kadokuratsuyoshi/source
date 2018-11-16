BEGIN{
	print "set ter png";
	print "set xrange[-1.0:1.0]";
	print "set yrange[-3.0:50.0]";
#	for ( i=0; i<1000; i+=20 ) {
	for ( i=0; i<40; i+=1 ) {
		printf( "set out 'out_%d.png'\n", i+1000);
		printf( "plot 'wf_%d' using 1:2 with lines,'potential.dat' using 1:2 with lines \n", i );
	}
}
