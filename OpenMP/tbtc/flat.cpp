/*
    filename:   flat.cpp
    contents:   evaluate the flatness of velocity field by wavenumber
    make:       >g++ -fopenmp flat.cpp fft.o -o flat
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
IM **Psi1;
IM **Ck;
IM comp;
IM **dpsix;
IM **dpsiy;
IM **wx0;
IM **wy0;
IM **wx1;
IM **wy1;
double **vf;
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
	vf = dMalloc2D(XN, YN, dp);
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
			vf[i][j] = 0.0;
			E[i][j] = 0.0;
		}
	}
	printf( "debug\n" );
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

void kolmogorov( char *dstr )
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

				double A0x = real(wx0[i][j]);
				double A0y = real(wy0[i][j]);
				double A1x = real(wx1[i][j]);
				double A1y = real(wy1[i][j]);
				wx0[i][j] = wx0[i][j] - (kx * A0x + ky * A0y) / (kx * kx + ky * ky) * kx;
		    	wy0[i][j] = wy0[i][j] - (kx * A0x + ky * A0y) / (kx * kx + ky * ky) * ky;
				wx1[i][j] = wx1[i][j] - (kx * A1x + ky * A1y) / (kx * kx + ky * ky) * kx;
		    	wy1[i][j] = wy1[i][j] - (kx * A1x + ky * A1y) / (kx * kx + ky * ky) * ky;
				/*
				E[i][j] = 0.5 * (
						  norm(wx0[i][j]) + norm(wy0[i][j])
						+ norm(wx1[i][j]) + norm(wy1[i][j])
						);
                */

				vf[i][j] = sqrt (
					norm(wx0[i][j]) + norm(wy0[i][j])
					+ norm(wx1[i][j]) + norm(wy1[i][j])
					);
				if ( vf[i][j] < vf_min ) {
					vf_min = vf[i][j];
				}
				if ( vf[i][j] > vf_max ) {
					vf_max = vf[i][j];
				}
//				vf_ratio = (vf_max - vf_min)/vf_max;
                //vf_ave += vf[i][j];

				double B = kx * kx + ky * ky;
				double kk = sqrt(B);
				int a = int(kk / dk);
				//Ek[a] += E[i][j];

				//flat[a] += vf_ratio/a;
				//flat[a] += vf_ratio;
				flat[a] += (vf_max - vf_min);
                //flat[a] = vf_ave;
                if ( flat_max < flat[a]) {
                    flat_max = flat[a];
                }
				vf_min = 0.0;
				vf_max = 0.0;
		}
	}

	sprintf(fname, "%s.flat", dstr);
	fp = fopen(fname, "w");
	for (int l=0; l < XN; l++) {
		double k = dk * (l + 0.5);
		fprintf(fp,"%.10lf %.10lf\n", k, flat[l]/flat_max);
		flat[l] = 0;
	}
	fclose(fp);
	printf( "Flatness velocity field process Done.\n" );
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
