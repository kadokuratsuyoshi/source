#include <stdio.h>
#include <math.h>

int main(void)
{
	double dt = 0.001;
	double dx, dy;
	double t;
	double x = 10.0;	// prey
	double y = 7.0;		// predator
	double dx0=0.0, dy0=0.0, dx1=0.0, dy1=0.0;
	double dxdt0, dydt0, dxdt1, dydt1;
	double a = 8.0;
	double b = 3.0;
	double c = 4.0;
	double d = 18.0;
	int i;

	for (i=0; i<5000; i++) {
		printf("%f %f %f\n", t, x, y);
		dxdt0 = a*x - b*x*y;
		dydt0 = c*x*y - d*y;
		dx0 = dxdt0 * dt;
		dy0 = dydt0 * dt;
		dxdt1 = a*(x+dx0) - b*(x+dx0)*(y+dy0);
		dydt1 = c*(x+dx0)*(y+dy0) - d*(y+dy0);
		dx1 = dxdt1 * dt;
		dy1 = dydt1 * dt;
		dx = (dx0 + dx1)/2.0;
		dy = (dy0 + dy1)/2.0;
		x = x + dx;
		y = y + dy;
		t = t + dt;
	}
	return 0;
}
