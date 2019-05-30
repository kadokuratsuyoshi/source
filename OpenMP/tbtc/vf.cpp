/*
    filename:   vf.cpp
    contents:   output velocity field by wavenumber
    make:       >g++ -fopenmp vf.cpp fft.o -o vf
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
double flat[XN];
double flat_max = 0.0;
double **E;
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
	E  = dMalloc2D(XN, YN, dp);

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
			E[i][j] = 0.0;
		}
	}
}

void velocity_field( char *option, char *dstr )
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
		Ek[l] = 0.0;
		flat[l] = 0.0;
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
			dpsix[i][j] = (Psi1[(i+1) % XN][j] - Psi1[i][j])/dx;
			dpsiy[i][j] = (Psi1[i][(j+1) % YN] - Psi1[i][j])/dy;
			wx1[i][j] = imag(conj(Psi1[i][j]) * dpsix[i][j]) / sqrt(norm(Psi1[i][j]));
			wy1[i][j] = imag(conj(Psi1[i][j]) * dpsiy[i][j]) / sqrt(norm(Psi1[i][j]));
		}
	}

    fft2D(&wx0[0][0], XN, YN, 1);
    fft2D(&wy0[0][0], XN, YN, 1);
    fft2D(&wx1[0][0], XN, YN, 1);
    fft2D(&wy1[0][0], XN, YN, 1);

	double vf_min = 0.0/0.0; // NaN
	double vf_max = 0.0;
	double vf_ratio = 0.0;
    double vf_ave = 0.0;
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

			double B = kx * kx + ky * ky;
			double kk = sqrt(B);
            double kkl, kkh;
            string option_k = "-k";
            string option_l = "-l";
            string option_m = "-m";
            string option_h = "-h";
            if ( !option_l.compare(option) ) {
                kkl = LKKL;
                kkh = LKKH;
	            sprintf(fname, "%s.vfl", dstr);
            }
            else if ( !option_m.compare(option) ) {
                kkl = MKKL;
                kkh = MKKH;
	            sprintf(fname, "%s.vfm", dstr);
            }
            else if ( !option_h.compare(option) ) {
                kkl = HKKL;
                kkh = HKKH;
	            sprintf(fname, "%s.vfh", dstr);
            }
            else {
                kkl = LKKL;
                kkh = LKKH;
	            sprintf(fname, "%s.vfl", dstr);
            }
			if ((kk < kkl) || (kkh < kk)) {
				wx0[i][j] = 0.0 + I * 0.0;
				wy0[i][j] = 0.0 + I * 0.0;
				wx1[i][j] = 0.0 + I * 0.0;
				wy1[i][j] = 0.0 + I * 0.0;
			}
			double size = SIZE;
			wx0[i][j] *= size + I * 0.0;
			wy0[i][j] *= size + I * 0.0;
			wx1[i][j] *= size + I * 0.0;
			wy1[i][j] *= size + I * 0.0;
		}
		printf("p");
	}
    fft2D(&wx0[0][0], XN, YN, -1);
    fft2D(&wy0[0][0], XN, YN, -1);
    fft2D(&wx1[0][0], XN, YN, -1);
    fft2D(&wy1[0][0], XN, YN, -1);
	printf("\n");

	fp = fopen(fname, "w");
	for (int i=0; i < XN; i++ ) {
	    double X = (i - XN / 2.0) * DX;
		for (int j=0; j < YN; j++) {
	        double Y = (j - XN / 2.0) * DY;
			v0[i][j] = sqrt(norm(wx0[i][j]) + norm(wy0[i][j]));
			v1[i][j] = sqrt(norm(wx1[i][j]) + norm(wy1[i][j]));
			fprintf(fp,"%f %f %.10lf %.10lf\n", X, Y, v0[i][j], v1[i][j] );
		}
		fprintf(fp, "\n");
		printf("o");
	}
	fclose(fp);
	printf( "\nVelocity field process Done.\n" );
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

	sprintf( fname, argv[2] );
	printf( "filename : %s\n", fname );
	fin.open(fname);
	if ( !fin ) {
		printf("can not open file : %s\n", argv[1]);
		exit(1);
	}
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
	if ( ! argc == 2 ) {
		printf("usage :vf -l[,m,h] input-filename\n");
		exit(1);
	}
	velocity_field( argv[1], argv[2]);

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
