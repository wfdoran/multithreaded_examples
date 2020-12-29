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


  
int worker(node_t *init_node) {
  int count = 0;

  int n = init_node->n;
  int max_stack = n * n;
  node_t **stack = malloc(max_stack * sizeof(node_t*));
  stack[0] = init_node;
  int stack_size = 1;

  while (stack_size > 0) {
    stack_size--;
    node_t *x = stack[stack_size];

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
	  stack[stack_size] = sub;
	  stack_size++;
	}
      }
    }

    free(x);
  }
  
  free(stack);
  return count;
}

int main(void) {
  node_t *init = malloc(sizeof(node_t));
  init->n = 10;
  init->m = 0;

  printf("%d\n", worker(init));

  return 0;
}
  
