#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>

#define FILE_S (0)
#define FILE_F (9)
#define FILE_N (FILE_F - FILE_S)
#define DATA_N (11)

using namespace std;

int main(void) {
	int fcnt, n, i;
	double x, p[FILE_N];
	double total, ave;
	char fname[256];
	ifstream fin;
	ofstream fout;

	for (i=0; i<FILE_N; i++ ) {
		p[i] = 0.0;
	}
	total = 0.0;
	for (fcnt=FILE_S; fcnt<=FILE_F; fcnt++ ) {
		sprintf(fname, "out%d", fcnt);
		fin.open(fname);
		if ( fin.fail() ) {
			cout << "can not open " << fname << endl;
			return 1;
		}
		n = 0;
		while(true) {
			fin >> x;
			p[n] = p[n] + x;
			total = total + x;
			if ( fin.fail() ) {
				break;
			}
			else {
				n++;
			}
		}
		fin.close();
	}
	x = 0.0;
	for ( n=0; n<=DATA_N; n++ ) {
		printf("%d %4.1f\n", n, p[n]/DATA_N );
		total = total + p[n];
	}
	printf("FILE N:%d, total=%4.1f\n", FILE_N, total);
	printf("(0+1+2+3+4+5+6+7+8+9)/11=%4.1f\n",
	(0.0+1.0+2.0+3.0+4.0+5.0+6.0+7.0+8.0+9.0)/11.0);
	printf("(10+20+30+40+50+60+70+80+90+100)/11=%4.1f\n",
	(10.0+20.0+30.0+40.0+50.0+60.0+70.0+80.0+90.0+100.0)/11.0);

	return 0;
}
