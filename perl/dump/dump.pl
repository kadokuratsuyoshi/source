use strict;
use warnings;

sub isPrint
{
	my $str = shift || return(undef);
	#if ( $str =~ /^[a-zA-Z]{1,}$/) { # isAlpha
	if ( $str =~ /^[\x21-\x7e]{1,}$/) { # isPrint
		return(1);
	}
	else {
		return(0);
	}
}

if (@ARGV == 0 ) {
	print "perl dump.pl filenmae\n";
	exit;
}

my $fin = shift @ARGV;
my $str;
open(FILE, $fin) or die "$fin: $!";

binmode(FILE);

my $count = 0;
while (defined(my $chin = getc(FILE)) ) {
	if (isPrint($chin)) {
		$str = $str.$chin;
	}
	else {
		$str = $str.".";
	}
	if ($count % 16 == 0) {
		printf("%08X : ", $count);
	}
	printf("%02X ", ord($chin));
	if ($count % 16 == 7) {
		printf("- ");
	}
	if ($count % 16 == 15) {
		printf("  %s", $str);
		printf("\n");
		$str = "";
	}
	$count++;
}
close(FILE);
printf("\n");
#eof
