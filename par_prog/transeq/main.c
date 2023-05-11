#include <mpi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TAG 123
#define M_PI 3.14159265358979323846

const double x0 = 0, x1 = 1;
const double t0 = 0, t1 = 1;
const double a = 2;
int nx = 0, nt = 0;
double hx = 0.0;
double ht = 0.0; 

double f(double x, double t) {
	return x + t;
}

double u_x0(double x) {
	return cos(M_PI * x);
}

double u_0t(double t) {
	return exp(-t);
}

double calcLeftCorner(double u_mk, double u_m_1k, double f_mk) {
	return u_mk * (1 - a * ht / hx) + a * ht / hx * u_m_1k + ht * f_mk;
}

double solution(double x, double t) {
	if (x >= a * t) {
		return x * t - 0.5 * t * t + cos(M_PI * (2 * t - x));
	} else {
		return x * t - 0.5 * t * t + (2 * t - x) * (2 * t - x) / 8 + exp(-t + 0.5 * x);
	}
}
int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Usage: ./%s <nx> <nt>\n", argv[0]);
		return EXIT_FAILURE;
	}

	nx = atoi(argv[1]);
	if (nx == 0) {
		printf("nx value is wrong\n");
		return EXIT_FAILURE;
	}

	nt = atoi(argv[2]);
	if (nx == 0) {
		printf("nt value is wrong\n");
		return EXIT_FAILURE;
	}

	hx = (x1 - x0) / (nx - 1.);
	ht = (t1 - t0) / (nt - 1.);

	int commSize = 0;
	int myRank = 0;

	// Initialize MPI
	MPI_Init(&argc, &argv);
	
	// Get rank and communicator size
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	double hx = (x1 - x0) / (nx - 1);
	double ht = (t1 - t0) / (nt - 1);

	double* mesh = malloc(sizeof(double) * nx * nt);

	if (mesh == NULL) {
		return EXIT_FAILURE;
	}

	for (int i = 0; i < nt; ++i) {
		mesh[i * nx] = u_0t(i * ht);
	}

	for (int j = 0; j < nx; ++j) {
		mesh[j] = u_x0(j * hx);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double startTime = MPI_Wtime();

	for (int i = myRank + 1; i < nt; i += commSize) {
		for (int j = 1; j < nx; ++j) {
			if (j != 1 && i != 1) {
				int recvRank = (myRank == 0) ? commSize - 1 : myRank - 1;
				MPI_Recv(&mesh[(i - 1) * nx + j - 1], 1, MPI_DOUBLE, recvRank, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			mesh[i * nx + j] = calcLeftCorner(mesh[(i-1) * nx + j], mesh[(i - 1) * nx + j - 1], f(i * hx, j * ht));\
			if (j != nx - 1) {
				int sendRank = (myRank == commSize - 1) ? 0 : myRank + 1;
				MPI_Send(&mesh[i * nx + j], 1, MPI_DOUBLE, sendRank, TAG, MPI_COMM_WORLD);
			}

		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double endTime = MPI_Wtime();
	if (myRank == 0)
		printf("%.3g\n", endTime - startTime);

	if (myRank == 0) {
		for (int i = 2; i < nt; ++i) {
			if ((i - 1) % commSize == 0) {
				continue;
			}
			MPI_Recv(&mesh[i * nx], nx, MPI_DOUBLE, (i - 1) % commSize, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		char outputFileName[1024];
		sprintf(outputFileName, "output_np_%d.csv", commSize);
		
		FILE* outputFile = fopen(outputFileName, "w");
		if (outputFile == NULL) {
			return EXIT_FAILURE;
		}

		fprintf(outputFile, "%g %g %g %d %g %g %g %d\n", x0, x1, hx, nx, t0, t1, ht, nt);

		for (int i = 0; i < nt; ++i) {
			for (int j = 0; j < nx; ++j) {
				fprintf(outputFile, "%g ", mesh[i * nx + j]);
			}
			fprintf(outputFile, "\n");
		}

		for (int i = 0; i < nt; ++i) {
			for (int j = 0; j < nx; ++j) {
				fprintf(outputFile, "%g ", solution(j * hx, i * ht));
			}
			fprintf(outputFile, "\n");
		}
	} else {
		for (int i = myRank + 1; i < nt; i += commSize) {
			MPI_Send(&mesh[i * nx], nx, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}