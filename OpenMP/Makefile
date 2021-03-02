CC = gcc
CFLAGS = -std=gnu99 -O2 -fopenmp

all: omp1 omp2

omp1: omp1.c
	$(CC) $(CFLAGS) omp1.c -o omp1

omp2: omp2.c
	$(CC) $(CFLAGS) omp2.c -o omp2

nqueens: nqueens.c
	$(CC) $(CFLAGS) nqueens.c -o nqueens

pi: pi.c
	$(CC) $(CFLAGS) pi.c -o pi -lm
