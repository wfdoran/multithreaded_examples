#include <stdio.h>

int main(void) {
    int sum = 0;
    int n = 1000;
    
    #pragma omp parallel for reduction(+:sum) 
    for (int i = 1; i <= n; i++) {
        sum += i * i;
    }    
    
    int sum_sqr = n * (n + 1) * (2 * n + 1) / 6;
    printf("%d %d\n", sum_sqr, sum);
}
