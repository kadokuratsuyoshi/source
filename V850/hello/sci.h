/*
	filename:  sci.h
	contents:  uart for sci.c
*/

void vInit_uart(void);
void c_putch(unsigned char);
unsigned char c_kbhit(void);
unsigned char c_getch(void);
