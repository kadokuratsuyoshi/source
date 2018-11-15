#! /bin/csh
set t=`echo "0.0" | bc -l`
set dt=`echo "0.01" | bc -l`
set x=`echo "1.0" | bc -l`
set v=`echo "0.0" | bc -l`
set dx=`echo "0.0" | bc -l`
set dv=`echo "0.0" | bc -l`
set k=`echo "1.0" | bc -l`
set m=`echo "1.0" | bc -l`
set a=`echo "1.0" | bc -l`
set i=1
while ($i <= 1000)
    set dx=`echo "$v * $dt" | bc -l`
    set dv=`echo "(-1.0 * ($k / $m) * $x - ($a / $m) * $v) * $dt" | bc -l` 
    set x=`echo "$x + $dx" | bc -l`
    set v=`echo "$v + $dv" | bc -l`
    set t=`echo "$t + $dt" | bc -l`
    echo $t $x $v
    @ i++
end
