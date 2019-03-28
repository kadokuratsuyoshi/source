/*
	filename:	qv.cpp
	contents:	quantum vortex
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

#define MAP (32)
#define MESH (MAP * 2)
#define N (MESH)
#define MAXNX (64)
#define MAXNY (64)
#define SIZE (MAXNX*MAXNY)
#define CR (0.9)
#define OMEGA (2.0*M_PI*500.0)
#define TAU (1000.0)

using namespace std;
typedef complex<double> IM;
const int NUM = 4; // 16;
const IM I(0, 1);
const int AR = MAP;
const int BR = MAP;
const double DX = 0.05; // 0.2;
const double DY = 0.05; // 0.2;
const int XN = MAXNX;
const int YN = MAXNY;
const double DT = 0.0002;
const double dkx = (2.0 * M_PI) / MAP;
const double dky = (2.0 * M_PI) / MAP;
const double dwt = DT * 8000.0;
const double IR = 0.02;
const double TR = 100000;
const double f = 1.0 / (XN * YN);
const double kk = sqrt(AR * BR);
const IM GAMMA = 0.03 + I * 0.0;
const double GG = 1.0;
const double VE = 0.0;
const int TN = TR / DT;

FILE *fp;
char fname[128];
time_t t1, t2;
int t3, hour, minute, sec;
char str[128];

#include "fft.h"

IM **Psi;
IM comp;

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

void InitialState()
{

#ifdef _OPENMP
    omp_set_num_threads(NUM);
#endif

    IM* p = new IM[XN*YN];
    Psi = Malloc2D(XN, YN, p);

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
			double trapx =((double)(i-(XN-1)/2.0)*DX);
			double trapy =((double)(j-(YN-1)/2.0)*DY);
	    	Psi[i][j] = exp(-0.5*trapx*trapx) * exp(-0.5*trapy*trapy);
        }
    }
}

void KineticEvolution(IM comp, double t)
{
    IM c0;

    if ( comp != -1.0 ) {
        comp = comp - GAMMA;
        c0 = DT / comp;
    }
    else {
        c0 = DT / comp;
    }
    fft2D(&Psi[0][0], XN, YN, 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        double kvecx = i < XN / 2 ? i * dkx : (i - XN) * dkx;
        for (int j = 0; j < YN; j++) {
            double kvecy = j < YN / 2 ? j * dky : (j - YN) * dky;
	        IM cd0 = c0 * 0.5 * ((kvecx) * (kvecx) + (kvecy) * (kvecy));
	        if (comp != - 1.0) {
	            cd0 += c0 * VE * kvecx;
	        }
	        Psi[i][j] *= f * exp(cd0);
        }
    }
    fft2D(&Psi[0][0], XN, YN, -1);
}

void LocalEvolution(IM comp, double t)
{
    IM c0;

    if ( comp != -1.0 ) {
        comp = comp - GAMMA;
        c0 = DT / comp;
    }
    else {
        c0 = DT / comp;
    }

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
            double d0 = norm(Psi[i][j]);
			double trapx =((double)(i-(XN-1)/2.0)*DX) * (CR *  cos(OMEGA*t) * exp(-t*TAU));
			double trapy =((double)(j-(YN-1)/2.0)*DY) * (CR * -cos(OMEGA*t) * exp(-t*TAU));
			double Rpo = (double) trapx*trapx+trapy*trapy;
	        IM cd0 = c0 * (Rpo + GG * d0);
	        Psi[i][j] *= exp(cd0 * t / (I-GAMMA));
        }
    }
}

double Check_0_Normalization()
{
    double A0 = 0.0;
#ifdef _OPENMP
#pragma omp parallel for reduction (+:A0)
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
	        A0 += norm(Psi[i][j]);
        }
    }
    return A0 * DX * DY;
}

void Normalization()
{
    double A0 = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:A0)
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
	        A0 += norm(Psi[i][j]);
        }
    }
	A0 = A0 * DX * DY;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
	        Psi[i][j] = Psi[i][j] / sqrt(A0);
        }
    }
}


double Energyfunc()
{
	double E_kin = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:E_kin)
#endif
    for (int i = 1; i < XN - 1; i++) {
        for (int j = 1; j < YN - 1; j++) {
	    	IM x0 = (Psi[i+1][j] - Psi[i-1][j]) / (2 * DX);
	    	IM y0 = (Psi[i][j+1] - Psi[i][j-1]) / (2 * DY);
	    	E_kin += 0.5 * ( norm(x0) + norm(y0) );
        }
    }
    E_kin *= DX * DY;

    double E_trap = 0.0;
    double E_int = 0.0;
#ifdef _OPENMP
#pragma omp parallel for reduction (+:E_int)
#endif
    for (int i = 0; i < XN; i++) {
        for ( int j = 0; j < YN; j++) {
	    	double d0 = norm(Psi[i][j]);
            double n_ii = 0.5 * (GG * d0 * d0);
	        E_int += n_ii;
        }
    }
    E_int *= DX * DY;
    return E_kin + E_trap + E_int;
}

void print_elapsed_time(void)
{
    t2 = time(NULL);
    t3 = int(t2-t1);
    hour = t3 / 3600;
    minute = (t3 - hour*3600)/60;
    sec = t3 - hour*3600 - minute*60;
    sprintf( str, "%dh %dm %ds\n", hour, minute, sec);
    printf("%s", str);
}

int main(int argc, char *argv[])
{
    int g = 0;
    ifstream fin;
    double it = 0.0, t = 0.0, nextt = 0.0;
    char pstr[256], str[256];
    FILE *outputfile;

    outputfile = fopen("status.txt", "w");
    if ( outputfile == NULL ) {
        printf("cannot file\n");
        exit(1);
    }
    t1 = time(NULL);

    printf( "argc = %d\n", argc );
    if (argc == 2 ) {
        // 1. Imaginary Time
        printf( "\nImaginary time\n" );
        comp = - 1.0;
        InitialState();
        Normalization();
        for (int d = 1; 1; d++) {
            KineticEvolution(comp, t);
            LocalEvolution(comp, t);
            Normalization();
            if(d % 10 == 0) {
	            sprintf( str, "%.1f norm0:%.1f energy:%.1f\n",
					it, Check_0_Normalization(), Energyfunc() );
	            fprintf(outputfile, str);
	            cerr << str;
            }
            if(it > IR) break;
            it += DT;
        }
    }
    else if ( argc == 4 ) {
        // usage ./test.exe outwf outwf/out2500 2500 &
        comp = I;
        InitialState();
        Normalization();
        g = atoi(argv[3]);
        g = g + 1;
        sprintf( fname, argv[2] );
        printf( "filecount : %d, filename : %s\n", g, fname );
        fin.open(fname);
        for (int i = 0; i < XN; i++) {
            for (int j = 0; j < YN; j++) {
	                double x, y, re0, im0;
	                fin >> x >> y >> re0 >> im0;
	                Psi[i][j] = re0 + I * im0;
            }
            printf(".");
        }
        fin.close();
        printf("\n");
    }
    else {
        printf( "argc error\n" );
        exit(1);
    }

    // 2. Real Time
    printf( "Real time\n" );
    comp = I;
    // SetPsi();
    Normalization();
    sprintf(pstr, "%s/pop.txt", argv[1]);
    ofstream ofs(pstr);
    for (int d = 0; 1; d++) {
        if (d) {
            KineticEvolution(comp, t);
            LocalEvolution(comp, t);
            Normalization();
            t += DT;
        }
        if (t >= nextt) {
            print_elapsed_time();
            fprintf(outputfile, str);
            sprintf( str, "%d system time:%.1f norm0:%.1f energy:%.1f\n",
                g, t, Check_0_Normalization(), Energyfunc() );
            cerr << str;
            ofs << str;

            sprintf(str, "%s/out%d", argv[1], g);
            ofstream out(str);
            for (int i = 0; i < XN; i++) {
	            double X = (i - XN / 2.0) * DX;
	            for (int j = 0; j < XN; j++) {
	                double Y = (j - XN / 2.0) * DY;
	                out << X << ' ' << Y << ' '
                    << real(Psi[i][j]) << ' ' << imag(Psi[i][j]) << ' '
                    << endl;
	            }
                out << endl;
            }
            out.close();
            nextt += dwt;
            g++;
        }
        if(g > 300) {
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
    ofs.close();
    return 0;
}
// eof
