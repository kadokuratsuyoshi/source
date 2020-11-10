#include <stdio.h>

double rk_v(double t,double x,double v) {
    return v;
}

double rk_a(double t,double x,double v) {
    double k = 1.0;
    double m = 1.0;
    double gamma = 1.0;
	
    return ( (-k*x - gamma*v)/m );
}

int main(void)
{
    double t=0.0;
    double x=1.0;
    double v=0.0;
    double dt=0.01;
    double dx, dv;
    double rdx[4], rdv[4];
    int i;

    for (i=0; i<1000; i++) {
        printf("%f %f %f\n", t, x, v);
        rdx[0] = dt * rk_v(t, x, v);
        rdx[1] = dt * rk_v(t+dt/2, x+rdx[0]/2, v+rdv[0]/2);
        rdx[2] = dt * rk_v(t+dt/2, x+rdx[1]/2, v+rdv[1]/2);
        rdx[3] = dt * rk_v(t+dt,   x+rdx[2],   v+rdx[2]);
        rdv[0] = dt * rk_a(t, x, v);
        rdv[1] = dt * rk_a(t+dt/2, x+rdx[0]/2, v+rdv[0]/2);
        rdv[2] = dt * rk_a(t+dt/2, x+rdx[1]/2, v+rdv[1]/2);
        rdv[3] = dt * rk_a(t+dt,   x+rdx[2],   v+rdv[3]);
        dx = (rdx[0]+2*rdx[1]+2*rdx[2]+rdx[3]) / 6;
        dv = (rdv[0]+2*rdv[1]+2*rdv[2]+rdv[3]) / 6;
	t = t + dt;
        x = x + dx;
        v = v + dv;
    }
    return 0;
}

//  eof
