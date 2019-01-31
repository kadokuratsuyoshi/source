#include <stdio.h>
#include <math.h>

#define N (1000)

__global__ euler(int n, double *x, double *v, double dt){
    int id = blockIdx.x*blockDim.x + threadIdx.x;
    double dx, dv;
    double k = 1.0, m = 1.0, a = 1.0;

    if ( id < n ) {
        dx = v[id] * dt;
        dv = (-1.0*(k/m)*x[id] - (a/m)*v[id]) * dt;
        x[id] = x[id] + dx;
        v[id] = v[id] + dv;
    }
}

int main(void){
    double *x;
    double *v;
    double *d_x;
    double *d_v;

    x = (double *)malloc(sizeof(double) * N);
    v = (double *)malloc(sizeof(double) * N);
    cudaMalloc(&d_x, sizeof(double) * N);
    cudaMalloc(&d_v, sizeof(double) * N);
    for (int i=0; 1; i++ ) {
        cudaMemcpy(d_x, x, sizeof(double) * N, cudaMemcpyHostToDevice);
        cudaMemcpy(d_v, v, sizeof(double) * N, cudaMemcpyHostToDevice);
        euler<<< 1, 1 >>>(N, d_x, d_v, dt * i);
        cudaMemcpy(x, d_x, sizeof(double) * N, cudaMemcpyDeviceToHost);
        cudaMemcpy(v, d_v, sizeof(double) * N, cudaMemcpyDeviceToHost);
    }
    cudaFree(d_x);
    cudaFree(d_v);
    free(x);
    free(v);
    return 0;
}
