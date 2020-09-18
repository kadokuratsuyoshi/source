PIOAD:  equ     0
PIOAC:  equ     1

        org     0
RESET:
        ld      sp,0
;       8255A PIO setting
        ld      a,11001111b ;bit mode setting
        out     PIOAC,a
        ld      a,00001111b ;0:output, 1:input
        out     PIOAC,a
        ld      a,0
        ld      b,0
MAIN:
        inc     a
        out     PIOAD,a
DELAY:
        inc     b
        jp      nz,DELAY
        jp      MAIN
