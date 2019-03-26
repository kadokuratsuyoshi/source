/*
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

using namespace std;
typedef complex <double> IM;
const int NUM = 16;
const complex <double> I(0, 1);
const int AR = MAP;
const int BR = MAP;
const int CR = MAP;
const double DX = 0.5;
const double DY = 0.5;
const double DZ = 0.5;
const int XN = 64;
const int YN = 64;
const int ZN = 1;
const double dkx = (2.0 * M_PI) / MAP;
const double dky = (2.0 * M_PI) / MAP;
const double dkz = (2.0 * M_PI) / MAP;
IM ***Psi;
IM comp;
IM ***dpsix;
IM ***dpsiy;
IM ***dpsiz;
IM ***wx;
IM ***wy;
IM ***wz;
IM ***Ck;
double E[XN][YN][ZN];
double Ek[XN];
double cw, ccw;
FILE *outputfile;
FILE *fp;
char fname[128];
time_t t1, t2;
int t3, hour, minute, sec;
char str[128];

extern void fft(complex <double> x[], int n, int flag);
extern void fft2D(complex  <double> x[], int n1, int n2, int flag);
extern void fft3D(complex  <double> x[], int n1, int n2, int n3, int flag);

IM*** Malloc3D(int n1, int n2, int n3, IM* p)
{
	for (int i = 0; i < n1 * n2 * n3; i++) {
		p[i] = 0;
	}
	IM*** p0 = new IM**[n1];
	for (int i = 0; i < n1; i++ ) {
		p0[i] = new IM*[n2];
		for ( int j = 0; j < n2; j++ ) {
			p0[i][j] = p + j*n3 + i*n2*n3;
		}
	}
	return p0;
}

void InitialState()
{
	IM* p = new IM[XN*YN*ZN];
	Psi = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	dpsix = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	dpsiy = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	dpsiz = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	wx = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	wy = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	wz = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	Ck = Malloc3D(XN, YN, ZN, p);

	for (int i = 0; i < XN; i++ ) {
		for (int j = 0; j < YN; j++) {
            int k = 0;
				Psi[i][j][k] = 1.0 + I * 0.0;
				dpsix[i][j][k] = 0.0 + I * 0.0;
				dpsiy[i][j][k] = 0.0 + I * 0.0;
				wx[i][j][k] = 0.0 + I * 0.0;
				wy[i][j][k] = 0.0 + I * 0.0;
				Ck[i][j][k] = 0.0 + I * 0.0;
		}
	}
}

double CheckNormalization()
{
	double A = 0.0;

	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
            int k = 0;
				A += norm(Psi[i][j][k]);
		}
	}
	return A * DX * DY;
}

void Normalization()
{
	double A = 0.0;

	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
            int k = 0;
				A += norm(Psi[i][j][k]);
		}
	}
	A = sqrt((AR * BR) / (A * DX * DY));

	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
            int k = 0;
				Psi[i][j][k] *= A;
		}
	}
}

void kolmogorov( char *dstr )
{
	double x, y, z, kx, ky, kz, dkx, dky, dkz;
	double dx = 0.5;
	double dy = 0.5;
	double dk;

	dkx = 2 * M_PI/(XN*dx);
	dky = 2 * M_PI/(YN*dy);
	dk = dkx;

	for (int l=0; l < XN; l++) {
		Ek[l] = 0;
	}

	for(int i = 0; i < XN; i++){
		for(int j = 0; j < YN; j++){
            int k = 0;
				dpsix[i][j][k] = (Psi[(i+1) % XN][j][k] - Psi[i][j][k])/dx; 
				dpsiy[i][j][k] = (Psi[i][(j+1) % YN][k] - Psi[i][j][k])/dy;
				wx[i][j][k] = imag(conj(Psi[i][j][k]) * dpsix[i][j][k]) / sqrt(norm(Psi[i][j][k]));
				wy[i][j][k] = imag(conj(Psi[i][j][k]) * dpsiy[i][j][k]) / sqrt(norm(Psi[i][j][k]));
		}
	}
    fft3D(&wx[0][0][0], XN, YN, ZN, 1);
    fft3D(&wy[0][0][0], XN, YN, ZN, 1);
	for (int i=0;i < XN; i++) {
		double kx, ky, kz;
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
            int k = 0;
            kz = 0.0;
				double Ax = real(wx[i][j][k]);
				double Ay = real(wy[i][j][k]);
				double Az = real(wz[i][j][k]);
				wx[i][j][k] = wx[i][j][k] - (kx * Ax + ky * Ay) / (kx * kx + ky * ky) * kx; 
				wy[i][j][k] = wy[i][j][k] - (kx * Ax + ky * Ay) / (kx * kx + ky * ky) * ky;
				E[i][j][k] = 0.5 * (norm(wx[i][j][k]) + norm(wy[i][j][k]));  
				double B = kx * kx + ky * ky;
				double kk = sqrt(B);
				int a = int(kk / dk);
				Ek[a] += E[i][j][k];
		}
	}
  
	sprintf(fname, "%s.kol", dstr);
	fp = fopen(fname, "w");
	for (int l=0; l < XN; l++) {
		double k = dk * (l + 0.5);
		fprintf(fp,"%.10lf %.10lf\n", k, Ek[l]);
		Ek[l] = 0;
	}
	fclose(fp);
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

	outputfile = fopen("status.txt", "w");
	if ( outputfile == NULL ) {
		printf("cannot file\n");
		exit(1);
	}
	t1 = time(NULL);

	printf( "\nargc = %d\n", argc );
	InitialState();
	Normalization();

	sprintf( fname, argv[1] );
	printf( "filename : %s\n", fname );
	fin.open(fname);
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
				double x, y, z, re0, im0, re1, im1, pre, pim;
				fin >> x >> y >> re0 >> im0 >> re1 >> im1 >> pre >> pim;
				Psi[i][j][0] = re0 + I * im0;
		}
		printf(".");
	}
	fin.close();
	printf("\n");
	Normalization();
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
