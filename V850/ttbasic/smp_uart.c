/*********************************************************************************
*
* (copyright)
* ================================================================================
* IDENTIFICATION
*
* Project   : 
* Title     : 
* File      : 
* --------------------------------------------------------------------------------
*
* $Revision:$
* $Date:$
*
*********************************************************************************/
#pragma ioreg
#pragma interrupt	INTUA1R		vINTUA1R_IntHdr
/*==============================================================================*/
/* include
/*==============================================================================*/
#include	<stdio.h>
/*==============================================================================*/
/* localdefine
/*==============================================================================*/

typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;
typedef unsigned long	U64;
typedef signed char		S8;
typedef signed short	S16;
typedef signed int		S32;

#define	UARTRECVBUFSIZE				( 20 )
#define	UARTRECVSTART				(  0 )
#define	UARTRECVSTOP				(  1 )
#define	CODE_CR						( 0x0D )

#define	UART_RESET					( 0x10 )
#define	UART_PWRON					( 0x80 )
#define	UART_TXENV					( 0x40 )
#define	UART_RXENV					( 0x20 )
#define	UART_DIRMSB					( 0x00 )
#define	UART_DIRLSB					( 0x10 )

#define	UART_PARITY_EVEN			( 0x0C )
#define	UART_PARITY_ODD				( 0x08 )
#define	UART_PARITY_NON				( 0x00 )
#define	UART_PARITY_ZERO			( 0x04 )

#define	UART_DATABIT_7				( 0x00 )
#define	UART_DATABIT_8				( 0x02 )

#define	UART_STOPBIT_1				( 0x00 )
#define	UART_STOPBIT_2				( 0x01 )

#define	UART_115200_HIGH			( 0x00 )
#define	UART_115200_LOW				( 0x57 )

/*==============================================================================*/
/* prototype
/*==============================================================================*/
__interrupt	void vINTUA1R_IntHdr( void );
			void vSample_uart( void );
			void vInit_uart( void );
			void vEcho_uart( void );
			void vPrint( S8* , ... );
//			void vUartSend( U8* , U16 );
//			void vUartRecvByte( void );

/*==============================================================================*/
/* localvariable
/*==============================================================================*/
U8	ucUartRxBuf[ UARTRECVBUFSIZE ];
U16	ucUartRxCnt	= 0;
U8	ucUartRecv	= UARTRECVSTART;

/*==============================================================================*/
/* Interrupt Proc.
/*==============================================================================*/
__interrupt void vINTUA1R_IntHdr( void )
{
	ucUartRxBuf[ ucUartRxCnt ]	= UA0RX;
	if( ucUartRxBuf[ ucUartRxCnt ]==CODE_CR )
	{
		ucUartRecv	= UARTRECVSTOP;
	}
	if( ucUartRxCnt<(UARTRECVBUFSIZE-1) )
	{
		ucUartRxCnt++;
	}
	UA1RIF	= 0;
}
/*==============================================================================*/
/* Program
/*==============================================================================*/
/*--------------------------------------------------------------------------------
* Function : 
* Descr    : 
* Inputs   : 
* Outputs  : 
* Return   : 
*-------------------------------------------------------------------------------*/
void vSample_uart( void )
{
	vInit_uart();
	vEcho_uart();
}
/*--------------------------------------------------------------------------------
* Function : 
* Descr    : 
* Inputs   : 
* Outputs  : 
* Return   : 
*-------------------------------------------------------------------------------*/
void vInit_uart( void )
{
	U16	i;
	/*======================================*/
	/*	UART 0 Register Initialize			*/
	/*======================================*/
	PMC3L.0 = 1;
	PMC3L.1 = 1;
	PFC3L.0 = 0;
	PFC3L.1 = 0;

	PF3L.0	= 1;
	PF3L.1	= 1;

	INTF3.1= 0;
	INTR3.1= 0;

	UA0CTL0		= UART_RESET;

	UA0CTL1		= UART_115200_HIGH;
	UA0CTL2		= UART_115200_LOW;

	UA0CTL0		= UART_DIRLSB | UART_PARITY_NON | UART_DATABIT_8 | UART_STOPBIT_1 ;
	UA0CTL0		= UA0CTL0 | UART_PWRON ;
	UA0CTL0		= UA0CTL0 | UART_TXENV | UART_RXENV ;
#if 0
	UA0CTL1 = 0x09;				/* 9600bps */
	UA0CTL2 = 0x41;				
	UA0CTL0 = 0x80;				/* N81 */
	UA0CTL0 = 0xF2;
#endif
	for (i=0; i<1000; i++);			/* •¶Žš‰»‚¯–hŽ~ƒEƒFƒCƒg */

	UA0TXE = 1;
	UA0RXE = 1;
}
/*--------------------------------------------------------------------------------
* Function : 
* Descr    : -
* Inputs   : -
* Outputs  : -
* Return   : -
*-------------------------------------------------------------------------------*/
void vEcho_uart( void )
{
//	UA0RMK		= 0;
	__EI();

	for( ; ; )
	{
	//	vUartRecvByte();
	}
}
/*--------------------------------------------------------------------------------
* Function : 
* Descr    : -
* Inputs   : -
* Outputs  : -
* Return   : -
*-------------------------------------------------------------------------------*/
void vPrint( S8 *fmt , ... )
{
	S8		msg[ 100 ];
	va_list	argp;
	S32		len;

	va_start( argp , fmt );
	len	= vsprintf( (char *)&msg , (const char*)fmt , argp );
	va_end( argp );
	//vUartSend( (U8*)msg , (U16)len );
}
/*--------------------------------------------------------------------------------
*-------------------------------------------------------------------------------*/
void uart_tx(unsigned char ch)
{
	unsigned char txf;

	UA0TX = ch;
	txf=1;
	while(txf==1){
		if(UA0TIF==1){
			UA0TIF=0;
			txf=0;
		}
	}
}
/*--------------------------------------------------------------------------------
* Function : 
* Descr    : -
* Inputs   : -
* Outputs  : -
* Return   : -
*-------------------------------------------------------------------------------*/
/*
void vUartSend( U8 *msg , U16 len )
{
	U16	usEscape	= 0xFFFF;

	while( len-- > 0 )
	{
		UA0TX	= *msg++;
		while( (!UA0TIF) && usEscape )
		{
			usEscape--;
		}
		UA0TIF	= 0;
	}
}
*/
/*--------------------------------------------------------------------------------
* Function : 
* Descr    : 
* Inputs   : 
* Outputs  : -
* Return   : -
*-------------------------------------------------------------------------------*/
/*
void vUartRecvByte( void )
{
	while( ucUartRecv != UARTRECVSTOP );

	__DI();

	vPrint( (S8*)"\r\nRecv[%d Bytes] echo Send >%s\r\n" , ucUartRxCnt , &ucUartRxBuf[0] );

	ucUartRxCnt	= 0;
	ucUartRecv	= UARTRECVSTART;
	memset( &ucUartRxBuf[0] , 0x00 , UARTRECVBUFSIZE );
	__EI();
}
*/
