#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

typedef struct {
  int32_t thread_id;
  int32_t num_threads;
  int64_t n;
} state_t;

void *sum_sqr(void *arg) {
  state_t *s = (state_t*) arg;
  int64_t sum = 0;
  for (int64_t i = s->thread_id; i <= s->n; i += s->num_threads) {
    sum += i * i;
  }
  int64_t *rv = malloc(sizeof(int64_t));
  *rv = sum;
  return (void*) rv;
}

int main(void) {
  int32_t num_threads = 4;
  int64_t n = 10000;
  
  state_t s[num_threads];
  pthread_t thread[num_threads];
  for (int i = 0; i < num_threads; i++) {
    s[i].thread_id = i;
    s[i].num_threads = num_threads;
    s[i].n = n;

    pthread_create(&thread[i], NULL, sum_sqr, &s[i]);
  }

  int64_t total = 0;
  for (int i = 0; i < num_threads; i++) {
    int64_t *rv;
    pthread_join(thread[i], (void*) &rv);
    total += *rv;
    free(rv);
  }

  printf("%ld\n", total);
  printf("%ld\n", n * (n + 1) * (2 * n + 1) / 6);

  return 0;
}
