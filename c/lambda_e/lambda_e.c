#include <stdio.h>
#include <math.h>

const double c		= 2.99792458e8;	// m/s
const double mu0	= 1.2566e-6;	// NA^-2
const double e0		= 8.8542e-12;	// Fm^-1
const double G		= 6.6743e-11;	// Nm^2kg
const double h		= 6.6261e-34;	// Js
const double hbar	= 1.0546e-34;	// Js
const double e		= 1.6022e-19;	// C
const double alpha	= 7.2974e-3;	// 1/alpha=137.04
const double Rinf	= 1.0974e7;		// m^-1
const double a0		= 5.2918e-11;	// m
const double me		= 9.1094e-31;	// kg
const double mp		= 1.6726e-27;	// kg
const double mn		= 1.6749e-27;	// kg
const double mu		= 1.6605e-17;	// kg 12C/12 = 1u
const double NA		= 6.0221e23;	// mol^-1
const double kB		= 1.3806e-23;	// JK^-1
const double F		= 96485.33212;	// Cmol^-1
const double R		= 8.3145;		// Jmol^-1K^-1
const double Vm		= 22.4140e-3;	// m^3mol^-1
const double SB		= 5.6703e-8;	// Wm^-2K^-4

main(){
	double V = 100.0; // 100V
	double E = e*V;
	double lambda;		// wavelength of electron as the DC 100V

	lambda = h / sqrt(2.0 * me * E);
	printf("%g\n", lambda);
} 
