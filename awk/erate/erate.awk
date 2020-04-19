BEGIN{
	tt_cnt = 0;
	old_tt_cnt = 0;
	startf = 0;
}
{
	tt_cnt = $7;
	if ( (tt_cnt == 10) && (old_tt_cnt == 9) ) {
		startf = 1;
		i=0;
		start = $2;
	}
	if (startf == 1) {
		i++;
	}
	if ((i == 100) && (startf == 1)) {
		startf = 0;
		stop = $2;
	}
	old_tt_cnt = tt_cnt;
}
END{
	erate = (start - stop)/10.0;
	printf("start\t\t stop\t\t energy rate\t i\n");
	printf("%.2f\t\t %.2f\t\t %.2f\t\t\t %d\n", start, stop, erate, i);
}
