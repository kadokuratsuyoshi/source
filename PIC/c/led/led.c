/*
	program:	led.c
	contents:	led blinking
	make:		ccsc led.c
	device:		PIC16F84A-20/P

              1:RA2       RA1:18
              2:RA3       RA0:17------>|----R(330)---GND
              3:RA4/TCKI CKIN:16-----X'tal(10MHz)
 +5V--R(10k)--4:/MCLR   CKOUT:15-----+
 GND----------5:GND       VCC:14-----------+-------+5V
              6:RB0/INT   RB7:13          C(10uF)
              7:RB1       RB6:12           |
              8:RB2       RB5:11          GND
              9:RB3       RB4:10

*/
#include <16f84a.h>
#fuses HS,NOWDT,PUT,NOPROTECT
#use delay(clock=3800000)

int main()
{
	set_tris_a(0x00);
	while(1)
	{
		output_low(PIN_A0);
		delay_ms(300);
		output_high(PIN_A0);
		delay_ms(300);
	}
	return 0;
}
