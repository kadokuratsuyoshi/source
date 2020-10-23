/*
	program:	lorentz.c
	contents:	solve the Lorentz equation
	make:		gcc lorentz.c -o lorentz.exe -lm
	usage:		lorentz
*/
#include <stdio.h>
#include <stdlib.h>

int    sigma = 10;
int    r   = 28;
double b   = 8.0/3.0;

double f(double x, double y) {
	return ( sigma*(-x + y) );
}
double g(double x, double y, double z) {
	return ( r*x - y - x*z );
}
double h(double x, double y, double z) {
	return ( -b*z + x*y );
}

int main(void) {
	int i, n;
	double x0, y0, z0, x, y, z;
	double dt, df[4], dg[4], dh[4];
	FILE *fp, *pp;

	x0 = 1.0;
	y0 = 1.0;
	z0 = 1.0;
	dt = 0.001;
	n  = 200000;
	fp = fopen("lorenz.dat", "w");
	pp = popen("gnuplot -persist", "w");
	for(i = 0 ; i < n ; i++){
		fprintf(fp, "%lf\t%lf\t%lf\n", x, y, z);
		df[0] = dt * f(x0,           y0);
		dg[0] = dt * g(x0,           y0,           z0);
		dh[0] = dt * h(x0,           y0,           z0);
		df[1] = dt * f(x0+df[0]/2.0, y0+dg[0]/2.0);
		dg[1] = dt * g(x0+df[0]/2.0, y0+dg[0]/2.0, z0+dh[0]/2.0);
		dh[1] = dt * h(x0+df[0]/2.0, y0+dg[0]/2.0, z0+dh[0]/2.0);
		df[2] = dt * f(x0+df[1]/2.0, y0+dg[1]/2.0);
		dg[2] = dt * g(x0+df[1]/2.0, y0+dg[1]/2.0, z0+dh[1]/2.0);
		dh[2] = dt * h(x0+df[1]/2.0, y0+dg[1]/2.0, z0+dh[1]/2.0);
		df[3] = dt * f(x0+df[2],     y0+dg[2]);
		dg[3] = dt * g(x0+df[2],     y0+dg[2],     z0+dh[2]);
		dh[3] = dt * h(x0+df[2],     y0+dg[2],     z0+dh[2]);
		xn = x0 + (df[0] + 2.0*(df[1]+df[2]) + df[3])/6.0;
		yn = y0 + (dg[0] + 2.0*(dg[1]+dg[2]) + dg[3])/6.0;
		zn = z0 + (dh[0] + 2.0*(dh[1]+dh[2]) + dh[3])/6.0;
		x0 = x;
		y0 = y;
		z0 = z;
	}
	fclose(fp);
	fprintf(pp, "splot \"lorenz.dat\" w l\n");
	pclose(pp);
	return 0;
}
// eof
