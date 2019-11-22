/*
	program:	ftest.cpp
	contents:	test program for FFT/IFFT of 1D, 2D, 3D
	make:		g++ ftest.cpp fft.cpp -o ftest.exe
	usage:		ftest
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

#define SIZE (256)

using namespace std;

typedef complex<double> IM;
const int NUM = 4;
const IM I(0, 1);
IM Psi[SIZE];

#include "fft.h"

void Init()
{
	for (int i=0; i<SIZE; i++ ) {
		//Psi[i] = (i % 64) * sin(i*5.0); // monotonically increasing, ramp wave
		Psi[i] = exp( (-(SIZE/2-i)*(SIZE/2-i))*1.0e-3 ) * sin(i*5.0); //sin wave within wave packet
	}
}

void outwfa(void){
	char str[256];

	sprintf(str, "ftest.a");
	ofstream out(str);
	for (int i=0; i<SIZE; i++ ) {
		out << i << ' ' << real(Psi[i]) << ' ' << imag(Psi[i]) << endl;
	}
	out.close();
	printf("Original data \n");
}

void outwfb(void){
	char str[256];

	sprintf(str, "ftest.b");
	ofstream out(str);
	for (int i=0; i<SIZE; i++ ) {
		out << i << ' ' << -1.0 * real(Psi[i]) << ' ' << -1.0 * imag(Psi[i]) << endl;
	}
	out.close();
	printf("FFT data \n");
}

void outwfc(void){
	char str[256];

	sprintf(str, "ftest.c");
	ofstream out(str);
	for (int i=0; i<SIZE; i++ ) {
		// after IFFT, it has to perform 1/SIZE to the data, because of full size integration.
		double a = real(Psi[i])/SIZE;
		double ai = imag(Psi[i])/SIZE;
		out << i << ' ' << a << ' ' << ai << endl;
	}
	out.close();
	printf("IFFT data \n");
}

int main(void)
{
	Init();
	outwfa();
    fft(&Psi[0], SIZE, 1); 
	outwfb(); // plot imaginary data, set xrange[0:128], plot 'ftest.b' using 1:3 with lines
    fft(&Psi[0], SIZE, -1); 
	outwfc();

	return 0;
}
