BEGIN{
	print "set ter png";
	print "set xrange[-6.0:16.0]";
	print "set yrange[-500.0:2500.0]";
#	for ( i=0; i<1000; i+=20 ) {
	for ( i=0; i<200; i+=1 ) {
		printf( "set out 'out%d.png'\n", i+1000);
		printf( "plot 'out%d' using 1:2 \n", i );
	}
}
