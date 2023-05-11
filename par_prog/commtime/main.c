#include <mpi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int power(int x, int y) {
	int res = x;
	for(int i = 1; i < y; ++i) {
		res *= x;
	}
	return res;
}

int main(int argc, char* argv[])
{
	int commSize = 0;
	int myRank = 0;
	int buffer = 0;
	FILE* outputFile = fopen("output.csv", "w");
	if (outputFile == NULL) {
		return EXIT_FAILURE;
	}

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
	for (int i = 0; i < 20; ++i) {
		buffer = power(2, i + 1);
		MPI_Barrier(MPI_COMM_WORLD);
		double start = MPI_Wtime();
		if (myRank == 1)
		{
			MPI_Recv(&buffer, 1, MPI_INT, 0, 56, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Message %d Recieved\n", buffer);
		}
		else
		{
			MPI_Send(&buffer, 1, MPI_INT, 1, 56, MPI_COMM_WORLD);
			
		}
		MPI_Barrier(MPI_COMM_WORLD);

		// MPI_Barrier(MPI_COMM_WORLD);
		double end = MPI_Wtime();

		if (myRank == 1)
		{
			printf("Measured time: %g us\n", 1e6 * (end - start));
			fprintf(outputFile, "%g ", 1e6 * (end - start));
		}

	}
	
	MPI_Finalize();

	return EXIT_SUCCESS;
}