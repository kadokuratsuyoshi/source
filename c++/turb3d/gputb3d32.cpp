/*
	program:	tb3d.cpp
	contents:	quantum turbulence in 16^3 resolution
	make:		g++ -fopenmp tb3d.cpp fft.cpp -o tb3d.exe
	usage:		test.exe outwf &
				test.exe outwf out001 001 &
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
#include <omp.h>
#include <random>

#define MAP (32)
#define MESH (MAP * 2)
#define XN (32)
#define YN (32)
#define ZN (32)
#define SIZE (XN*YN*ZN)
#define XI (4)

using namespace std;
typedef complex<double> IM;
const int NUM = 4;
const IM I(0, 1);
const double DX = 1.0;
const double DY = 1.0;
const double DZ = 1.0;
const double DT = 0.0002;
const double dkx = (2.0 * M_PI) / XN;
const double dky = (2.0 * M_PI) / YN;
const double dkz = (2.0 * M_PI) / ZN;
const double dwt = DT * 1000.0;
const double IR = 0.002;
const double TR = 10000;
const double f = 1.0 / (XN * YN * ZN);
const double kk = sqrt(XN * YN * ZN);
const double A0 = 5.0e0;
const double kappa = 3.0e0;
const double delta = ((XN*2.0)/(2.0*M_PI));
const IM GAMMA = 0.005 + I * 0.0;
const double GG = 1.0;
const double VE = 0.0;
const int TN = TR / DT;

#include "fft.h"

IM ***Psi;
IM ***Ck;
IM comp;
double ***E;

FILE *fp;
char fname[256];
time_t t1, t2;
int t3, hour, minute, sec;
char str[256];

int readf = 0;

random_device rnd;

IM*** Malloc3D(int n1, int n2, int n3, IM* p){
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

double ***dMalloc3D(int n1, int n2, int n3, double *p){
	for (int i = 0; i < n1 * n2 * n3; i++) {
		p[i] = 0;
	}
	double ***p0 = new double **[n1];
	for (int i = 0; i < n1; i++ ) {
		p0[i] = new double *[n2];
		for ( int j = 0; j < n2; j++ ) {
			p0[i][j] = p + j*n3 + i*n2*n3;
		}
	}
	return p0;
}

double crand() {
	double y = (rnd() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
	return y;
}

void Init(){
	IM *p;
	p = new IM[XN*YN*ZN];
	Psi = Malloc3D(XN, YN, ZN, p);
	p = new IM[XN*YN*XN];
	Ck = Malloc3D(XN, YN, ZN, p);
	double *dp = new double[XN*YN*ZN];
	E = dMalloc3D(XN, YN, ZN, dp);

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int k = 0; k < ZN; k++ ) {
		for (int i = 0; i < XN; i++) {
			for (int j = 0; j < YN; j++) {
				// initial state within random phase
				double Real = crand();
				double Imag = crand();
				Psi[i][j][k] = Real + I * Imag;
				// initial homogeneous state
				//Psi[i][j][k] = 1.0 + I * 0.0;
				//Ck[i][j][k] = 0.0 + I * 0.0;
			}
		}
	}
}

void KineticEvolution(IM comp, double t) {
	IM c0;
/*
	if ( comp != -1.0 ) {
		comp = comp - GAMMA;
		c0 = DT / comp;
	}
	else {
		c0 = DT / comp;
	}
*/
		comp = comp - GAMMA;
		c0 = DT / comp;
    fft3D(&Psi[0][0][0], XN, YN, ZN, 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < XN; i++) {
		double kvecx = i < XN / 2 ? i * dkx : (i - XN) * dkx;
		for (int j = 0; j < YN; j++) {
			double kvecy = j < YN / 2 ? j * dky : (j - YN) * dky;
			for (int k = 0; k < ZN; k++ ) {
				double kvecz = k < ZN / 2 ? k * dkz : (k - ZN) * dkz;
				double kkr = kvecx * kvecx + kvecy * kvecy + kvecz * kvecz;
				double kr = sqrt(kr);
				//IM cd0 = c0 * 0.5 * ((kvecx * kvecx) + (kvecy * kvecy) + (kvecz * kvecz));
				/*
				if (kr > ((2.0*M_PI)/8) ) {
					comp = comp - GAMMA;
					c0 = DT / comp;
				}
				else {
					c0 = DT / comp;
				}
				*/
				IM cd0 = c0 * 0.5 * kkr;
				if (comp != - 1.0) {
					cd0 += c0 * VE * kvecx;
				}
				Psi[i][j][k] *= f * exp(cd0);
			}
		}
	}
    fft3D(&Psi[0][0][0], XN, YN, ZN, -1);
}

void LocalEvolution(IM comp, double t){
	IM c0;
/*
	if ( comp != -1.0 ) {
		comp = comp - GAMMA;
		c0 = DT / comp;
	}
	else {
		c0 = DT / comp;
	}
*/
		comp = comp - GAMMA;
		c0 = DT / comp;
   	fft3D(&Ck[0][0][0], XN, YN, ZN, 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < XN; i++) {
		double kvecx = i < XN / 2 ? i * dkx : (i - XN) * dkx;
		for (int j = 0; j < YN; j++) {
			double kvecy = j < YN / 2 ? j * dky : (j - YN) * dky;
			for (int k = 0; k < ZN; k++ ) {
				double kvecz = k < ZN / 2 ? k * dkz : (k - ZN) * dkz;

	        	double Real = crand();
	        	double Imag = crand();
				double coff = sqrt(
					A0 * exp(
						-(1.0/4.0) * delta * delta * ((kvecx * kvecx) + (kvecy * kvecy) + (kvecz * kvecz))
					)
				);
				IM cd0 = Real * coff + I * Imag * coff;
				Ck[i][j][k] *= f;
				Ck[i][j][k] += cd0 * DT - kappa * DT * Ck[i][j][k];
			}
		}
	}
   	fft3D(&Ck[0][0][0], XN, YN, ZN, -1);

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < XN; i++) {
		double X = (i - XN / 2.0) * DX;
		for (int j = 0; j < YN; j++) {
			double Y = (j - YN / 2.0) * DY;
			for (int k = 0; k < ZN; k++) {
				double Z = (k - ZN / 2.0) * DZ;
				IM cd0 = c0 * (GG * norm(Psi[i][j][k]));
				if ( comp != -1.0) {
					cd0 += c0 * kk * (1/(sqrt(XN * YN * ZN)) ) * Ck[i][j][k];
				}
				Psi[i][j][k] *= exp(cd0);
			}
		}
	}
}

double CheckNormalization(void){
	double A = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:A)
#endif
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
			for (int k = 0; k < ZN; k++) {
				A += norm(Psi[i][j][k]);
			}
		}
	}
	return A;
}

void Normalization(){
	double A = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:A)
#endif
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
			for (int k = 0; k < ZN; k++ ) {
				A += norm(Psi[i][j][k]);
			}
		}
	}
	A = A * DX * DY * DZ;
	A = sqrt((DX*DY*DZ*XN*YN*ZN) / A);

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < XN; i++) {
		for (int j = 0; j < YN; j++) {
			for (int k = 0; k < ZN; k++) {
				Psi[i][j][k] *= A;
			}
		}
	}
}

double Energyfunc(void){
	double E_kin = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:E_kin)
#endif
	for (int i = 1; i < XN - 1; i++) {
		for (int j = 1; j < YN - 1; j++) {
			for ( int k = 1; k < ZN - 1; k++ ) {
				IM fx = (Psi[i+1][j][k] - Psi[i-1][j][k]) / (2 * DX);
				IM fy = (Psi[i][j+1][k] - Psi[i][j-1][k]) / (2 * DY);
				IM fz = (Psi[i][j][k+1] - Psi[i][j][k-1]) / (2 * DZ);
				E_kin += 0.5 * (norm(fx) + norm(fy) + norm(fz));
			}
		}
	}
	E_kin *= DX * DY * DZ;

	double E_trap = 0.0;
	/*
	for (int i = 0; i < XN; i++) {
		for ( int j = 0; j < YN; j++) {
			for ( int k = 0; k < ZN; k++ ) {
				E_trap += Ck[i][j][k];
			}
		}
	}
	E_int *= DX * DY * DZ;
	*/

	double E_int = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:E_int)
#endif
	for (int i = 0; i < XN; i++) {
		for ( int j = 0; j < YN; j++) {
			for ( int k = 0; k < ZN; k++ ) {
				E_int += GG * norm(Psi[i][j][k]);
			}
		}
	}
	E_int *= DX * DY * DZ;
	return E_kin + E_trap + E_int;
}

void print_elapsed_time(void){
	t2 = time(NULL);
	t3 = int(t2-t1);
	hour = t3 / 3600;
	minute = (t3 - hour*3600)/60;
	sec = t3 - hour*3600 - minute*60;
	sprintf( str, "%dh %dm %ds\n", hour, minute, sec);
	printf("%s", str);
}

int main(int argc, char *argv[]){
	int g = 0;
	ifstream fin;
	double it = 0.0, t = 0.0, nextt = 0.0;
	char pstr[256], str[256];
    FILE *outputfile;

	sprintf(pstr, "%s.log", argv[1]);
    ofstream ofs(pstr, ios::app);
    outputfile = fopen("status.txt", "w");
    if ( outputfile == NULL ) {
        printf("cannot file\n");
        exit(1);
    }
#ifdef _OPENMP
	omp_set_num_threads(NUM);
#endif
	t1 = time(NULL);
	printf( "argc = %d\n", argc );
	if (argc == 2 ) {
		Init();
	}
	else if ( argc == 4 ) {
		comp = I;
		Init();
		Normalization();
		g = atoi(argv[3]);
		g = g + 1;
		sprintf( fname, argv[2] );
		printf( "filecount : %d, filename : %s\n", g, fname );
		fin.open(fname);
		for (int i = 0; i < XN; i++) {
			for (int j = 0; j < YN; j++) {
				for (int k = 0; k < ZN; k++) {
					double x, y, z, re, im, pre, pim;
					fin >> x >> y >> z >> re >> im >> pre >> pim;
					Psi[i][j][k] = re + I * im;
					Ck[i][j][k] = pre + I * pim;
				}
			}
			printf("i");
		}
		fin.close();
		printf("\n");
		readf = 1;
	}
	else {
		printf( "argc error\n" );
		exit(1);
	}
	printf( "Real time\n" );
	comp = I;
	Normalization();
	for (int d = 0; 1; d++) {
		if (d) {
			KineticEvolution(comp, t);
			LocalEvolution(comp, t);
			Normalization();
			t += DT;
		}
		if ( readf != 0 ) {
			readf = 0;
			d = 1;
			nextt += dwt;
		}
		else if (t >= nextt) {
			print_elapsed_time();
			fprintf(outputfile, str);
			sprintf( str, "%f %f %i %f\n",
					t, Energyfunc()*f, g, CheckNormalization() );
			cerr << str;
			ofs << str;
			sprintf(str, "%s/out%d", argv[1], g);
			ofstream out(str);
			for (int i = 0; i < XN; i++) {
				double X = (i - XN / 2.0) * DX;
				for (int j = 0; j < YN; j++) {
					double Y = (j - YN / 2.0) * DY;
					for (int k = 0; k < ZN; k++) {
						double Z = (k - ZN / 2.0) * DZ;
						out << X << ' ' << Y << ' ' << Z << ' '
							<< real(Psi[i][j][k]) << ' ' << imag(Psi[i][j][k]) << ' '
							<< real(Ck[i][j][k]) << ' ' <<imag(Ck[i][j][k]) <<
							endl;
					}
				}
			}
			out.close();
			nextt += dwt;
			g++;
		}
		if(t > TR){
			break;
    	}
	}
	printf( "end of \"" );
	for (int i=0; i<argc; i++) {
		printf("%s\" ", argv[i] );
	}
	printf("\n");
	print_elapsed_time();
	fprintf(outputfile, str);
	fclose(outputfile);
	return 0;
}
// eof
