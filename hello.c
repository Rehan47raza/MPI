Want to use open MP in linux. You need to follow these steps :

1. Install MPICC
	sudo apt-get install lam4-dev

2. Run lamboot
	lamboot ( Don't run it as root)

3. Now to compile a MPI program,
   mpicc hello.c

4. To run it
	mpirun -np 4 ./a.out

	here 4 gives the no. of processes

**** Hello World ***

hello.c file


#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[]) 
{
	int ierr;

	ierr = MPI_Init(&argc,&argv);	
	printf("Hello World.\n");
	ierr = MPI_Finalize();
	return 0;
}
----------------------------
Now to compile a MPI program,
   mpicc hello.c

To run it
	mpirun -np 4 ./a.out
--------------------------

#include <mpi.h>
#include <stdio.h>

int main(int argc,char *argv[])
{
	int myrank, size;

	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	printf("Hello world from %d out of %d\n",myrank,size );
	MPI_Finalize();
	return 0;
}

compile it as described above

Now I can use myrank to assign different task to different processes.

#include <stdio.h>
#include <mpi.h>

int main(int argc,char *argv[])
{
	int myrank,mysize;

	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&mysize);

	if ( myrank == 1 )
	{
		printf("Hello from %d process\n",myrank );
	}
	else
	{
		printf("Hello from other %d process\n",mysize - 1);
	}
	MPI_Finalize();
	return 0;
}

********Using Send or Recieve in MPI :: Communication routines****

MPI_Send(data_to_send,send_count,data_send_type,destination_id,tag,environment);

data_to_send :: Which data should be send. ( address of that data )
send_count :: The number of charachters to be send.
data_send_type :: Type of data to be sent.
destination_id :: the destination id 
tag :: identify each data.
environment :: generally MPI_COMM_WORLD.


MPI_Recv (data_to_recieve,recieve_count,data_recv_type,source_id,tag,environment,status);
data_to_recieve :: data type to be recieved.
recieve_count :: no of data to be recieved.
data_recv_type :: data to be recived type ( MPI_INT).
source_id :: the source id.
tag :: tag to identify sending data.
environment :: generally MPI_COMM_WORLD.
&	status :: MPI_Status type data.


-------------------------------------------------------------------

#include <stdio.h>
#include <mpi.h>

int main(int argc , char *argv[])
{
	int send,recieve,tag;
	MPI_Status MPI_STATUS_SIZE;
	int myrank;
	send = 1000;

	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

	if ( myrank == 1 )
	{
		printf("Sending data...\n");
		MPI_Send(&send,1,MPI_INT,0,1,MPI_COMM_WORLD);
	}
	if ( myrank == 0 )
	{
		printf("Recieving data..\n");
		MPI_Recv(&recieve,1,MPI_INT,1,1,MPI_COMM_WORLD,&MPI_STATUS_SIZE);

		printf("Data recieved data is  : %d\n",recieve );
	}
	MPI_Finalize();
	return 0;
}
--------------------------------------------------------------

Calculating sum of an array using MPI.
The question is : 
For a given array, enter the N ( size of the array ) .
Process 0 takes it (N) and sends it to all of the other processes.
All other processes revieves it and calculates the sum of all 
rank, rank + i * (no_of_processes ) elements
Now they send their sums to first process.
Process 0 calculates the sum.


#include <mpi.h>
#include <stdio.h>

int array[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

int main(int argc, char *argv[])
{
	int size,N,myrank,sum = 0,i;
	MPI_Status status;

// Initialize the MPI
	MPI_Init(&argc,&argv);
// Calculate rank
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
// Now check for first process

	if ( myrank == 0 )
	{
		printf("Enter the value of N\n");
		scanf("%d",&N);

		//Send N to all process

		for (i = 1; i < size; i++)
		{
			MPI_Send(&N,1,MPI_INT,i,i,MPI_COMM_WORLD);
		}
		// Now calculate the sum for this process
		for ( i = 0 ; i < N ; i += size )
		{
			sum = sum + array[i];
		}
		int temp;

		for ( i = 1 ; i < size ; i++ )
		{
			MPI_Recv(&temp,1,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			sum = sum + temp;
		}

		printf("Sum of the array is: %d\n",sum );
	}
	else
	{
		
// Recieving sum of other processes

		MPI_Recv(&N,1,MPI_INT,0,myrank,MPI_COMM_WORLD,&status);

// calculating sum of for other processes
		for ( i = myrank ; i < N ; i += size )
		{
			sum = sum + array[i];
		}
		printf("Sum returned by %d process is %d\n",myrank,sum );
// Sending data to first process
		MPI_Send(&sum,1,MPI_INT,0,myrank,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}



// Using broad-cast

#include <mpi.h>
#include <stdio.h>

int array[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

int main(int argc, char *argv[])
{
	int size,N,myrank,sum = 0,i;
	MPI_Status status;

// Initialize the MPI
	MPI_Init(&argc,&argv);
// Calculate rank
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
// Now check for first process

	if ( myrank == 0 )
	{
		printf("Enter the value of N\n");
		scanf("%d",&N);

		//broad_cast to all processes

		MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
		// for ( i = 1; i < size; i++)
		// {
		// 	MPI_Send(&N,1,MPI_INT,i,i,MPI_COMM_WORLD);
		// }
		// Now calculate the sum for this process
		for ( i = 0 ; i < N ; i += size )
		{
			sum = sum + array[i];
		}
		int temp;

		for ( i = 1 ; i < size ; i++ )
		{
			MPI_Recv(&temp,1,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			sum = sum + temp;
		}

		printf("Sum of the array is: %d\n",sum );
	}
	else
	{
		
// Recieving sum of other processes

		// MPI_Recv(&N,1,MPI_INT,0,myrank,MPI_COMM_WORLD,&status);
		MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
// calculating sum of for other processes
		for ( i = myrank ; i < N ; i += size )
		{
			sum = sum + array[i];
		}
		printf("Sum returned by %d process is %d\n",myrank,sum );
// Sending data to first process
		MPI_Send(&sum,1,MPI_INT,0,myrank,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}






