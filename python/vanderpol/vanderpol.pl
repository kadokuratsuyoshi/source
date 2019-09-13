$i = 0;
open(IN, "out_vdp");
while($a = <IN>) {
	#$a = <IN>;
	$a =~ s/\[//g;
	$a =~ s/\]//g;
	print "$i $a";
	$i = $i + 1;
}
close(IN);
