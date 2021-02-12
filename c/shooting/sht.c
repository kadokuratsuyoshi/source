/*
	program:	sht.c
	contents:	to solve the shrodinger-equation by using the shooting method
	make:		gcc sht.c -o sht.exe -lm
	using:		>./sht.exe > out0, gnuplot plot 'out0' using 1:2 with lines

	-hbar^2/2m (d^2/dx^2) psi(x) + [V(x)-E] psi(x) = 0
	-hbar^2/2m [(psi(x+dx)-2psi(x)+psi(x-dx))/(dx)^2] + [V(x)-E] psi(x) = 0
	psi(x+dx)-2psi(x)+psi(x-dx) = 2m/hbar^2 (dx)^2 [V(x)-E] psi(x)
	psi(x+dx) = [2m/hbar^2 (dx)^2 [V(x)-E] + 2]psi(x) - psi(x-dx)
	psi2 = (2.0*m*(dx/hbar)*(dx/hbar)*(e0*(x/10e-9)*(x/10e-9)-E)+2.0)*psi1 - psi0;
	psi1 = psi2;
	psi0 = psi1;
*/
#include <stdio.h>
#define		hbar	1.05459e-34			// Dirac constant
#define		m		9.109534e-31		// mass of electron
#define		e0		1.602189e-19		// element of electron
#define		POT		10.0e-9				// 10nm

int main(void){
	float	dE=1.0e-3*e0;	// 1meV
	float	dx=0.1e-9; // 0.1e-9;		// 0.1nm = 1angstlome
	float	E;				// the energy E
	float	psi0,psi1,psi2; // psi(x-dx), psi(x), and psi(x+dx)
	float	x;
	int 	n=2;
	float	old_psi2;

	for (E=0.0; E<e0; E+=dE) {
		psi0=0.0;
		psi1=1.0;
		for(x=dx; x<POT; x+=dx){
			psi2=(2.0*m*(dx/hbar)*(dx/hbar)*
				(e0*(x/POT)*(x/POT)-E)+2.0)*psi1 - psi0;
			psi0=psi1;
			psi1=psi2;
		}
		printf("%f %f\n", E/dE, psi2);

		if ((psi2>0.0) && (old_psi2<0.0)) {
			fprintf(stderr, "%d %f %f\n", n, E/dE, E/dE/((float)n-0.5));
			n += 2;
		}
		if ((psi2<0.0) && (old_psi2>0.0)) {
			fprintf(stderr, "%d %f %f\n", n, E/dE, E/dE/((float)n-0.5));
			n += 2;
		}
		old_psi2 = psi2;
	}
	return 0;
}
