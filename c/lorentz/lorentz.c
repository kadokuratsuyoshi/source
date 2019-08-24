/*
	program:	lorentz.c
	contents:	solve the Lorentz equation
	make:		gcc lorentz.c -o lorentz.exe -lm
	usage:		lorentz
*/
#include <stdio.h>
#include <stdlib.h>

int    sgm = 10;
int    r   = 28;
double b   = 8.0/3.0;

double f(double x, double y) {
	return ( sgm*(-x + y) );
}
double g(double x, double y, double z) {
	return ( r*x - y - x*z );
}
double h(double x, double y, double z) {
	return ( -b*z + x*y );
}

int main(void) {
	int i, n;
	double x0, y0, z0, xn, yn, zn;
	double dt, kf[4], kg[4], kh[4];
	FILE *fp, *gp;

	x0 = 1.0;
	y0 = 1.0;
	z0 = 1.0;
	dt = 0.001;
	n  = 200000;
	fp = fopen("lorenz.dat", "w");
	gp = popen("gnuplot -persist", "w");
	for(i = 0 ; i < n ; i++){
		fprintf(fp, "%lf\t%lf\t%lf\n", xn, yn, zn);
		kf[0] = dt * f(x0, y0);
		kg[0] = dt * g(x0, y0, z0);
		kh[0] = dt * h(x0, y0, z0);
		kf[1] = dt * f(x0+kf[0]/2.0, y0+kg[0]/2.0);
		kg[1] = dt * g(x0+kf[0]/2.0, y0+kg[0]/2.0, z0+kh[0]/2.0);
		kh[1] = dt * h(x0+kf[0]/2.0, y0+kg[0]/2.0, z0+kh[0]/2.0);
		kf[2] = dt * f(x0+kf[1]/2.0, y0+kg[1]/2.0);
		kg[2] = dt * g(x0+kf[1]/2.0, y0+kg[1]/2.0, z0+kh[1]/2.0);
		kh[2] = dt * h(x0+kf[1]/2.0, y0+kg[1]/2.0, z0+kh[1]/2.0);
		kf[3] = dt * f(x0+kf[2], y0+kg[2]);
		kg[3] = dt * g(x0+kf[2], y0+kg[2], z0+kh[2]);
		kh[3] = dt * h(x0+kf[2], y0+kg[2], z0+kh[2]);
		xn = x0 + (kf[0] + 2.0*(kf[1]+kf[2]) + kf[3])/6.0;
		yn = y0 + (kg[0] + 2.0*(kg[1]+kg[2]) + kg[3])/6.0;
		zn = z0 + (kh[0] + 2.0*(kh[1]+kh[2]) + kh[3])/6.0;
		x0 = xn;
		y0 = yn;
		z0 = zn;
	}
	fclose(fp);
	fprintf(gp, "splot \"lorenz.dat\" w l\n");
	pclose(gp);
	return 0;
}
// eof
