#include <stdio.h>
#include <mpi.h>

int main(int argc,char *argv[])
{
	int myid,num_process;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &num_process);

	printf("Hello I am process %d out of %d\n",myid,num_process);
	MPI_Finalize();
	return 0;
}