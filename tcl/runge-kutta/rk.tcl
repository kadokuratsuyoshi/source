#
proc rk_v { t x v } {
    return  $v 
}

proc rk_a { t x v } {
    set k 1.0
    set m 1.0
    set gamma 1.0
    return [expr ((-$k*$x-$gamma*$v)/$m) ]
}

proc rk {} {
    set t 0.0
    set x 1.0
    set v 0.0
    set dt 0.01
    for {set n 0} {[expr $n <= 1000]} { set n [expr $n + 1] } {
        puts "$t $x $v"
        set kx(1) [expr $dt * [rk_v $t          $x            $v          ] ]
        set kv(1) [expr $dt * [rk_a $t          $x            $v          ] ]
        set kx(2) [expr $dt * [rk_v ($t+$dt/2) ($x+$kx(1)/2) ($v+$kv(1)/2)] ]
        set kv(2) [expr $dt * [rk_a ($t+$dt/2) ($x+$kx(1)/2) ($v+$kv(1)/2)] ]
        set kx(3) [expr $dt * [rk_v ($t+$dt/2) ($x+$kx(2)/2) ($v+$kv(2)/2)] ]
        set kv(3) [expr $dt * [rk_a ($t+$dt/2) ($x+$kx(2)/2) ($v+$kv(2)/2)] ]
        set kx(4) [expr $dt * [rk_v ($t+$dt)   ($x+$kx(3))   ($v+$kv(3))  ] ]
        set kv(4) [expr $dt * [rk_a ($t+$dt)   ($x+$kx(3))   ($v+$kv(3))  ] ]
        set dx [expr ($kx(1)+2*($kx(2)+$kx(3))+$kx(4))/6 ]
        set dv [expr ($kv(1)+2*($kv(2)+$kv(3))+$kv(4))/6 ]
		set t [expr $t+$dt]
        set x [expr $x+$dx]
        set v [expr $v+$dv]
    }
}
rk
