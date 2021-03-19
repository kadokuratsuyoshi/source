#	program:   rk.pl

sub rk_v {
    ($t, $x, $v) = @_;
    return ( $v );
}

sub rk_a {
    ($t, $x, $v) = @_;
    $k = 1.0;
    $m = 1.0;
    $gamma = 1.0;
	return ( (-$k*$x-$gamma*$v)/$m );
}

$t = 0.0;
$x = 1.0;
$v = 0.0;
$dt = 0.01;
for ($n=0; $n<1000; $n++) {
    print "$t $x $v\n";
    ($kx[0]) = $dt * &rk_v($t,         $x,            $v);
    ($kv[0]) = $dt * &rk_a($t,         $x,            $v);
    ($kx[1]) = $dt * &rk_v($t+$dt/2.0, $x+$kx[0]/2.0, $v+$kv[0]/2.0);
    ($kv[1]) = $dt * &rk_a($t+$dt/2.0, $x+$kx[0]/2.0, $v+$kv[0]/2.0);
    ($kx[2]) = $dt * &rk_v($t+$dt/2.0, $x+$kx[1]/2.0, $v+$kv[1]/2.0);
    ($kv[2]) = $dt * &rk_a($t+$dt/2.0, $x+$kx[1]/2.0, $v+$kv[1]/2.0);
    ($kx[3]) = $dt * &rk_v($t+$dt,     $x+$kx[2],     $v+$kv[2]);
    ($kv[3]) = $dt * &rk_a($t+$dt,     $x+$kx[2],     $v+$kv[2]);
    $dx = ($kx[0]+2.0*($kx[1]+$kx[2])+$kx[3])/6.0;
    $dv = ($kv[0]+2.0*($kv[1]+$kv[2])+$kv[3])/6.0;
	$t = $t + $dt;
    $x = $x + $dx;
    $v = $v + $dv;
}
#eof
