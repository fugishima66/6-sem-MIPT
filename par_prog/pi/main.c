#include <mpi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double calculatePi(unsigned long startRange, unsigned long endRange)
{
	if (startRange > endRange)
	{
		return -1;
	}

	double result = 0;
    for (unsigned int i = startRange; i < endRange; ++i)
    {
        double tmp = 1. / (2. * i + 1.);
        if (i % 2 != 0)
            tmp *= -1;
        result += tmp;
    }

	return result;
}

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

	// Abort if N < commsize
	if (N < commSize)
	{
		printf("Error: N must be greater or equal than number of processors.\n");
		return EXIT_FAILURE;
	}

	// Determine which path of series should calculate
    unsigned long length = (N / commSize) + ((N % commSize > myRank) ? 1 : 0);
    unsigned long begin = length * myRank;
    printf("my rank %d, start %lu end %lu\n", myRank, begin, begin + length);
    double result = calculatePi(begin, begin + length);
    printf("my rank %d, my result %g, stark %lu end %lu\n", myRank, result, begin, begin + length);

    double globalResult = 0.0;
    MPI_Reduce(&result, &globalResult, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myRank == 0)
	{
		printf("Given argument: N = %lu\n", N);
		printf("Result: pi = %.20g\n", 4 * globalResult);
	}
	
	MPI_Finalize();

	return EXIT_SUCCESS;
}