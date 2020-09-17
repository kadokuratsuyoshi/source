        .cpu        300HA
        .section    prog4, code, locate=h'000000
P1DR    .equ        h'ffffc2
P1DDR   .equ        h'ffffc0
        .section    rom, code, locate=h'000100
        mov.l       #h'ffff00, ER7  ; sp setting
        mov.b       #h'ff, R0L
        mov.b       R0L, @P1DDR
        mov.b       #b'01111111, R0L
LOOP:   mov.b       R0L, @P1DR
        jsr         @TIM2
        rotr.b      R0L
        jmp         @LOOP
TIM2:   mov.w       #d'500, ER5
L2:     jsr         @TIM1
        dec.w       #1, ER5
        bne         L2
        rts
TIM1:   mov.l       #d'20000, ER6
L1:     dec.l       #1, ER6
        nop
        bne         L1
        rts
        .end
