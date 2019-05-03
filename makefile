quicksort: quicksort.c
	mpicc quicksort.c -o quicksort
run:
	mpirun -n 4 ./quicksort input output 1