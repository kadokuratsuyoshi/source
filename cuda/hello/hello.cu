#include <stdio.h>

__global__ void hello_world() {
    printf("hello, world\n");
}

int main(void){
    hello_world<<< 1, 1 >>>();
    return 0;
}
