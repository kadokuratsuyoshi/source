BEGIN{
	sum=0;
	j=0;
}
{
	line[NR]=$1;
	i=NR;
	if ( (i>=300) && (i<400) ) {
		sum+=$1;
		j++;
	}
	#sum+=$3;
}
END{
	ave = sum/j; # NR;
	varTemp=0;
	for (i=1; i<=NR; i++) {
		if ( i>=300 || i<400 ) {
			varTemp+=(line[i]-ave)^2;
		}
		#varTemp+=(line[i]-ave)^2;
	}
	var=varTemp/j; # NR;
	std=sqrt(var);
	printf("NR\t\t\t sum\t\t ave\t\t var\t\t std\t\t j\n");
	printf("%d\t\t %.2f\t\t %.2f\t\t %.2f\t %.2f\t\t %d\n", NR, sum, ave, var, std, j);
}
