BEGIN{
	print "set ter png";
	print "set pm3d map";
    print "set palette defined ( 0 '#000090',1 '#000fff',2 '#0090ff',3 '#0fffee',4 '#90ff70',5 '#ffee00',6 '#ff7000',7 '#ee0000',8 '#7f0000')";
	#print "unset key";  ## filename does not display
	for ( i=0; i<=300; i+=1) {
	    printf( "set out 'out_%d.png'\n", 1000 + i);
		printf( "splot 'out%d' using 1:2:($3**2+$4**2+$5**2+$6**2) with pm3d\n", i);
	}
}
