/*
	program:	main.c
	contents:	TOYOSHIKI TINY BASIC for CQ_V850
	make:		build on PM+
	usage:		write on FPL, power on CQ_V850
*/
#pragma ioreg
#include "ttbasic.h"
extern unsigned long _S_romp;

/*
#define	ADA0M0 (unsigned char*)0xfffff200H
#define	ADA0M1 (unsigned char*)0xfffff201H
#define	ADA0M2 (unsigned char*)0xfffff203H
#define	ADA0S (unsigned char*)0xfffff202H
#define	ADA0PFM (unsigned char*)0xfffff204H
#define	ADA0CR0 (unsigned char*)0xfffff210H
#define	ADA0CR1 (unsigned char*)0xfffff212H
#define	ADA0CR2 (unsigned char*)0xfffff214H
#define	ADA0CR3 (unsigned char*)0xfffff216H
#define	ADA0CR4 (unsigned char*)0xfffff218H
#define	ADA0CR5 (unsigned char*)0xfffff21AH
#define	ADA0CR6 (unsigned char*)0xfffff21CH
#define	ADA0CR7 (unsigned char*)0xfffff21EH
#define	ADA0CR8 (unsigned char*)0xfffff220H
#define	ADA0CR9 (unsigned char*)0xfffff222H
#define	ADA0CR10 (unsigned char*)0xfffff24H
#define	ADA0CR11 (unsigned char*)0xfffff26H
#define	ADA0PFT (unsigned char*)0xfffff205H
*/

void init_ad()
{
	PM7L=0xFF; /* AN0 -- AN7 */
	PM7H=0x0F; /* AN8 -- AN11 */
	ADA0M0=0x20;
	ADA0M1=0x00; /* interval time 15.5ms */
	ADA0M2=0x00;
	ADA0S=0x00; /* ANI0 */
	ADA0PFM=0x00;
	ADA0PFT=0x00;
}

void init_timer()
{
	PRSCM0 = 19;	// prescaler, fBRG = 32894Hz
	PRSM0 = 0x12;
	WTM = 0x87;		// {WTM7, WTM3, WTM2} = 101
	WTIC = 0;		// enable interrupt
	__EI();
}

void init_gpio()
{
	PMCT.6	=	0;		// output mode
}

void init_pwm()
{
	TP2CE = 0; // stop timer
	PM9L.7 = 0;
	PMC9L.7 = 1; // set P97 TOP2 output
	PFC9L.7 = 1;
	PFCE9L.7 = 1;
	TP2CTL0 = 0x05; // frequency 0x00==fxx, 0x04==fxx/16, 0x05==fxx/32
	TP2CTL1 = 0x04; // pwm mode
	TP2IOC0 = 0x01; // 0x01 | 0x02; // 0x02 output enable if low level start
	TP2CCR0 = 16384-1; // 500-1; // 40kHz
	TP2CCR1 = 16384/2; // 500/2; // duty == 50%
	//TP2CCIC0 = 0x07;
}

void main( void )
{
	int ret;
	
	/* initialize CPU */
	VSWC	= 1;
	WDTM2	= 0;
	__asm("mov	0x00, r11");
	__asm("st.b	r11,  PRCMD");
	__asm("st.b	r11,  PCC");
	while( LOCK );
	SELPLL	= 1;
	ret	= _rcopy( &_S_romp, -1 );

	// initialize GPIO for LED port
	init_gpio();

	// initialize A/D converter
	init_ad();
	
	// initialize timer as interrupt
	init_timer();

	// for UART
	vInit_uart();

	// for PWM
	init_pwm();

	while(1){
		// TOYOSHIKI TINY BASIC
		basic();
	}
}

#pragma interrupt INTWT intwt_handler
__interrupt void intwt_handler(void) {
	PCT.6 ^=1 ;
}
