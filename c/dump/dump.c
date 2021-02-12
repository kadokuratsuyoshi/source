/*
    program:	dump.c
    contents:   hexadecimal notation print out
    make:		gcc -o dump dump.c
    usage:		./dump filename
*/
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char **argv) {
	unsigned char buf[16];
	int nbytes = 16;
	int i, j, ch;
	FILE *fp;
	unsigned int row, column[16];
	int line_cnt=0;
  
	for (i=0; i<16; i++) {
		column[i]=0;
	}
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("can not open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	for (i = 0; nbytes == 16; i++) {
		printf("%06X : ", i*16);
		nbytes = fread(buf, sizeof(char), 16, fp);
		row = 0;
		for (j = 0; j < 16; j++) {
			if (j < nbytes) {
				printf("%02X ", buf[j]);
            }
            else {
				printf("   ");
            }
			if (j == 7) {
				printf(" ");
            }
			row += buf[j];
			column[j] += buf[j];
		}
		printf(":%02X    ", row % 0x100);
		for ( j=0; j<16; j++) {
			if (j < nbytes) {
				ch = buf[j] & 0xFF;
				if ((32 < ch) && (ch < 127)) {
					printf("%c", ch);
                }
                else if ( ch == 0x20 ) {
                    printf(" ");
                }
				else {
					printf(".");
                }
			}
			buf[j]=0;
		}
		printf("\n");
		line_cnt++;
		if (line_cnt > 15 ) {
			line_cnt=0;
			printf("---------------------------------------------------------\n");
			printf("         ");
			for ( j=0; j<16; j++) {
				if ( j==8 ) {
					printf(" ");
				}
				printf("%02X ",column[j] % 0x100);
				column[j] = 0;
			}
			printf("\n\n");
		}
	}
	if ( line_cnt != 0 ) {
		printf("---------------------------------------------------------\n");
		printf("         ");
		for ( j=0; j<16; j++) {
			if ( j==8 ) {
				printf(" ");
			}
			printf("%02X ",column[j] % 0x100);
			column[j] = 0;
		}
		printf("\n");
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
