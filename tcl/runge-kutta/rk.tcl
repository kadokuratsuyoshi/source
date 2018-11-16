proc rk_v { t x v } {
    return  $v 
}

proc rk_a { t x v } {
    set k 1.0
    set m 1.0
    set a 1.0
    return [ expr -1.0 * ($k/$m)*$x - ($a/$m)*$v ]
}

proc rk {} {
    set N 10
    set t 0.0
    set dt 0.01
    set x 1.0
    set v 0.0
    set dx 0.0
    set dv 0.0
    for {set t 0} {[expr $t <= $N]} { set t [expr $t + $dt] } {
        set k1(1) [expr $dt * [rk_v $t $x $v] ]
        set k1(2) [expr $dt * [rk_a $t $x $v] ]
        set k2(1) [expr $dt * [rk_v $t+$dt/2.0 $x+$k1(1)/2.0 $v+$k1(2)/2.0] ]
        set k2(2) [expr $dt * [rk_a $t+$dt/2.0 $x+$k1(1)/2.0 $v+$k1(2)/2.0] ]
        set k3(1) [expr $dt * [rk_v $t+$dt/2.0 $x+$k2(1)/2.0 $v+$k2(2)/2.0] ]
        set k3(2) [expr $dt * [rk_a $t+$dt/2.0 $x+$k2(1)/2.0 $v+$k2(2)/2.0] ]
        set k4(1) [expr $dt * [rk_v $t+$dt $x+$k3(1) $v+$k3(2)] ]
        set k4(2) [expr $dt * [rk_a $t+$dt $x+$k3(1) $v+$k3(2)] ]
        set dx [expr ($k1(1)+2.0*$k2(1)+2.0*$k3(1)+$k4(1))/6.0 ]
        set dv [expr ($k1(2)+2.0*$k2(2)+2.0*$k3(2)+$k4(2))/6.0 ]
        set x [expr $x+$dx]
        set v [expr $v+$dv]
        puts "$t,$x,$v"
    }
}
rk
