#include <stdio.h>
#include <mpi.h>

int main(int argc,char *argv[])
{
	int myrank;
	MPI_Init(&argc,&argv);
	printf("Hello World!");
	MPI_Finalize();
	return 0;
}