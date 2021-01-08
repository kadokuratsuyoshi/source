/*
	Filename: TinyBASIC/sci.c
	Language: MPLAB XC16(C30)
*/
#pragma ioreg

#include <ctype.h>

#define BUF_SIZE 64

struct {
	char buf[BUF_SIZE];
	unsigned char wp;
	unsigned char rp;
	unsigned char dc;
} RX_BUF;

//PICkit2 UART tool
void sci2_init(void)
{
/*
*/
}

void putch2(char c){
/*
	while(U2STAbits.UTXBF);
	U2TXREG = c;
*/
}


unsigned char kbhit2(void){
	return(UA0RIF);  // 1==rx_req ON, 0==rx_req OFF
}
char getch2(void){
	char c;
	unsigned char rxf;
	unsigned int led_cnt;
	
	while(!kbhit2()){
		led_cnt++;
		if (led_cnt > 0xFF00) {
			led_cnt = 0;
			PMCT.6 ^= 1;
		}
	}
	c = UA0RX;
	UA0RIF=0;
	
	return c;
}
