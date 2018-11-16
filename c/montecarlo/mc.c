#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double mc(int a, int b){
    int i;
    int n;
    double x,y;

    srand(a);
    n = 0;
    printf("\n");
    for (i=0; i<b; i++) {
        x = (double)rand()/(double)RAND_MAX;
        y = (double)rand()/(double)RAND_MAX;
        if ( x*x + y*y < 1.0 ) {
            n++;
        }
        if ( i % 1000000 == 0 ) {
            printf("c");
        }
    }
    printf("\n");
    return 4.0 * (double)n / (double)b;
}

int main(void){
    double pi;
    
    pi = mc(1, 100000000);
    printf("%f \n", pi);
    return 0;
}
