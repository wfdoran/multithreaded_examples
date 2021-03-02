#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void) {
  int sample_size = 10000000;
  int total = 0;

  #pragma omp parallel
  {
    int num_threads = omp_get_num_threads();
    int thread_id = omp_get_thread_num();
    int start = thread_id * sample_size / num_threads;
    int end = (thread_id + 1) * sample_size / num_threads;
    
    struct drand48_data rand;
    srand48_r(time(NULL) + thread_id * 0x5555, &rand);

    int my_total = 0;
    for (int i = start; i < end; i++) {
      double x, y;
      drand48_r(&rand, &x);
      drand48_r(&rand, &y);

      if (x * x + y * y <= 1.0) {
	my_total++;
      }
    }

    printf("%2d %8d %12.8f\n", thread_id, my_total,
	   4 * (double) my_total / (double) (end - start));
    #pragma omp atomic
    total += my_total;
  }
  
  double pi = 4.0 * (double) total / (double) sample_size;

  printf("\n");
  printf("%12.8f\n", pi);
  printf("%12.8f\n", M_PI);
  printf("%12.8f\n", 1.0 / sqrt(sample_size));
  return 0;
}
