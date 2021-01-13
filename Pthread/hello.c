#include <stdio.h>
#include <pthread.h>

typedef struct {
  int thread_id;
  int num_threads;
} state_t;

void *hello(void *arg) {
  state_t *s = (state_t*) arg;
  printf("Hello from %d of %d\n", s->thread_id, s->num_threads);
  return NULL;
}

int main(void) {
  int num_threads = 4;

  state_t s[num_threads];
  pthread_t thread[num_threads];
  for (int i = 0; i < num_threads; i++) {
    s[i].thread_id = i;
    s[i].num_threads = num_threads;

    pthread_create(&thread[i], NULL, hello, &s[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(thread[i], NULL);
  }

  return 0;
}
