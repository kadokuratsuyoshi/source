BEGIN{
}
{
	addr = substr($0,4,4);
	if (addr != "0000") {
		printf("%s\n:", addr);
		nc_str = "0x" substr($0,2,2)
		nc = strtonum(nc_str);
		for (i=0; i<nc; i++) {
			printf("%s ", substr($0,10+(i*2), 2));
		}
		printf("\n");
	}
}
END{
}
