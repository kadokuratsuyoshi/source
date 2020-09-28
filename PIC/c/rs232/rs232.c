/*
	program:	rs232.c
	contents:	rs232c tx/rx test
	make:		ccsc led.c
	device:		PIC16F84A-20/P

              1:RA2       RA1:18
 TX(XMIT)-----2:RA3       RA0:17------>|----R(330)---GND
 RX(RCV)------3:RA4/TCKI CKIN:16-----X'tal(3.8MHz)
 +5V--R(10k)--4:/MCLR   CKOUT:15-----+
 GND----------5:GND       VCC:14-----------+-------+5V
              6:RB0/INT   RB7:13          C(10uF)
              7:RB1       RB6:12           |
              8:RB2       RB5:11          GND
              9:RB3       RB4:10

*/
#include <16f84a.h>
#include <stdlib.h>
#fuses HS,NOWDT,PUT,NOPROTECT
#use delay(clock=3800000)
#use RS232(BAUD=9600, XMIT=PIN_A3, RCV=PIN_A4)
#define CNT 10

int count;
int toggle;
int cmnd;

#INT_RTCC
timer0_int()
{
	count = count - 1;
	if (count == 0) {
		if ( toggle != 0 ) {
			output_high(PIN_A0);
			toggle = 0;
		}
		else {
			output_low(PIN_A0);
			toggle = 1;
		}
		count = cmnd;
	}
}


int main()
{
	char str[8];

	set_tris_a(0x10);
	cmnd = CNT;
	count = cmnd;
	toggle = 1;
	set_rtcc(0);
	setup_counters(RTCC_INTERNAL, RTCC_DIV_128);
	enable_interrupts(INT_RTCC);
	enable_interrupts(GLOBAL);

	printf("\r\nPIC16F84A LED blink with RS232C communication\r\n");
	while(1)
	{
		printf("\r\ncmd (10,0-255)@ ");
		gets(str);
		cmnd = atoi(str);
		printf(" Input= %d", cmnd);
		count = cmnd;
	}
	return 0;
}
