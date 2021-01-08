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

int	adc;

void ad_init()
{
	PM7L=0xFF; /* AN0 -- AN7 */
	PM7H=0x0F; /* AN8 -- AN11 */
	ADA0M0=0x20;
	ADA0M1=0x00; /* •ÏŠ·ŠÔ 15.5 */
	ADA0M2=0x00;
	ADA0S=0x00; /* ANI0 */
	ADA0PFM=0x00;
	ADA0PFT=0x00;
}

int ad_get()
{
	int x;
	if(adc==0)
		x=ADA0CR0;
	if(adc==1)
		x=ADA0CR1;
	if(adc==2)
		x=ADA0CR2;
	if(adc==3)
		x=ADA0CR3;
	if(adc==4)
		x=ADA0CR4;
	if(adc==5)
		x=ADA0CR5;
	if(adc==6)
		x=ADA0CR6;
	if(adc==7)
		x=ADA0CR7;
	if(adc==8)
		x=ADA0CR8;
	if(adc==9)
		x=ADA0CR9;
	if(adc==10)
		x=ADA0CR10;
	if(adc==11)
		x=ADA0CR11;
	x = x >> 6;
	return x;
}

void main( void )
{
	int		ret,x,i;
	int		msec;
	int		sec;
	unsigned char	mes[20];

	/* CPU‰Šú‰» */
	VSWC	= 1;
	WDTM2	= 0;
	__asm("mov	0x00, r11");
	__asm("st.b	r11,  PRCMD");
	__asm("st.b	r11,  PCC");
	while( LOCK );
	SELPLL	= 1;
	ret	= _rcopy( &_S_romp, -1 );

	mes[0]=' ';
	mes[1]=' ';
	mes[6]=0x0d;
	mes[7]=0x0a;

	/* LEDÎß°Ä ‰Šú‰» */
	PMCT.6	=	0;		// o—ÍÎß°ÄÓ°ÄŞ
	vInit_uart();

	while(1){
	  basic();
	  /*
	  adc=0x00;
	
	  for(i=0; i<11; i++ )
	  {
	  	ADA0S=adc;
	    ADA0M0.7=1;
    	for( msec=0;msec<600000;msec++ ) __nop();
		PCT.6	^= 1;
		x=ad_get();
		ADA0M0.7=0;
		mes[2]=x/1000+'0';
		mes[3]=(x%1000)/100+'0';
		mes[4]=(x%100)/10+'0';
		mes[5]=x%10+'0';
		vUartSend( (unsigned char*)&mes , 6 );
		adc++;
	  }	
	  	ADA0S=adc;
	    ADA0M0.7=1;
    	for( msec=0;msec<600000;msec++ ) __nop();
		PCT.6	^= 1;
		x=ad_get();
		ADA0M0.7=0; 
		mes[2]=x/1000+'0';
		mes[3]=(x%1000)/100+'0';
		mes[4]=(x%100)/10+'0';
		mes[5]=x%10+'0';
		vUartSend( (unsigned char*)&mes , 8 );
	  */
	}
}
