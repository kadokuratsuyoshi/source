BEGIN{
	print "set ter png";
	print "set pm3d map";
	print "set palette gray";
	print "set cbrange[-pi:pi]";
	#print "unset key";  ## filename does not display
	for ( i=0; i<=582; i+=1) {
	    printf( "set out 'out_%d.png'\n", 1000 + i);
		printf( "splot 'out%d' using 1:2:(atan2($4,$3)) with pm3d\n", i);
	}
}
