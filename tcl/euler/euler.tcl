proc euler {} {
    set N 10
    set t 0.0
    set dt 0.01
    set x 1.0
    set v 0.0
    set dx  0
    set dv  0
    set k 1
    set m 1
    set a 1
    for {set t 0} {[expr $t <= $N]} { set t [expr $t + $dt] } {
        set dx [expr $v * $dt]
        set dv [expr (-1.0 * ($k/$m) * $x - ($a/$m) * $v) * $dt]
        set x  [expr $x + $dx]
        set v  [expr $v + $dv]
        puts "$t,$x,$v"
    }
}
euler
