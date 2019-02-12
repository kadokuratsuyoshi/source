#include <stdio.h>

#define HEIGHT 9
#define WIDTH 9

__global__ void kernel(int* target){
    int id = blockIdx.x * blockDim.x + threadIdx.x;

    if ( id < (HIGHT*WIDTH) ) {
        target[blockIdx.x*blockDim.x + threadIdx.x] = (blockIdx.x + 1) * (threadIdx.x + 1);
    }
}

int main(){
    int array[HEIGHT*WIDTH];
    int *device_array;

    cudaMalloc((void**)&device_array, sizeof(int) * HEIGHT*WIDTH);
    kernel<<<HEIGHT, WIDTH>>>(device_array);
    cudaMemcpy(array, device_array, sizeof(int) * HEIGHT*WIDTH, cudaMemcpyDeviceToHost);
    cudaFree(device_array);

    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            printf("%d\t", array[y*WIDTH + x]);
        }
        printf("\n");
    }
    return 0;
}
