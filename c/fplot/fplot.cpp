#include <iostream>
#include <fstream>
#include <cstdlib>
//#include <cstdio>
#define N (128)

using namespace std;

double psi;
char outst[32];

void out_psi(int fileno, double x, double psi) {
    int i;

    sprintf(outst, "a%03d", fileno);
    ofstream out(outst);
    if ( !out ) {
        exit(1);
    }
    out << x << " " << psi << endl;
    out.close();
}

int main(void) {
    int i, fileno;
    double x, dx;

    fileno = 0;
    x = -5.0;
    dx = 0.1;
    for ( i=0; i<200; i++ ) {
        psi = x * (x - 2.0) * (x - 2.0);
         //cout << scientific << x << ' ' << y << endl;
        out_psi(fileno, x, psi);
        x = x + dx; 
        fileno++;
    }
    return 0;
}
