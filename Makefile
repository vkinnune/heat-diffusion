CC = mpicc
CFLAGS = -Wall -O2

all: heat heat_mpi

heat: heat.c
	$(CC) $(CFLAGS) -o heat heat.c

heat_mpi: heat_mpi.c
	$(CC) $(CFLAGS) -o heat_mpi heat_mpi.c

clean:
	rm -f heat heat_mpi
