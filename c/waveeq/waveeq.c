#include <stdlib.h> 
#include <stdio.h>
#include <math.h>

#define nx 51

int main(void)
{ 
    int i, j, nstep;
    double c, nu, x, dx, t, dt, u[nx], du[nx];     
    FILE *output;

    output = fopen("wave.dat","w");
    nu = 2.99792458;
    nstep = 1000;
    c = 0.01;
    dx = 1.0/nx;
    dt = nu*dx/c;
    t = 0.0;

    for (j=0; j<nx; j++) {
        x=dx*j;
        if (x < 0.5) {
            u[j]=1.0;
        }
        else {
            u[j]=0.0;
        }
    }
    for (j=0 ; j<nx; j++) {
        fprintf(output, "%f\n", u[j]);
    }
    fprintf(output, "\n");
   
    for (i=1; i<=nstep; i++) {
        for(j=1; j<(nx-1); j++) {
            du[j]=-nu/2.0*(u[j+1]-u[j-1]);
            du[j]=du[j];      // Forward in Time and Centered Space difference method (FTCS)
        }
        for (j=1; j<(nx-1); j++) {
            u[j] = u[j] + du[j];
        }
      
        for (j=0 ; j<nx; j++) {
            fprintf(output, "%f\n", u[j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);
}   
