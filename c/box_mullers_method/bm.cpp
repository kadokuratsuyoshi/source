//  Box-Muller's method
//  f = 1/2pi exp(-x^2+y^2/2) Gaussian distribution
//  theta = arctan z2/z1
//  r^2 = z1^2 + z2^2
//  z1 = r cos theta = sqrt(-2 ln x) cos 2 pi y
//  z2 = r sin theta = sqrt(-2 ln x) sin 2 pi y
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

double pi = M_PI;

void boxmuller (double mu,double sigma,int imax) {
    double num1,num2,x1,x2,z1,z2;

    for (int i = 0; i < imax; i++){
	    // homogeneous randomness from 0 to 1. 0~1
	    num1 = (double)rand()/(double)RAND_MAX;
	    num2 = (double)rand()/(double)RAND_MAX;
	    // attempt the box-muller method 
	    x1 = sqrt(-2.0*log(num1))*cos(2*pi*num2);
	    x2 = sqrt(-2.0*log(num1))*sin(2*pi*num2);
	    // linear transration
	    z1  = mu + sigma*x1;
	    z2  = mu + sigma*x2;
	    cout << i << " " << z1 << " " << z2 << endl; 
    }
}
int main(void) {
	// mean, deviation, number
    boxmuller(0,1,1000);	
}
