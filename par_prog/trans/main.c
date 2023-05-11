#include <mpi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAG 123

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: ./%s <N>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int commSize = 0;
	int myRank = 0;

	// Get input argument
	unsigned long N = strtoul(argv[1], NULL, 10);

	// Initialize MPI
	MPI_Init(&argc, &argv);
	
	// Get rank and communicator size
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	int* matrix = malloc(sizeof(int) * N * N);

	if (matrix == NULL) {
		return EXIT_FAILURE;
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrix[i * N + j] = i * 10 + j;
		}
	}
	if (myRank == 0) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				printf("%d ", matrix[i * N + j]);
			}
			printf("\n");
		}
	}
	for (int i = myRank; i < N; i += commSize) {
		for (int j = 0; j < N; ++j) {
			if ((j - myRank) % commSize == 0 && j < i) {
				continue;
			}
			int tmp = matrix[i * N + j];
			matrix[i * N + j] = matrix[j * N + i];
			matrix[j * N + i] = tmp;
		}
	}
	if (myRank == 0) {
		for (int i = 1; i < N; ++i) {
			if (i % commSize == 0) {
				continue;
			}
			MPI_Recv(&matrix[i*N], N , MPI_INT , i % commSize, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				printf("%d ", matrix[i * N + j]);
			}
			printf("\n");
		}
	} else {
		for (int i = myRank; i < N; i += commSize) {
			MPI_Send(&matrix[i*N], N , MPI_INT , 0 , TAG , MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}