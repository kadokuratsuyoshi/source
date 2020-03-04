/*
	program:	ens.cpp
	contents:	ens.cpp for tb1c.cpp
	make:		g++ -fopenmp ens.cpp fft.cpp -o ens.exe
	usage:		ens input-data
*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <complex>
#include <cstring>
#include <ctime>
#include <time.h>

#define MAP (32)
#define MAXNX (64)
#define MAXNY (64)
#define SIZE (MAXNX*MAXNY)

using namespace std;
typedef complex <double> IM;
const int NUM = 4; // 16;
const complex <double> I(0, 1);
const int AR = MAP;
const int BR = MAP;
const double DX = 0.5;
const double DY = 0.5;
const int XN = 64;
const int YN = 64;
const double dkx = (2.0 * M_PI) / MAP;
const double dky = (2.0 * M_PI) / MAP;
IM **Psi0;
IM **Ck;
IM comp;
IM **dpsix;
IM **dpsiy;
IM **wx0;
IM **wy0;
IM **wz0;
double E[XN][YN];
double Ek[XN];
FILE *outputfile;
FILE *fp;
char fname[128];
time_t t1, t2;
int t3, hour, minute, sec;
char str[128];

extern void fft(complex <double> x[], int n, int flag);
extern void fft2D(complex  <double> x[], int n1, int n2, int flag);
extern void fft3D(complex  <double> x[], int n1, int n2, int n3, int flag);

IM** Malloc2D(int n1, int n2, IM* p)
{
    for (int i = 0; i < n1 * n2; i++) {
        p[i] = 0;
    }

    IM** p0 = new IM*[n1];
    for (int i = 0; i < n1; i++ ) {
    	p0[i] = p + i*n2;
    }
    return p0;
}

void Init()
{
	IM* p;
	p = new IM[SIZE];
	Psi0 = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	Ck = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	dpsix = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	dpsiy = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	wx0 = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	wy0 = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	wz0 = Malloc2D(XN, YN, p);

	for (int i = 0; i < XN; i++ ) {
		for (int j = 0; j < YN; j++) {
			Psi0[i][j] = 0.0 + I * 0.0;
			Ck[i][j] = 0.0 + I * 0.0;
			dpsix[i][j] = 0.0 + I * 0.0;
			dpsiy[i][j] = 0.0 + I * 0.0;
			wx0[i][j] = 0.0 + I * 0.0;
			wy0[i][j] = 0.0 + I * 0.0;
			wz0[i][j] = 0.0 + I * 0.0;
		}
	}
}

void kolmogorov( char *dstr )
{
	double x, y, kx, ky, dkx, dky;
	double dx = 0.5;
	double dy = 0.5;
	double dk;

	dkx = 2 * M_PI/(XN*dx);
	dky = 2 * M_PI/(YN*dy);
	dk = dkx;

	for (int l=0; l < XN; l++) {
		Ek[l] = 0;
	}

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i = 0; i < XN; i++){
		for(int j = 0; j < YN; j++){
			dpsix[i][j] = (Psi0[(i+1) % XN][j] - Psi0[i][j])/dx; 
			dpsiy[i][j] = (Psi0[i][(j+1) % YN] - Psi0[i][j])/dy;
			wx0[i][j] = imag(conj(Psi0[i][j]) * dpsix[i][j]) / sqrt(norm(Psi0[i][j]));
			wy0[i][j] = imag(conj(Psi0[i][j]) * dpsiy[i][j]) / sqrt(norm(Psi0[i][j]));
		}
	}
	/*
    fft2D(&wx0[0][0], XN, YN, 1);
    fft2D(&wy0[0][0], XN, YN, 1);
	for (int i=0;i < XN; i++) {
		double kx, ky;
		if (i < XN/2) {
			kx = dkx * i;
		}
		else {
			kx = dkx * (i - XN);
		} 
		for (int j=0;j < YN; j++) {
			if (j < YN/2) {
				ky = dky * j;
			}
			else {
				ky = dky * (j - YN);
			}
			double A0x = real(wx0[i][j]);
			double A0y = real(wy0[i][j]);
			//wx0[i][j] = wx0[i][j] - (kx * A0x + ky * A0y) / (kx * kx + ky * ky) * kx; 
	    	//wy0[i][j] = wy0[i][j] - (kx * A0x + ky * A0y) / (kx * kx + ky * ky) * ky;
		}
	}
    fft2D(&wx0[0][0], XN, YN, -1);
    fft2D(&wy0[0][0], XN, YN, -1);
	*/
	for (int i=0;i < XN; i++) {
		for (int j=0;j < YN; j++) {
			dpsix[i][j] = (wx0[(i+1) % XN][j] - wx0[i%XN][j])/dy; 
			dpsiy[i][j] = (wy0[i][(j+1) % YN] - wy0[i][j%YN])/dx;
			wz0[i][j] = dpsiy[i][j] - dpsix[i][j];
		}
	}
    fft2D(&wz0[0][0], XN, YN, 1);
	for (int i=0;i < XN; i++) {
		double kx, ky;
		if (i < XN/2) {
			kx = dkx * i;
		}
		else {
			kx = dkx * (i - XN);
		} 
		for (int j=0;j < YN; j++) {
			if (j < YN/2) {
				ky = dky * j;
			}
			else {
				ky = dky * (j - YN);
			}
			E[i][j] = (
					  norm(wz0[i][j])
					);  
			double B = kx * kx + ky * ky;
			double kk = sqrt(B);
			int a = int(kk / dk);
			Ek[a] += E[i][j];
		}
		if ( i % 10 == 0 ) {
			cerr << "p";
		}
	}
	cerr << endl;
  
	sprintf(fname, "%s.ens", dstr);
	fp = fopen(fname, "w");
	for (int l=0; l < XN; l++) {
		double k;
		if ( l == 0 ) {
			k = dk * 0.1;
		}
		else {
			k = dk * l;
		}
		fprintf(fp,"%f %f\n", k, Ek[l]);
		Ek[l] = 0;
		if (l % 10 == 0) {
			cerr << "o";
		}
	}
	cerr << endl;
	fclose(fp);

	cerr << "OUTPUT file :" << fname << endl;
	printf( "K41 process Done.\n" );
}

void print_elapsed_time(void)
{
	t2 = time(NULL);
	t3 = int(t2-t1);
	hour = t3 / 3600;
	minute = (t3 - hour*3600)/60;
	sec = t3 - hour*3600 - minute*60;
	sprintf( str, "%dh %dm %ds\n", hour, minute, sec);
	fprintf(outputfile, str);
	printf("%s", str);
}

int main(int argc, char *argv[])
{
	ifstream fin;
	double it = 0.0, t = 0.0, nextt = 0.0;
	char pstr[256], str[256];

	outputfile = fopen("status.txt", "a");
	if ( outputfile == NULL ) {
		printf("cannot open file, status.txt\n");
		exit(1);
	}
	t1 = time(NULL);
	printf( "\nargc = %d\n", argc );
	Init();
	sprintf( fname, argv[1] );
	printf( "INPUT file : %s\n", fname );
	fin.open(fname);
	if ( !fin ) {
		printf("can not open file : %s\n", argv[1]);
		exit(1);
	}
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
				double x, y, re0, im0, re1, im1, reck, imck;
				fin >> x >> y >> re0 >> im0 >> reck >> imck;
				Psi0[i][j] = re0 + I * im0;
				Ck[i][j] = reck + I * imck;
		}
		if ( i % 10 == 0 ) {
			printf("i");
		}
	}
	fin.close();
	cerr << endl;

	kolmogorov( argv[1] );
	
	printf( "end of >" );
	for (int i=0; i<argc; i++) {
		printf("%s ", argv[i] );
	}
	printf("\n");
	print_elapsed_time();
	fprintf(outputfile, str);
	fclose(outputfile);
	return 0;
}
// eof
