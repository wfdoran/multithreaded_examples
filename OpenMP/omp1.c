#include <stdio.h>
#include <omp.h>

void main(void) {
    int last = -1;
    #pragma omp parallel num_threads(3)
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        printf("Hello from thread %d of %d \n", thread_id, num_threads);
        last = thread_id;
    }
    
    printf("last = %d\n", last);
}
