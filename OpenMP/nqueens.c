#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX (32)

typedef struct {
  int n;
  int m;
  int board[MAX];
} node_t;

typedef struct {
  int stack_size;
  node_t **stack;
}stack_t;

stack_t *stack_init(int n) {
  node_t *x = malloc(sizeof(node_t));
  x->n = n;
  x->m = 0;
  
  stack_t *s = malloc(sizeof(stack_t));
  s->stack = malloc(n * n * sizeof(node_t*));
  s->stack_size = 1;
  s->stack[0] = x;

  return s;
}

node_t *stack_get(stack_t *s) {
  if (s->stack_size == 0) {
    return NULL;
  }

  s->stack_size -= 1;
  return s->stack[s->stack_size];
}

void stack_put(stack_t *s, node_t *x) {
  s->stack[s->stack_size] = x;
  s->stack_size += 1;
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

    free(x);
  }
  
  return count;
}

int main(void) {
  stack_t *s = stack_init(10);
  printf("%d\n", worker(s));
  s = stack_destroy(s);
  
  return 0;
}
  
