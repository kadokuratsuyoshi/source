/*
	program:	flux.cpp
	contents:	calculate energy flux from 3D data 
	make:		icc -openmp flux.cpp -o flux $MKLP
	usage:		flux input-data &
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

//#define MKL
#ifdef MKL
#include <mkl.h>
#include <mkl_lapack.h>
#include "omp.h"
#endif

#define LXY (1)
#define MAXNX (32*LXY)
#define MAXNY (32*LXY)
#define MAXNZ (32)
#define SIZE  (MAXNX*MAXNY*MAXNZ)
#define DX (1.0)
#define DY (1.0)
#define DZ (1.0)

using namespace std;
typedef complex<double> IM;
const int NUM = 16;
const IM I(0, 1);
IM ***Psi;
IM comp;
IM ***dpsix;
IM ***dpsiy;
IM ***dpsiz;
IM ***wx;
IM ***wy;
IM ***wz;
IM ***Ck;
IM ***Pf;
IM ***Px;
IM ***Py;
IM ***Pz;
IM ***cPx;
IM ***cPy;
IM ***cPz;
IM ***L;
IM ***Lx;
IM ***Ly;
IM ***Lz;
IM ***cLx;
IM ***cLy;
IM ***cLz;
double ***E;
double *Ek;
double *Tk;

int argf;
int ncancel;

FILE *fout_laptime;
FILE *fp;
time_t t1, t2;
int t3, hour, minute, sec;
char laptime_str[256];
void print_elapsed_time(void)
{
	t2 = time(NULL);
	t3 = int(t2-t1);
	hour = t3 / 3600;
	minute = (t3 - hour*3600)/60;
	sec = t3 - hour*3600 - minute*60;
	sprintf( laptime_str, "%dh %dm %ds\n", hour, minute, sec);
	fprintf(fout_laptime, laptime_str);
	printf("%s", laptime_str);
}

#ifdef MKL
DFTI_DESCRIPTOR *dfti_handle;
#else
extern void fft(complex <double> x[], int n, int flag);
extern void fft2D(complex  <double> x[], int n1, int n2, int flag);
extern void fft3D(complex  <double> x[], int n1, int n2, int n3, int flag);
#endif

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

double ***dMalloc3D(int n1, int n2, int n3, double *p)
{
	for (int i = 0; i < n1 * n2 * n3; i++) {
		p[i] = 0;
	}
	double ***p0 = new double**[n1];
	for (int i = 0; i < n1; i++ ) {
		p0[i] = new double*[n2];
		for ( int j = 0; j < n2; j++ ) {
			p0[i][j] = p + j*n3 + i*n2*n3;
		}
	}
	return p0;
}

void mem_copy(IM ***s, IM ***d)
{
	for (int i = 0; i < MAXNX; i++) {
		for (int j = 0; j < MAXNY; j++) {
			for (int k = 0; k < MAXNZ; k++) {
				d[i][j][k] = s[i][j][k];
			}
		}
	}
}

void Init()
{
	IM* p;
	p = new IM[SIZE];
	Psi = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	dpsix = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	dpsiy = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	dpsiz = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	wx = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	wy = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	wz = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Pf = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Px = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Py = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Pz = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	cPx = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	cPy = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	cPz = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	L =  Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Lx=  Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Ly=  Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Lz=  Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	cLx=  Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	cLy=  Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	cLz=  Malloc3D(MAXNX, MAXNY, MAXNZ, p);
	p = new IM[SIZE];
	Ck = Malloc3D(MAXNX, MAXNY, MAXNZ, p);
    double *dp;
	dp= new double[SIZE];
	E = dMalloc3D(MAXNX, MAXNY, MAXNZ, dp);
    Ek = new double[MAXNX];
	Tk = new double[MAXNX];

	for (int i = 0; i < MAXNX; i++) {
		for (int j = 0; j < MAXNY; j++) {
			for (int k = 0; k < MAXNZ; k++) {
				Psi[i][j][k] = 1.0 + I * 0.0;
				dpsix[i][j][k] = 0.0 + I * 0.0;
				dpsiy[i][j][k] = 0.0 + I * 0.0;
				dpsiz[i][j][k] = 0.0 + I * 0.0;
				wx[i][j][k] = 0.0 + I * 0.0;
				wy[i][j][k] = 0.0 + I * 0.0;
				wz[i][j][k] = 0.0 + I * 0.0;
				Ck[i][j][k] = 0.0 + I * 0.0;
			}
		}
	}
	for (int i = 0; i < MAXNX; i++) {
		Ek[i] = 0.0;
		Tk[i] = 0.0;
	}
}

void eflux( char *dstr )
{
	double x, y, z, kx, ky, kz, dkx, dky, dkz;
	double dk;
	char fout_name[256];

#ifdef MKL
	DFTI_DESCRIPTOR *dfti_handle;
	long length[3] = {MAXNX, MAXNY, MAXNZ};
	DftiCreateDescriptor(&dfti_handle, DFTI_DOUBLE,DFTI_COMPLEX, 3, length);
	DftiCommitDescriptor(dfti_handle);
#endif
	dkx = 2 * M_PI/(MAXNX*DX);
	dky = 2 * M_PI/(MAXNY*DY);
	dkz = 2 * M_PI/(MAXNZ*DZ);
	dk = dkx;
	for (int l=0; l < MAXNX; l++) {
		Ek[l] = 0;
	}
	for(int i = 0; i < MAXNX; i++){
		for(int j = 0; j < MAXNY; j++){
			for(int k = 0; k < MAXNZ; k++){
				dpsix[i][j][k] = (Psi[(i+1) % MAXNX][j][k] - Psi[i][j][k])/DX; 
				dpsiy[i][j][k] = (Psi[i][(j+1) % MAXNY][k] - Psi[i][j][k])/DY;
				dpsiz[i][j][k] = (Psi[i][j][(k+1) % MAXNZ] - Psi[i][j][k])/DZ;
				/*
				wx[i][j][k] = imag(conj(Psi[i][j][k])*dpsix[i][j][k])/sqrt(norm(Psi[i][j][k]));
				wy[i][j][k] = imag(conj(Psi[i][j][k])*dpsiy[i][j][k])/sqrt(norm(Psi[i][j][k]));
				wz[i][j][k] = imag(conj(Psi[i][j][k])*dpsiz[i][j][k])/sqrt(norm(Psi[i][j][k]));
				*/
				
				wx[i][j][k] = imag(conj(Psi[i][j][k])*dpsix[i][j][k]);
				wy[i][j][k] = imag(conj(Psi[i][j][k])*dpsiy[i][j][k]);
				wz[i][j][k] = imag(conj(Psi[i][j][k])*dpsiz[i][j][k]);
				
			}
		}
	}
#if 0
#ifdef MKL
	DftiComputeForward(dfti_handle, &wx[0][0][0]);
	DftiComputeForward(dfti_handle, &wy[0][0][0]);    
	DftiComputeForward(dfti_handle, &wz[0][0][0]);    
#else
    fft3D(&wx[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
    fft3D(&wy[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
    fft3D(&wz[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
#endif
	for (int i=0;i < MAXNX; i++) {
		double kx, ky, kz;
		if (i < MAXNX/2) {
			kx = dkx * i;
		}
		else {
			kx = dkx * (i - MAXNX);
		} 
		if (kx == 0.0) {
			kx = 0.1;
		}
		for (int j=0;j < MAXNY; j++) {
			if (j < MAXNY/2) {
				ky = dky * j;
			}
			else {
				ky = dky * (j - MAXNY);
			}
			if (ky == 0.0) {
				ky = 0.1;
			}
			for (int k = 0;k < MAXNZ; k++) {
				if (k < MAXNZ/2) {
					kz = dkz * k;
				}
				else {
					kz = dkz * (k - MAXNZ);
				}
				if ( kz == 0.0 ) {
					kz = 0.1;
				}
				double Ax = real(wx[i][j][k]);
				double Ay = real(wy[i][j][k]);
				double Az = real(wz[i][j][k]);
				wx[i][j][k]=wx[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * kx; 
				wy[i][j][k]=wy[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * ky;
				wz[i][j][k]=wz[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * kz;
				wx[i][j][k]/=SIZE;
				wy[i][j][k]/=SIZE;
				wz[i][j][k]/=SIZE;
			}
		}
	}
#ifdef MKL		
	DftiComputeBackward(dfti_handle, &wx[0][0][0]);
	DftiComputeBackward(dfti_handle, &wy[0][0][0]);    
	DftiComputeBackward(dfti_handle, &wz[0][0][0]);
#else
    fft3D(&wx[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
    fft3D(&wy[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
    fft3D(&wz[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
#endif
#endif
	// P, (P dot Nabla) V
	for(int i = 0; i < MAXNX; i++){
		for(int j = 0; j < MAXNY; j++){
			for(int k = 0; k < MAXNZ; k++){
				Pf[i][j][k] = sqrt(norm(Psi[i][j][k]));
				/*
				Px[i][j][k] = Pf[i][j][k] * wx[i][j][k];
				Py[i][j][k] = Pf[i][j][k] * wy[i][j][k];
				Pz[i][j][k] = Pf[i][j][k] * wz[i][j][k];
				*/
				Px[i][j][k] = wx[i][j][k];
				Py[i][j][k] = wy[i][j][k];
				Pz[i][j][k] = wz[i][j][k];
				Lx[i][j][k] =
					
					  Px[i][j][k] * ((wx[(i+1) % MAXNX][j][k] - wx[i][j][k])/DX)
					+ Py[i][j][k] * ((wx[i][(j+1) % MAXNY][k] - wx[i][j][k])/DY)
					;
					//+ Pz[i][j][k] * ((wx[i][j][(k+1) % MAXNZ] - wx[i][j][k])/DZ);
				    	
					//Px[i][j][k] * ((wx[(i+1) % MAXNX][j][k] - wx[i][j][k])/DX);
				Ly[i][j][k] =
					
					  Px[i][j][k] * ((wy[(i+1) % MAXNX][j][k] - wy[i][j][k])/DX)
					+ Py[i][j][k] * ((wy[i][(j+1) % MAXNY][k] - wy[i][j][k])/DY)
					;
					//+ Pz[i][j][k] * ((wy[i][j][(k+1) % MAXNZ] - wy[i][j][k])/DZ);
					
					//Py[i][j][k] * ((wy[i][(j+1) % MAXNY][k] - wy[i][j][k])/DY);
				Lz[i][j][k] =
					
					  Px[i][j][k] * ((wz[(i+1) % MAXNX][j][k] - wz[i][j][k])/DX)
					+ Py[i][j][k] * ((wz[i][(j+1) % MAXNY][k] - wz[i][j][k])/DY)
					;
					//+ Pz[i][j][k] * ((wz[i][j][(k+1) % MAXNZ] - wz[i][j][k])/DZ);
					
					//Pz[i][j][k] * ((wz[i][j][(k+1) % MAXNZ] - wz[i][j][k])/DZ);
			}
		}
	}

	
	for (int s=0; s < MAXNX; s++) {
		// DATA COPY
		mem_copy(Px, cPx);
		mem_copy(Py, cPy);
		mem_copy(Pz, cPz);
		mem_copy(Lx, cLx);
		mem_copy(Ly, cLy);
		mem_copy(Lz, cLz);
		// FFT
#ifdef MKL	
		DftiComputeForward(dfti_handle, &cPx[0][0][0]);
		DftiComputeForward(dfti_handle, &cPy[0][0][0]);    
		DftiComputeForward(dfti_handle, &cPz[0][0][0]);    
		DftiComputeForward(dfti_handle, &cLx[0][0][0]);    
		DftiComputeForward(dfti_handle, &cLy[0][0][0]);    
		DftiComputeForward(dfti_handle, &cLz[0][0][0]);    
#else	
    	fft3D(&cPx[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
    	fft3D(&cPy[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
    	fft3D(&cPz[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
    	fft3D(&cLx[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
    	fft3D(&cLy[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
    	fft3D(&cLz[0][0][0], MAXNX, MAXNY, MAXNZ, 1);
#endif
		// LPF P(k<), L(k<)
		for (int i=0;i < MAXNX; i++) {
			if (i < MAXNX/2) {
				kx = dkx * i;
			}
			else {
				kx = dkx * (i - MAXNX);
			} 
			if (kx == 0.0) {
				kx = 0.1;
			}
			for (int j=0;j < MAXNY; j++) {
				if (j < MAXNY/2) {
					ky = dky * j;
				}
				else {
					ky = dky * (j - MAXNY);
				}
				if (ky == 0.0) {
					ky = 0.1;
				}
				for (int k = 0;k < MAXNZ; k++) {
					if (k < MAXNZ/2) {
						kz = dkz * k;
					}
					else {
						kz = dkz * (k - MAXNZ);
					}
					if ( kz == 0.0 ) {
						kz = 0.1;
					}
					double kr = sqrt(kx*kx + ky*ky + kz*kz);
					//if ( kr > s ) {
					if ( (s < kr) && (kr < (s+2) ) ) {
						;
					}
					else {
						cPx[i][j][k] = 0.0;
						cPy[i][j][k] = 0.0;
						cPz[i][j][k] = 0.0;
						cLx[i][j][k] = 0.0;
						cLy[i][j][k] = 0.0;
						cLz[i][j][k] = 0.0;
					}
				}
			}
		}
		// IMCOMPRESSIBLE P(k<), L(k<)  and INTEGRATE full space P dot L
		for (int i=0;i < MAXNX; i++) {
			if (i < MAXNX/2) {
				kx = dkx * i;
			}
			else {
				kx = dkx * (i - MAXNX);
			} 
			if (kx == 0.0) {
				kx = 0.0001;
			}
			for (int j=0;j < MAXNY; j++) {
				if (j < MAXNY/2) {
					ky = dky * j;
				}
				else {
					ky = dky * (j - MAXNY);
				}
				if (ky == 0.0) {
					ky = 0.0001;
				}
				for (int k = 0;k < MAXNZ; k++) {
					if (k < MAXNZ/2) {
						kz = dkz * k;
					}
					else {
						kz = dkz * (k - MAXNZ);
					}
					if ( kz == 0.0 ) {
						kz = 0.0001;
					}
					double Ax;
					double Ay;
					double Az;
					Ax = real(cPx[i][j][k]);
					Ay = real(cPy[i][j][k]);
					Az = real(cPz[i][j][k]);
					cPx[i][j][k]=cPx[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * kx; 
					cPy[i][j][k]=cPy[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * ky;
					cPz[i][j][k]=cPz[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * kz;
					Ax = real(cLx[i][j][k]);
					Ay = real(cLy[i][j][k]);
					Az = real(cLz[i][j][k]);
					cLx[i][j][k]=cLx[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * kx; 
					cLy[i][j][k]=cLy[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * ky;
					cLz[i][j][k]=cLz[i][j][k]-(kx*Ax + ky*Ay + kz*Az)/(kx*kx + ky*ky + kz*kz) * kz;
					E[i][j][k] = real(
						  cPx[i][j][k] * cLx[i][j][k]
						+ cPy[i][j][k] * cLy[i][j][k]
						+ cPz[i][j][k] * cLz[i][j][k]
					);
					double B = kx * kx + ky * ky + kz * kz;
					double kk = sqrt(B);
					int a = int(kk / dk);
					Ek[a] += E[i][j][k] * dk;
					//Tk[s] += E[i][j][k] * dk;
				}
			}
		}
		Tk[s] = Ek[s];
#if 0
		// IFFT
#ifdef MKL	
		DftiComputeBackward(dfti_handle, &cPx[0][0][0]);
		DftiComputeBackward(dfti_handle, &cPy[0][0][0]);    
		DftiComputeBackward(dfti_handle, &cPz[0][0][0]);    
		DftiComputeBackward(dfti_handle, &cLx[0][0][0]);    
		DftiComputeBackward(dfti_handle, &cLy[0][0][0]);    
		DftiComputeBackward(dfti_handle, &cLz[0][0][0]);    
#else	
    	fft3D(&cPx[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
    	fft3D(&cPy[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
    	fft3D(&cPz[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
    	fft3D(&cLx[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
    	fft3D(&cLy[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
    	fft3D(&cLz[0][0][0], MAXNX, MAXNY, MAXNZ, -1);
#endif
		// P dot L
		/*
		for(int i = 0; i < MAXNX; i++){
			for(int j = 0; j < MAXNY; j++){
				for(int k = 0; k < MAXNZ; k++){
					Tk[s] += real( 
						  cPx[i][j][k] * cLx[i][j][k]
						+ cPy[i][j][k] * cLy[i][j][k]
						+ cPz[i][j][k] * cLz[i][j][k]
						) * dk;
				}
			}
		}
		*/
#endif
       	if ( s % 10 == 0 ) {
       		cerr << "p";
   		}
	}
    cerr << endl;

	sprintf(fout_name, "%s.flux", dstr);
	fp = fopen(fout_name, "w");
	Ek[0] = 0.0/0.0;
	for (int l=0; l < MAXNX; l++) {
		double k;
		if ( l == 0 ) {
			k = dk * 0.1; 
		}
		else {
			k = dk * l;
		}
		/*
		if ( l > 0 ) {
			Tk[l] = Tk[l-1] + Ek[l];
		}
		*/
		fprintf(fp,"%f %f %f\n", k, (-2.0/SIZE) * Ek[l], (-2.0/SIZE) * Tk[l]);
        if ( l % 10 == 0 ) {
            cerr << "o";
        }
	}
	fclose(fp);
	cerr << endl;

	cerr << "OUTPUT file :" << fout_name << endl;
	cerr << "Energy Flux process Done.\n";
}

void usage(void)
{
	cerr << "flux filename" << endl;
}

int main(int argc, char *argv[])
{
#ifdef MKL	
	long length[3] = {MAXNX, MAXNY, MAXNZ};
	DftiCreateDescriptor(&dfti_handle, DFTI_DOUBLE, DFTI_COMPLEX, 3, length);
 	DftiCommitDescriptor(dfti_handle);
	omp_set_num_threads(NUM);
#endif

	ifstream fin;
	double it = 0.0, t = 0.0, nextt = 0.0;
	char fin_name[256];

	fout_laptime = fopen("laptime.log", "a");
	if ( fout_laptime == NULL ) {
		printf("cannot make file\n");
		exit(1);
	}
	t1 = time(NULL);
	printf( "\nargc = %d\n", argc );
	if ( argc != 2) {
		usage();
		return 0;
	}
	Init();
	argf = 1;
	sprintf( fin_name, argv[argf] );
	cout << "INPUT file :" << fin_name << endl;
	fin.open(fin_name);
    if ( !fin ) {
        cerr << "cannot open file :" << fin_name << endl;
        exit(1);
    }
	for (int i = 0; i < MAXNX; i++) {
		for (int j = 0; j < MAXNY; j++) {
			for (int k = 0; k < MAXNZ; k++) {
				double x, y, z, re, im, pre, pim;
				fin >> x >> y >> z >> re >> im >> pre >> pim;
				Psi[i][j][k] = re + I * im;
                Ck[i][j][k] = pre + I * pim;
			}
		}
        if ( i % 10 == 0 ) {
		    cerr << "i";
        }
	}
    cerr << endl;
	fin.close();

	eflux( argv[argf] );
	
	printf( "end of >" );
	for (int i=0; i<argc; i++) {
		printf("%s ", argv[i] );
	}
	printf("\n");
	print_elapsed_time();
	fclose(fout_laptime);
	return 0;
}
// eof
