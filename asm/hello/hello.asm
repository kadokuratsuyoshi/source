;
;   filename:   hello.asm
;   contents:   display "hello, world"
;
;   assemble:   >nasm -g -f elm64 -o hello.o hello.asm 
;               >gcc -o hello.exe hello.o
;   usage:      >./hello.exe
;
 section .data
    msg db      "hello, world",0x0d, 0x0a, 0

 section .text
    global main
 main:
    mov     rax, 1
    mov     rdi, 1
    mov     rsi, msg
    mov     rdx, 16
    syscall

    mov    rax, 60
    mov    rdi, 0
    syscall
;   eof
