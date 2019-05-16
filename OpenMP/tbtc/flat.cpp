/*
    filename:   vflat.cpp
    contents:   output flatness of velocity field by wavenumber
    make:       >g++ -fopenmp vflat.cpp fft.o -o vflat
    author:     1643002, T.Kadokura, kadokura@uec.hs.ac.jp
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
#include <string>
#include <limits>

#define MAXNX (64)
#define MAXNY (64)
#define SIZE (MAXNX*MAXNY)

using namespace std;
typedef complex <double> IM;
const int NUM = 4; // 16;
const complex <double> I(0, 1);
const int AR = MAXNX;
const int BR = MAXNY;
const double DX = 0.5;
const double DY = 0.5;
const int XN = MAXNX;
const int YN = MAXNY;
const double dkx = (2.0 * M_PI) / MAXNX;
const double dky = (2.0 * M_PI) / MAXNY;
// low wavenumber
const double LKKL = dkx * 7.5;
const double LKKH = dkx * 8.5;
// middle wavenumber
const double MKKL = dkx * 15.5;
const double MKKH = dkx * 16.5;
// high wavenumber
const double HKKL = dkx * 31.5;
const double HKKH = dkx * 32.5;

IM **Psi0;
IM **Psi1;
IM **Ck;
IM comp;
IM **dpsix;
IM **dpsiy;
IM **wx0;
IM **wy0;
IM **wx1;
IM **wy1;
double **v0;
double **v1;
double **flat;
double flatk[XN];
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

double** dMalloc2D(int n1, int n2, double* p)
{
	for (int i = 0; i < n1 * n2; i++) {
		p[i] = 0;
	}

	double** p0 = new double*[n1];
	for (int i = 0; i < n1; i++ ) {
		p0[i] = p + i*n2;
	}
    return p0;
}

void InitialState()
{
	IM* p;
	p = new IM[SIZE];
	Psi0 = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	Psi1 = Malloc2D(XN, YN, p);
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
	wx1 = Malloc2D(XN, YN, p);
	p = new IM[SIZE];
	wy1 = Malloc2D(XN, YN, p);

	double *dp;
	dp = new double[SIZE];
	v0 = dMalloc2D(XN, YN, dp);
	dp = new double[SIZE];
	v1 = dMalloc2D(XN, YN, dp);
	dp = new double[SIZE];
	flat = dMalloc2D(XN, YN, dp);

	for (int i = 0; i < XN; i++ ) {
		for (int j = 0; j < YN; j++) {
			Psi0[i][j] = 0.0 + I * 0.0;
			Psi1[i][j] = 0.0 + I * 0.0;
			Ck[i][j] = 0.0 + I * 0.0;
			dpsix[i][j] = 0.0 + I * 0.0;
			dpsiy[i][j] = 0.0 + I * 0.0;
			wx0[i][j] = 0.0 + I * 0.0;
			wy0[i][j] = 0.0 + I * 0.0;
			wx1[i][j] = 0.0 + I * 0.0;
			wy1[i][j] = 0.0 + I * 0.0;
			v0[i][j] = 0.0;
			v1[i][j] = 0.0;
			flat[i][j] = 0.0;
		}
	}
}

void Normalization()
{
	double A0 = 0.0;
	double A1 = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:A0, A1)
#endif
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
			A0 += norm(Psi0[i][j]);
			A1 += norm(Psi1[i][j]);
		}
	}
	A0 = A0 * (DX * DY);
	A0 = sqrt((AR*BR) / A0);
	A0 = A0 / sqrt(2.0);
	A1 = A1 * (DX * DY);
	A1 = sqrt((AR*BR) / A1);
	A1 = A1 / sqrt(2.0);

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
			Psi0[i][j] *= A0;
			Psi1[i][j] *= A1;
		}
	}
}

void flatness_velocity_field( char *dstr )
{
	double x, y, kx, ky, dkx, dky;
	double dx = 0.5;
	double dy = 0.5;
	double dk;

	dkx = 2 * M_PI/(XN*dx);
	dky = 2 * M_PI/(YN*dy);
	dk = dkx;

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int l=0; l < XN; l++) {
		flatk[l] = 0.0;
	}

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
			dpsix[i][j] = (Psi0[(i+1) % XN][j] - Psi0[i][j])/dx;
			dpsiy[i][j] = (Psi0[i][(j+1) % YN] - Psi0[i][j])/dy;
			wx0[i][j] = imag(conj(Psi0[i][j]) * dpsix[i][j]) / sqrt(norm(Psi0[i][j]));
			wy0[i][j] = imag(conj(Psi0[i][j]) * dpsiy[i][j]) / sqrt(norm(Psi0[i][j]));
			dpsix[i][j] = (Psi1[(i+1) % XN][j] - Psi1[i][j])/dx;
			dpsiy[i][j] = (Psi1[i][(j+1) % YN] - Psi1[i][j])/dy;
			wx1[i][j] = imag(conj(Psi1[i][j]) * dpsix[i][j]) / sqrt(norm(Psi1[i][j]));
			wy1[i][j] = imag(conj(Psi1[i][j]) * dpsiy[i][j]) / sqrt(norm(Psi1[i][j]));
		}
	}

	double uwx0 = 0.0;
	double uwy0 = 0.0;
	double uwx1 = 0.0;
	double uwy1 = 0.0;
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++ ) {
			uwx0 = uwx0 + sqrt(norm(wx0[i][j])); // real(wx0[i][j]);
			uwy0 = uwy0 + sqrt(norm(wy0[i][j])); // real(wy0[i][j]);
			uwx1 = uwx1 + sqrt(norm(wx1[i][j])); // real(wx1[i][j]);
			uwy1 = uwy1 + sqrt(norm(wy1[i][j])); // real(wy1[i][j]);
		}
	}
	double size = SIZE;
	uwx0 = uwx0 / size;
	uwy0 = uwy0 / size;
	uwx1 = uwx1 / size;
	uwy1 = uwy1 / size;
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++ ) {
			wx0[i][j] = (wx0[i][j] - uwx0) * (wx0[i][j] - uwx0);
			wy0[i][j] = (wy0[i][j] - uwy0) * (wy0[i][j] - uwy0);
			wx1[i][j] = (wx1[i][j] - uwx1) * (wx1[i][j] - uwx1);
			wy1[i][j] = (wy1[i][j] - uwy1) * (wy1[i][j] - uwy1);
		}
	}

    fft2D(&wx0[0][0], XN, YN, 1);
    fft2D(&wy0[0][0], XN, YN, 1);
    fft2D(&wx1[0][0], XN, YN, 1);
    fft2D(&wy1[0][0], XN, YN, 1);

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
			double A0x = norm(wx0[i][j]);
			double A0y = norm(wy0[i][j]);
			double A1x = norm(wx1[i][j]);
			double A1y = norm(wy1[i][j]);
			flat[i][j] = sqrt(A0x + A0y + A1x + A1y);

			double B = kx * kx + ky * ky;
			double kk = sqrt(B);
			int a = int(kk / dk);
			flatk[a] += flat[i][j];
		}
	}

	sprintf(fname, "%s.flat", dstr);
	fp = fopen(fname, "w");
	flatk[0] = 0.0/0.0;
	for (int l=0; l < XN; l++) {
		double k = dk * (l + 0.5);
		double size = SIZE;
		double dev = sqrt(flatk[l]/size);
		//double dev = flatk[l];
		if ( isnan(dev) ) {
			//fprintf(fp,"%.10lf %.10lf\n", k, 0.0 );
			fprintf(fp,"%.10lf %.10lf\n", k, dev );
		}
		else {
			fprintf(fp,"%.10lf %.10lf\n", k, dev );
		}
		flatk[l] = 0;
	}
	fclose(fp);
	printf( "flatness velocity field process Done.\n" );
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

	sprintf( fname, argv[1] );
	printf( "filename : %s\n", fname );
	fin.open(fname);
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
				double x, y, re0, im0, re1, im1, reck0, imck0, reck1, imck1;
				fin >> x >> y >> re0 >> im0 >> re1 >> im1 >> reck0 >> imck0 >> reck1 >> imck1;
				Psi0[i][j] = re0 + I * im0;
				Psi1[i][j] = re1 + I * im1;
		}
		printf("i");
	}
	fin.close();
	printf("\n");
	Normalization();
	flatness_velocity_field( argv[1] );

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
