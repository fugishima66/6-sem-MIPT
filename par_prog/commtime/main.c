#include <mpi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int commSize = 0;
	int myRank = 0;
	int buffer = 0;

	// Initialize MPI
	MPI_Init(&argc, &argv);
	
	// Get rank and communicator size
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (commSize != 2)
	{
		printf("Communicator size must be 2\n");
		return EXIT_FAILURE;
	}

	// MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();
	if (myRank == 1)
	{
		MPI_Recv(&buffer, 1, MPI_INT, 0, 56, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Message %d Recieved\n", buffer);
	}
	else
	{
		buffer = 272;
		MPI_Send(&buffer, 1, MPI_INT, 1, 56, MPI_COMM_WORLD);
		
	}

	if (myRank == 0)
	{
		MPI_Recv(&buffer, 1, MPI_INT, 1, 56, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Message %d Recieved\n", buffer);
	}
	else
	{
		buffer = 727;
		MPI_Send(&buffer, 1, MPI_INT, 0, 56, MPI_COMM_WORLD);
	}

	// MPI_Barrier(MPI_COMM_WORLD);
	double end = MPI_Wtime();

	if (myRank == 0)
	{
		printf("Measured time: %g us\n", 1e6 * (end - start));
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}