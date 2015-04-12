#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int value = 7 ;

	int sum = 0 ;
	int rank;

	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	
	if ( rank == 0 )
	{
		value = 7;
	}
	else
	{
		value = 10;
	}

	MPI_Reduce(&value,&sum,2,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	printf("Sum is %d\n",sum );
	MPI_Finalize();
	return 0;
}