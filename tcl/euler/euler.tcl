proc euler {} {
	set t 0.0
	set x 1.0
	set v 0.0
	set dt 0.01
	set k 1.0
	set m 1.0
	set gamma 1.0
	for {set n 0} {[expr $n <= 1000]} { set n [expr $n + 1] } {
		puts "$t $x $v"
		set dx [expr $dt * $v]
		set dv [expr $dt * ((-$k*$x-$gamma*$v)/$m)]
		set t  [expr $t + $dt]
		set x  [expr $x + $dx]
		set v  [expr $v + $dv]
	}
}
euler
