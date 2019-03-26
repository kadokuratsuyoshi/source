//
// 2D, FFT
//
//
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <complex>
#include <cstring>
#include <ctime>
#include <omp.h>
//#include <mkl.h>
//#include <mkl_lapack.h>

using namespace std;
typedef complex<double> IM;

const int NUM = 16; // Num_threads
const int AR = 32; // - AR / 2 < X < AR / 2
const int BR = 8;  // - BR / 2 < Y < BR / 2
const int MESH = 64; //64 //128
const int XN = MESH * AR;
const int YN = MESH * BR;
const double DX = AR * 1.0 / XN;
const double DY = BR * 1.0 / YN;
const double DT = 1e-4; //2e-5
const double ONEMS = 1e-2;
const double IR = 10 * ONEMS;
const double TR = 150 * ONEMS;
const double dwt = 1 * ONEMS;
const double NP = 1e5; // plug HIGH
#if 1 // karman
const double VE = 2.6; // velocity
const double NE = 800.0; // 400.0; // plug WIDTH 
#endif
#if 0 // vortex pairs
const double VE = 2.4;
const double NE = 625.0;
#endif
const double PO = 6.0; // Initial position
const double GG = 1000.0;
const int TN = TR / DT;
const IM I(0, 1);
IM **Psi;
//DFTI_DESCRIPTOR *dfti_handle;

//extern void fft(complex <double> x[], int n, int flag);
//extern void fft2D(complex <double> x[], int n1, int n2, int flag);
//extern void fft3D(complex <double> x[], int n1, int n2, int n3, int flag);
#include "fft.h"

IM** Malloc3D(int n1, int n2, IM* p)
{
    for(int d = 0; d < n1 * n2; d++)
        p[d] = 0;
    IM** p0 = new IM*[n1];
    for(int d = 0; d < n1; d++)
        p0[d] = p + d * n2;
    return p0;
}

void InitialState()
{
#ifdef _OPENMP
    omp_set_num_threads(NUM);
#endif
    IM* p = new IM[XN*YN];
    Psi = Malloc3D(XN, YN, p);
    for (int i = 0; i < XN; i++) { 
        for (int j = 0; j < YN; j++)
	        Psi[i][j] = 1.0;
    }
  //long length[2] = {XN, YN};
  //DftiCreateDescriptor(&dfti_handle, DFTI_DOUBLE,
  //		       DFTI_COMPLEX, 2, length);
  //DftiCommitDescriptor(dfti_handle);
}

void KineticEvolution(IM comp)
{
    const double dkx = 2 * M_PI / (XN * DX);
    const double dky = 2 * M_PI / (YN * DY);
    const double f = 1.0 / (XN * YN);
    const IM c0 = DT / comp;

    fft2D(&Psi[0][0], XN, YN, 1);
  //DftiComputeForward(dfti_handle, &Psi[0][0]);
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        double kvecx = i < XN / 2.0 ? i * dkx : (i - XN) * dkx;
        for (int j = 0; j < YN; j++) {
	        double kvecy = j < YN / 2.0 ? j * dky : (j - YN) * dky;
	        IM cd0 = c0 * 0.5 * ((kvecx) * (kvecx) + (kvecy) * (kvecy));
	        if(comp != - 1.0)
	            cd0 += c0 * 2.0 * M_PI * VE * kvecx;
	        Psi[i][j] *= f * (1.0 + 0.5 * cd0) / (1.0 - 0.5 * cd0);
	    }
    }
  //DftiComputeBackward(dfti_handle, &Psi[0][0]);
    fft2D(&Psi[0][0], XN, YN, -1);
}

void LocalEvolution(IM comp)
{
    const IM c0 = DT / comp;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        double X = (i - XN / 2.0) * DX;
        for (int j = 0; j < YN; j++) { 
	        double Y = (j - YN / 2.0) * DY;
	        double plug = NP * exp(- NE * ((X + PO) * (X + PO) + Y * Y));

	        IM cd0 = c0 * (GG * norm(Psi[i][j]) + plug - GG);
	        Psi[i][j] *= (1.0 + 0.5 * cd0) / (1.0 - 0.5 * cd0);
	    }
    }
}

double CheckNormalization()
{
    double A = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:A)
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++)
	        A += norm(Psi[i][j]);
    }
    return A * DX * DY;
}

void Normalization()
{
    double A = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:A)
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++)
	        A += norm(Psi[i][j]);
    }
    A = sqrt(AR * BR / (A * DX * DY));

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < XN; i++) {
        for(int j = 0; j < YN; j++)
	        Psi[i][j] *= A;
    }
}

IM crand()
{
    // RAND_MAX = 32,767, Range - 0.5 < Real, Imag < 0.5
    double Real = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
    double Imag = (rand() % RAND_MAX) / ((double)RAND_MAX) - 0.5;
    return Real + I * Imag;
}

void SetPsi()
{
    srand((unsigned int) time(NULL));
#if 1 // White noize 1
    const double f = 0.0001;
    for (int i = 0; i < XN; i++) {
        double X = (i - XN / 2.0) * DX;

        for(int j = 0; j < YN; j++)
	        Psi[i][j] += f * crand();
    }
#endif
#if 0 // Colored noise
    const double f = 0.0001;
    for (int d = 1; d < 10; d++) {
        for (int e = 1; e < 10; e++) {
	        for (int i = 0; i < XN; i++) {
	            for (int j = 0; j < YN; j++)
		            Psi[i][j] += f * crand() *
		            cos(d * M_PI * i * DX + e * M_PI * j * DY
		            + 2 * M_PI * crand());
	        }
	    }
    }
#endif
}

double Energyfunc()
{
    double E_kin = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:E_kin)
#endif
    for (int i = 1; i < XN - 1; i++) {
        for (int j = 1; j < YN - 1; j++) {
	        IM fx = (Psi[i+1][j] - Psi[i-1][j]) / (2 * DX);
	        IM fy = (Psi[i][j+1] - Psi[i][j-1]) / (2 * DY);
	        E_kin += 0.5 * (norm(fx) + norm(fy));
	    }
    }
    E_kin *= DX * DY;

    double E_trap = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:E_trap)
#endif
    for (int i = 0; i < XN; i++) {
        double X = (i - XN / 2.0) * DX;
        for (int j = 0; j < YN; j++) {
	        double Y = (j - YN / 2.0) * DY;
	        double d = norm(Psi[i][j]);
	        double plug = NP * exp(- NE * ((X + PO) * (X + PO) + Y * Y));

	        E_trap += plug * d;
	    }
    }
    E_trap *= DX * DY;

    double E_int = 0.0;

#ifdef _OPENMP
#pragma omp parallel for reduction (+:E_int)
#endif
    for (int i = 0; i < XN; i++) {
        for (int j = 0; j < YN; j++) {
	        double d = norm(Psi[i][j]);
	        E_int += 0.5 * GG * d * d;
	    }
    }
    E_int *= DX * DY;
    return E_kin + E_trap + E_int;
}

int main(int argc, char *argv[])
{
    int g = 0;
    double it = 0.0, t = 0.0, nextt = 0.0;
    double px[TN], py[TN], dragx[TN], dragy[TN];
    char pstr[256], str[256];

    // 1. Imaginary Time
    IM comp = - 1.0;
    InitialState();
    for (int d = 1; 1; d++) {
        KineticEvolution(comp);
        //LocalEvolution(comp, t);
        LocalEvolution(comp);
        Normalization();
        it += DT;
        if(d % 20 == 0)
	        cerr << it / ONEMS << ' ' << norm(Psi[XN/2][YN/2]) << ' ' 
	        << Energyfunc() << endl;
        if (it > IR) break;
    }

    // 2. Real Time
    comp = I;
    SetPsi();
    Normalization();
    sprintf(pstr, "%s/pop.txt", argv[1]);
    ofstream ofs(pstr);
    for (int d = 0; 1; d++) {
        if (d) {
	        KineticEvolution(comp);
	        //LocalEvolution(comp, t);
	        LocalEvolution(comp);
	        t += DT;
	    }

#if 1 // Drag Force
        px[d+1] = py[d+1] = 0.0;
        double tempx = 0.0;
        double tempy = 0.0;
        const double dkx = 2 * M_PI / (XN * DX);
        const double dky = 2 * M_PI / (YN * DY);
        const double ff = 1.0 / (XN * YN);

        fft2D(&Psi[0][0], XN, YN, 1);
        //DftiComputeForward(dfti_handle, &Psi[0][0]);
    
#ifdef _OPENMP
#pragma omp parallel for reduction (+:tempx,tempy)
#endif
        for (int i = 0; i < XN; i++) {
	        double kvecx = i < XN / 2.0 ? i * dkx : (i - XN) * dkx;
	        for (int j = 0; j < YN; j++) {
	            double kvecy = j < YN / 2.0 ? j * dky : (j - YN) * dky;
	            Psi[i][j] *= ff;
	            tempx -= kvecx * norm(Psi[i][j]);
	            tempy -= kvecy * norm(Psi[i][j]);
	        }
	    }
        //DftiComputeBackward(dfti_handle, &Psi[0][0]);
        fft2D(&Psi[0][0], XN, YN, -1);
        px[d+1] = tempx; 
        py[d+1] = tempy;
        if (d == 0) {
	        dragx[0] = 0.0;
	        dragy[0] = 0.0;
	    }
        else {
	        dragx[d] = (px[d+1] - px[d]) / DT;
	        dragy[d] = (py[d+1] - py[d]) / DT;
	    }
        if (d % 20 == 0) {
	        cerr << t / ONEMS << ' ' << dragx[d] << ' ' << dragy[d] << ' '
	            << CheckNormalization() << endl;
	        ofs << t / ONEMS << ' ' << dragx[d] << ' ' << dragy[d] << ' ' 
	            << CheckNormalization() << endl;
	    }
#endif
        if (t >= nextt) {
	        sprintf(str, "%s/out%d", argv[1], g);
	        ofstream out(str);
	        out << "# t = " << t << endl;
	        for (int i = XN / 4; i < 3 * XN / 4; i++) {
	            double X = (i - XN / 2.0) * DX;
	            for (int j = YN / 4; j < 3 * YN / 4; j++) {
		            double Y = (j - YN / 2.0) * DY;
		            out << X << ' ' << Y << ' ' 
		                << norm(Psi[i][j]) << ' '
		                << arg(Psi[i][j]) << endl;
		        }
	            out << endl;
	        }
	        out.close();
	        nextt += dwt;
	        g++;
        }
        if (t > TR)
            break;
    }
    ofs.close();
    return 0;
}
