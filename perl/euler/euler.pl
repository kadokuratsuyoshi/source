#   progname:   euler.pl
use strict;
use warnings;

my $t = 0.0;
my $x = 1.0;
my $v = 0.0;
my $dt = 0.01;
my $k = 1.0;
my $m = 1.0;
my $gamma = 1.0;
for (my $n=0; $n<1000; $n++) {
	print "$t $x $v\n";
	my $dx = $dt * $v;
	my $dv = $dt * ((-$k * $x - $gamma * $v)/$m);
	$t = $t + $dt;
	$x = $x + $dx;
	$v = $v + $dv;
}
#eof
