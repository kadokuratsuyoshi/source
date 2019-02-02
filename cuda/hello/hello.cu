#include <stdio.h>

__global__ void hello_world() {
    printf("hello, world %d.%d\n", blockIdx.x, threadIdx.x);
}

int main(void){
    hello_world<<< 2, 2 >>>();
    cudaDeviceSynchronize();
    return 0;
}
