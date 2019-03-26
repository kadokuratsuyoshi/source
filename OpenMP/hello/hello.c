#include <omp.h>
#include <stdio.h>

int main(void){
#pragma omp parallel
    {
#ifdef _OPENMP
        printf("hello, world from %d of %d\n",
        omp_get_thread_num(), omp_get_num_threads());
#else
        printf("No OpenMP\n");
#endif
    }
    return 0;
}
