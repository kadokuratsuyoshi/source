/*
	program:	main.c
	contents:	hello, world for CQ_V850
	make:		build on PM+
	usage:		write on FPL, power on CQ_V850
*/
#pragma ioreg
#include "sci.h"

extern unsigned long _S_romp;

void main( void )
{
	int ret;
	int i;
	unsigned char c;
	char str[256];
	
	/* initialize CPU */
	VSWC	= 1;
	WDTM2	= 0;
	__asm("mov	0x00, r11");
	__asm("st.b	r11,  PRCMD");
	__asm("st.b	r11,  PCC");
	while( LOCK );
	SELPLL	= 1;
	ret	= _rcopy( &_S_romp, -1 );

	// for timer interrupt
	PRSCM0 = 19;	// prescaler, fBRG = 32894Hz
	PRSM0 = 0x12;
	WTM = 0x87;		// {WTM7, WTM3, WTM2} = 101
	WTIC = 0;		// enable interrupt
	__EI();

	// initialize LED port
	PMCT.6	=	0;		// set output port
	
	// for UART
	vInit_uart();

	sprintf(str, "hello, world\r\n");
	i = 0;
	while(str[i]!=0x00){
		c_putch(str[i]);
		i++;
	}
	while(1){
		c = c_getch();
		c_putch(c);
		if ( c == '\r' ) {
			c_putch('\n');
		}
	}
}

#pragma interrupt INTWT intwt_handler
__interrupt void intwt_handler(void) {
	PCT.6 ^=1 ;
}
