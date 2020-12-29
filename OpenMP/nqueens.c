#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX (32)

typedef struct {
  int n;
  int m;
  int board[MAX];
} node_t;

typedef struct {
  int num_waiting;
  int num_workers;
  int stack_size;
  node_t **stack;
}stack_t;

stack_t *stack_init(int n, int num_workers) {
  node_t *x = malloc(sizeof(node_t));
  x->n = n;
  x->m = 0;
  
  stack_t *s = malloc(sizeof(stack_t));
  s->stack = malloc(n * n * sizeof(node_t*));
  s->stack_size = 1;
  s->stack[0] = x;
  s->num_workers = num_workers;
  s->num_waiting = 0;

  return s;
}

node_t *stack_get(stack_t *s) {
#pragma omp critical(stack)
  {
  s->num_waiting += 1;
  }
  
  while (true) {
    node_t *rv;
    bool done = false;

#pragma omp critical(stack)
    {
    if (s->stack_size == 0 ) {
      if (s->num_waiting == s->num_workers) {
	rv = NULL;
	done = true;
      }
    } else {
      s->num_waiting -= 1;
      s->stack_size -= 1; 
      rv = s->stack[s->stack_size];
      done = true;
    }
    }

    if (done) {
      return rv;
    }
  }
}

void stack_put(stack_t *s, node_t *x) {
#pragma omp critical(stack)
  {
  s->stack[s->stack_size] = x;
  s->stack_size += 1;
  }
}

stack_t *stack_destroy(stack_t *s) {
  s->stack_size = 0;
  free(s->stack);
  s->stack = NULL;
  free(s);
  return NULL;
}
    
int worker(stack_t *s) {
  int count = 0;

  while (true) {    
    node_t *x = stack_get(s);
    if (x == NULL) {
      break;
    }
    int n = x->n;
    int m = x->m;

    if (m == n) {
      count++;
    } else {
      for (int i = 0; i < n; i++) {
	bool good = true;
	for (int j = 0; j < m && good; j++) {
	  int k = x->board[j];
	  int delta = m - j;
	  if (i == k || i == (k - delta) || i == (k + delta)) {
	    good = false;
	  }
	}

	if (good) {
	  node_t *sub = malloc(sizeof(node_t));
	  sub->n = n;
	  sub->m = m + 1;
	  memcpy(sub->board, x->board, m * sizeof(int));
	  sub->board[m] = i;

	  stack_put(s, sub);
	}
      }
    }

    x->m = -1;
    free(x);
  }
  
  return count;
}

int main(void) {
  int n = 12;
  int num_workers = 2;

  stack_t *s = stack_init(n, num_workers);
  int result[num_workers];

  #pragma omp parallel sections
  {
    #pragma omp section
    result[0] = worker(s);

    #pragma omp section
    result[1] = worker(s);
  }
 
  int total = 0;
  for (int i = 0; i < num_workers; i++) {
    total += result[i];
    printf("worker = %d count = %d\n", i, result[i]);
  }

  printf("total = %d\n", total);
 
  
  s = stack_destroy(s);
  return 0;
}

