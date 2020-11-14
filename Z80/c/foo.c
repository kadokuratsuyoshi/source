/* define AKI-80 board (TMCZ84C015) SFR */
__sfr __at 0x00 P55A   ;
__sfr __at 0x01 P55B   ;
__sfr __at 0x02 P55C   ;
__sfr __at 0x03 P55CTL ;

__sfr __at 0x10 CTC_C0 ;
__sfr __at 0x11 CTC_C1 ;
__sfr __at 0x12 CTC_C2 ;
__sfr __at 0x13 CTC_C3 ;

__sfr __at 0x18 SIOAD ;
__sfr __at 0x19 SIOAC ;
__sfr __at 0x1a SIOBD ;
__sfr __at 0x1b SIOBC ;

__sfr __at 0x1C PIO_AD ;
__sfr __at 0x1D PIO_AC ;
__sfr __at 0x1E PIO_BD ;
__sfr __at 0x1F PIO_BC ;

typedef unsigned char  UBYTE ;
typedef unsigned short UWORD ;
typedef   signed char  SBYTE ;
typedef   signed short SWORD ;

#define LAST 0xFF

int main(void)
{
	UBYTE i;
	UBYTE sum ;

	PIO_AC = 0x00;
	sum = 0 ;
	for ( i = 0 ; i <= LAST ; i++ ) {
		sum += i;
		PIO_AD = sum ;
	}
	return 0;
}
