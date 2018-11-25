/*
  wf.cpp
  solve the Schrodinger equation using runge-kutta regime
*/
#include <stdio.h>
#include <math.h>
#include "cmplx.h"

#define DX 0.004
#define DT DX*DX/4.0
#define M_MAX 500
#define M_TOTAL M_MAX+1
#define N_MAX 800
#define N_TOTAL N_MAX+1
#define H_SLASH 1
#define MASS 0.5
#define D 0.035
#define X_KAKKO -0.3
#define P_KAKKO 50.0*M_PI
#define A 0.032
#define DEPTH 50
#define V0 (DEPTH * M_PI) * (DEPTH * M_PI)

cmplx Psi[N_TOTAL][M_TOTAL];
cmplx K1[M_TOTAL];
cmplx K2[M_TOTAL];
cmplx K3[M_TOTAL];
cmplx K4[M_TOTAL];
double V[M_TOTAL];
cmplx I = cmplx(0, 1);

double getX(int m){
  double x = (m-M_MAX/2.0)*DX;
  return x;
}

double getT(int n){
  double t = n*DT;
  return t;
}

cmplx wavefunc(int m){
  cmplx z, w;
  double x = getX(m);
  double re, im;
  cmplx tonorm;

  re = -(x-X_KAKKO)*(x-X_KAKKO)/4.0/D/D;
  im = P_KAKKO*x/H_SLASH;
  z = exp_(cmplx(re, im));
  tonorm = cmplx(1.0/pow(2.0*M_PI*D*D, 1.0/4.0), 0);
  z = tonorm * z;

  return z;
}

double getV(int m){
  double abs_x;
  double x = getX(m);

  if ( x < (-1.0 * A) ) {
    return 0.0;
  }
  else {
    return V0;
  }
}

void init(void){
  int m, n;
  cmplx c_zero = cmplx(0.0, 0.0);

  // n=0 n=M_TOTAL
  for(n=0; n<=N_MAX; n++){
    Psi[n][0] = c_zero;
    Psi[n][M_MAX] = c_zero;
  }

  for(m=1; m<=M_MAX-1; m++){
    Psi[0][m] = wavefunc(m);
  }

  for(m=0; m<=M_MAX; m++){
    V[m] = getV(m);
  }
}

cmplx calculate_k1(int m, int n){
  cmplx z, w;

  z = cmplx(2.0, 0.0) * Psi[n][m];
  z = Psi[n][m+1] - z;
  z = Psi[n][m-1] + z;
  z = cmplx(H_SLASH * H_SLASH / 2.0 / MASS / DX / DX, 0) * z;
  w = cmplx(V[m], 0) *  Psi[n][m];
  z = z - w;
  z = I * z;
  z = cmplx(1.0 / H_SLASH, 0) * z;
  return z;
}

cmplx calculate_psi_plus_k(cmplx psi, double r, cmplx k){
  cmplx z;

  z = psi + cmplx(r * DT, 0) * k;
  return z;
}

cmplx calculate_k2(int m, int n){
  cmplx z, w, v;
  w = calculate_psi_plus_k(Psi[n][m], 1.0/2.0, K1[m]);
  v = cmplx(V[m], 0) * w;
  z = cmplx(2.0, 0) * w;
  w = calculate_psi_plus_k(Psi[n][m+1], 1.0/2.0, K1[m+1]);
  z = w - z;
  w = calculate_psi_plus_k(Psi[n][m-1], 1.0/2.0, K1[m-1]);
  z = w + z;
  z = cmplx(H_SLASH *H_SLASH / 2.0 / MASS / DX / DX, 0) * z;
  z = z - v;
  z = I * z;
  z = cmplx(1.0 / H_SLASH, 0) * z;
  return z;
}

cmplx calculate_k3(int m, int n){
  cmplx z, w, v;
  w = calculate_psi_plus_k(Psi[n][m], 1.0/2.0, K2[m]);
  v = cmplx(V[m], 0) * w;
  z = cmplx(2.0, 0) * w;
  w = calculate_psi_plus_k(Psi[n][m+1], 1.0/2.0, K2[m+1]);
  z = w - z;
  w = calculate_psi_plus_k(Psi[n][m-1], 1.0/2.0, K2[m-1]);
  z = w + z;
  z = cmplx(H_SLASH*H_SLASH/2.0/MASS/DX/DX, 0) * z;
  z = z - v;
  z = I * z;
  z = cmplx(1.0/H_SLASH, 0) * z;
  return z;
}

cmplx calculate_k4(int m, int n){
  cmplx z, w, v;

  w = calculate_psi_plus_k(Psi[n][m], 1.0, K3[m]);
  v = cmplx(V[m], 0) * w;
  z = cmplx(2.0, 0) * w;
  w = calculate_psi_plus_k(Psi[n][m+1], 1.0, K3[m+1]);
  z = w - z;
  w = calculate_psi_plus_k(Psi[n][m-1], 1.0, K3[m-1]);
  z = w + z;
  z = cmplx(H_SLASH*H_SLASH/2.0/MASS/DX/DX, 0) * z;
  z = z - v;
  z = I * z;
  z = cmplx(1.0/H_SLASH, 0) * z;
  return z;
}

cmplx runge_kutta(int m, int n){
    cmplx z;

    z = Psi[n][m] + cmplx(DT/6.0, 0) * (K1[m] + cmplx(2.0, 0) * K2[m] + cmplx(2.0, 0) * K3[m] + K4[m]);
    return z;
}

void calculate_psi(int n){
  int m;

  for(m=1; m<=M_MAX-1; m++){
    K1[m] = calculate_k1(m,n);
  }
  for(m=1; m<=M_MAX-1; m++){
    K2[m] = calculate_k2(m,n);
  }
  for(m=1; m<=M_MAX-1; m++){
    K3[m] = calculate_k3(m,n);
  }
  for(m=1; m<=M_MAX-1; m++){
    K4[m] = calculate_k4(m,n);
  }
  for(m=1; m<=M_MAX-1; m++){
    Psi[n+1][m] = runge_kutta(m,n);
  }
}

void writetitle(FILE *file_open, int n){
    printf("time N=%d\n", n);
    if(file_open != NULL){
        fprintf(file_open, "time N=%d\n", n);
    }
}

void writedata(FILE *file_open, int m, double x){
    printf("%f, %10.3e\n", getX(m), x);
    if(file_open != NULL){
        fprintf(file_open, "%f, %10.3e\n", getX(m), x);
    }
}

int main(void){
    char name[] = "wf";
    char filename[100];
    FILE * file_open = NULL;
    double x;
    int m, n, n_out = 0.8*(N_MAX/4);
    int i, di;
    int num = 0;

    di = N_MAX/40;
    num = 0;
    for (i=0; i<N_MAX; i+=di ) {
        n_out = i;
        sprintf(filename, "%s_%d", name, num );
        file_open = fopen(filename, "wt");
        writetitle(file_open, num);
        init();
        num++;
        for (n=0; n<=N_MAX-1; n++) {
            calculate_psi(n);
        }
        n = n_out;
        for(m=0; m<=M_MAX; m++){
            x = abs(Psi[n][m]);
            writedata(file_open, m, x*x);
        }
    }
    return 0;
}
//  eof
