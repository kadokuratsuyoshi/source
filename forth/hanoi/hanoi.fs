( gforth, >include hanoi.fs, >1 2 3 3 hanoi )
variable cnt

: hanoi-print
    cr 2dup . ." -> " . ;

: hanoi-impl
    dup >r 0> if
        rot swap r@ 1- recurse
        cnt @ 1+ cnt ! 
	hanoi-print
        rot r@ 1- recurse
        swap
    then
    r> drop ;

: hanoi
    0 cnt !
    hanoi-impl drop drop drop
    cr ." count: " cnt @ . cr ;
