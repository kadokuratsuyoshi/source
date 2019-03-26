/*
	filename:	tbtc.cpp
	contents:	Two-Dimensional binary-components quantum turulence
				under the Gross-Pitaevskii equation and the pseudo-spectrum method
	make:		>g++ tbtc.cpp fft.cpp -o tbtc.exe -lm
	usage:		>./tbtc.exe directory-name
	author:		1643002 T.Kadokura@uec
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

using namespace std;
typedef complex<double> IM;
const int NUM = 4; // 16;
const IM I(0, 1);
const int AR = MAP;
const int BR = MAP;
const double DX = 0.5;
const double DY = 0.5;
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
const double A0 = 1.0;
const double kappa = 1.0; // 2.0 * 0.025; // 1.2; // 0.8; //  0.4; // 0.1; // 4.0 * 0.05; // large number to fast time to change
const double delta = (2.0 * M_PI) / XN; // (XN / 4.0); // (2.0 * M_PI) / 0.5; // 0.25; // no vortex 0.1; // / 0.25; // large number to large area // (2.0 * M_PI) / 1.0; // large number to large area
const IM GAMMA = 0.03 + I * 0.0;
const double GG = 1.0;
const double G0 = 1.0;
const double G1 = 1.0;
const double G01 = 1.0;
const double VE = 0.0;
const int TN = TR / DT;

FILE *fp;
char fname[128];
time_t t1, t2;
int t3, hour, minute, sec;
char str[128];

#include "fft.h"

IM **Psi[2];
IM **Ck;
IM comp;

IM** Malloc2D(int n1, int n2, IM* p)
{
    for (int i = 0; i < n1 * n2; i++) {
        p[i] = 0;
    }
    IM** p0 = new IM*[n1];
    for (int i = 0; i < n1; i++ ) {
        p0[i] = new IM[n2];
        //for ( int j = 0; j < n2; j++ ) {
            p0[i] = p + i*n2;
            //p0[i][j] = p + j + i*n2;
         //   p0[i] = p + j + i*n2;
        //}
    }
    return p0;
}

void InitialState()
{

#ifdef _OPENMP
    omp_set_num_threads(NUM);
#endif

    IM* p = new IM[XN*YN];
    Psi[0] = Malloc2D(XN, YN, p);
    p = new IM[XN*YN];
    Psi[1] = Malloc2D(XN, YN, p);
    p = new IM[XN*YN];
    Ck = Malloc2D(XN, YN, p);

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
            // initial state within random phase
          	//double Real = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
	        //double Imag = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
	        //Psi[i][j][k] = Real + I * Imag;
	        //Psi[0][i][j] = Real + I * Imag;
	        //Real = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
	        //Imag = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
	        //Psi[1][i][j] = Real + I * Imag;

	        // initial homogeneous state
            if ( i < (XN/2) ) {
	            Psi[0][i][j] = 1.0 + I * 0.0;
	            Psi[1][i][j] = 0.0 + I * 0.0;
            }
            else {
	            Psi[0][i][j] = 0.0 + I * 0.0;
	            Psi[1][i][j] = 1.0 + I * 0.0;
            }
	        Ck[i][j] = 0.0 + I * 0.0;
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
    fft2D(&Psi[0][0][0], XN, YN, 1); 
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
	        Psi[0][i][j] *= f * exp(cd0);
        }
    }
    fft2D(&Psi[0][0][0], XN, YN, -1); 

    fft2D(&Psi[1][0][0], XN, YN, 1); 
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        double kvecx = i < XN / 2 ? i * dkx : (i - XN) * dkx;
        for (int j = 0; j < YN; j++) {
            double kvecy = j < YN / 2 ? j * dky : (j - YN) * dky;
	        IM cd1 = c0 * 0.5 * ((kvecx) * (kvecx) + (kvecy) * (kvecy));
	        if (comp != - 1.0) {
	            cd1 += c0 * VE * kvecx;
	        }
	        Psi[1][i][j] *= f * exp(cd1);
        }
    }
    fft2D(&Psi[1][0][0], XN, YN, -1); 
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

    fft2D(&Ck[0][0], XN, YN, 1); 
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        double kvecx = i < XN / 2 ? i * dkx : (i - XN) * dkx;
        for (int j = 0; j < YN; j++) {
            double kvecy = j < YN / 2 ? j * dky : (j - YN) * dky;
	        double Real = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
	        double Imag = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
	        double coff = sqrt(
                A0 * exp(
                    -(1.0/4.0) * delta * delta * (
                            (kvecx) * (kvecx) + (kvecy) * (kvecy)
                        )
                    )
                );
	        IM cd0 = Real * coff + I * Imag * coff;
	        Ck[i][j] *= f;
	        Ck[i][j] += cd0 * DT - kappa * DT * Ck[i][j];
        }
    }
    fft2D(&Ck[0][0], XN, YN, -1); 
  
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
            double d0 = norm(Psi[0][i][j]);
            double d1 = norm(Psi[1][i][j]);
	        IM cd0 = c0 * (G0 * d0 + G01 * d1);
	        IM cd1 = c0 * (G1 * d1 + G01 * d0);
            /*
            if ( comp != -1.0 ) {
                cd0 += c0 * kk * (1/(sqrt(AR * BR * (CR/2)))) * Ck[i][j];
                cd1 += c0 * kk * (1/(sqrt(AR * BR * (CR/2)))) * Ck[i][j];
            }
            */
            cd0 += c0 * kk * (1/(sqrt(AR * BR))) * Ck[i][j];
            cd1 += c0 * kk * (1/(sqrt(AR * BR))) * Ck[i][j];
	        Psi[0][i][j] *= exp(cd0);
	        Psi[1][i][j] *= exp(cd1);
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
	        A0 += norm(Psi[0][i][j]);
        }
    }
    return A0 * DX * DY;
}

double Check_1_Normalization()
{
    double A1 = 0.0;
#ifdef _OPENMP
#pragma omp parallel for reduction (+:A1)
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
	        A1 += norm(Psi[1][i][j]);
        }
    }
    return A1 * DX * DY;
}

void Normalization()
{
    double A0 = 0.0, A1 = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:A0, A1)
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
	        A0 += norm(Psi[0][i][j]);
	        A1 += norm(Psi[1][i][j]);
        }
    }
    A0 = sqrt((AR*BR) / A0);
	A0 = A0 / sqrt(AR*BR) * sqrt(2);
    A1 = sqrt((AR*BR) / A1);
	A1 = A1 / sqrt(AR*BR) * sqrt(2);

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
	        Psi[0][i][j] *= A0;
	        Psi[1][i][j] *= A1;
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
	    	IM x0 = (Psi[0][i+1][j] - Psi[0][i-1][j]) / (2 * DX);
	    	IM y0 = (Psi[0][i][j+1] - Psi[0][i][j-1]) / (2 * DY);
	    	IM x1 = (Psi[1][i+1][j] - Psi[1][i-1][j]) / (2 * DX);
	    	IM y1 = (Psi[1][i][j+1] - Psi[1][i][j-1]) / (2 * DY);
	    	E_kin += 0.5 * ( norm(x0) + norm(y0) + norm(x1) + norm(y1) );
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
	    	double d0 = norm(Psi[0][i][j]);
	        double d1 = norm(Psi[1][i][j]);
            double n_ii = 0.5 * (G0 * d0 * d0 + G1 * d1 * d1);
            double n_ij = G01 * d0 * d1;
	        E_int += n_ii + n_ij;
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
	            sprintf( str, "%.1f norm0:%.1f norm1:%.1f energy:%.1f\n",
					it, Check_0_Normalization(), Check_1_Normalization(), Energyfunc() );
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
	                double x, y, re0, im0, re1, im1, pre, pim;
	                fin >> x >> y >> re0 >> im0 >> re1 >> im1 >> pre >> pim;
	                Psi[0][i][j] = re0 + I * im0;
                    Psi[1][i][j] = re1 + I * im1;
	                Ck[i][j] = pre + I * pim;
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
            sprintf( str, "%d system time:%.1f norm0:%.1f norm1:%.1f energy:%.1f\n", 
                g, t, Check_0_Normalization(), Check_1_Normalization(), Energyfunc() );
            cerr << str;
            ofs << str;

            sprintf(str, "%s/out%d", argv[1], g);
            ofstream out(str);
            for (int i = 0; i < XN; i++) {
	            double X = (i - XN / 2.0) * DX;
	            for (int j = 0; j < XN; j++) {
	                double Y = (j - XN / 2.0) * DY;
	                out << X << ' ' << Y << ' '
                    << real(Psi[0][i][j]) << ' ' << imag(Psi[0][i][j]) << ' ' 
                    << real(Psi[1][i][j]) << ' ' << imag(Psi[1][i][j]) << ' ' 
                    << real(Ck[i][j]) << ' ' <<imag(Ck[i][j])
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
