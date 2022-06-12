#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "mpi.h"



int mod(int a, int b){
   if(b < 0) //you can check for b == 0 separately and do what you want
     return -mod(-a, -b);
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}

int isPerfectSquare(int n, int* p_sqrt){
	long double sr = sqrt(n);
	  // If square root is an integer
	if(sr - floor(sr) == 0){
		*p_sqrt = sr;
		return 1;
	}
    return 0; //
}

// t - times, rowcol - square matrix (same dimensions)
int shiftLeftTimes(int i, int j, int rowcol, int t){
	return i*rowcol+mod(j-t, rowcol);
}

int shiftUpTimes(int i, int j, int rowcol, int t){
	return j+rowcol*mod(i-t, rowcol);
}

void getDispls(int** sendcounts, int** displsA, int** displsB, int** Realdispls, int p, int p_sqrt, int n){
	*sendcounts = calloc(p, sizeof(int));
	*displsA = calloc(p, sizeof(int));
	*displsB = calloc(p, sizeof(int));
	*Realdispls = calloc(p, sizeof(int));

	for (int i=0; i<p; i++)
		*(*sendcounts + i) = 1;
	int disp = 0;
	for (int i=0; i<p_sqrt; i++) {
		for (int j=0; j<p_sqrt; j++) {
			*(*displsA + shiftLeftTimes(i, j, p_sqrt, i)) = disp;
			*(*displsB + shiftUpTimes(i, j, p_sqrt, j)) = disp;
			*(*Realdispls + (i*p_sqrt+j)) = disp;
			disp += 1;
		}
		disp += ((n / p_sqrt) - 1) * p_sqrt;
	}
}

void mulMat(double* A, double* B, double* C, int rowcol){
	for(int i=0; i<rowcol; i++){
		for(int j=0; j<rowcol; j++){
			for(int k=0; k<rowcol; k++){
				C[i*rowcol + j] += A[i*rowcol + k] * B[k*rowcol + j];
			}
		}
	}
}

void readMatrixFromFile(double** m, int* n, int otherRowCol, int p_sqrt, const char* filename){
	// n pointe fix i ucitaj n prvi red
	FILE *pf;
	pf = fopen(filename, "r");
	if (pf == NULL){
		fprintf(stderr, "\nERROR WITH FILE READING\n");
		MPI_Abort(MPI_COMM_WORLD, 0);;
	}

	fscanf(pf, "%d", n);
	int rowcol = *n;
	if(otherRowCol == -1)	{
		if(rowcol % p_sqrt != 0){
			fprintf(stderr, "\nMatrix size must be divisible with sqrt(number processes)!\n");
				MPI_Abort(MPI_COMM_WORLD, 0);
		}
	}
	else if(otherRowCol != rowcol){
		fprintf(stderr, "\nMatrices A and B must be same size!\n");
		MPI_Abort(MPI_COMM_WORLD, 0);
	}

	*m = calloc(rowcol*rowcol, sizeof(double));

	for(size_t i = 0; i < rowcol; i++)
	{
		for(size_t j = 0; j < rowcol; j++)
			fscanf(pf, "%lf", *m + i * rowcol + j);
	}


	fclose(pf);
}

int printMatrixToFile(double *m, int row, int col, const char* filename){
	FILE *fptr;
	fptr = fopen(filename, "w");

	if (fptr == NULL){
		printf("ERROR WITH FILE Writing");
		return 0;
	}

	fprintf(fptr, "%d\n", row);
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			fprintf(fptr, "%lf", m[i*col+j]);
			if(j != col-1)
				fprintf(fptr, " ");
		}
		fprintf(fptr, "\n");
	}
	fprintf(fptr, "\n");

	fclose(fptr);
	return 1;
}

int appendBenchmarkToFile(int p, double t, int size, int same, const char* filename){
	FILE *fptr;
	fptr = fopen(filename, "a");

	if (fptr == NULL){
		printf("ERROR WITH FILE Writing/Appending");
		return 0;
	}

	fprintf(fptr, "Number of processes - %d, matrix %5dx%5d, time - %.5lf. Matrix mult is correct (0 -> yes, else -> no): %d\n", p, size, size, t, same);


	fclose(fptr);
	return 1;
}
void printM(double* m, int row, int col){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			printf("%12.7lf", m[i*col+j]);
		}
		printf("\n");
	}
	printf("\n");
}
int main(int argc, char* argv[]){
	int  my_rank; /* rank of process */
	int  p;       /* number of processes */
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	int p_sqrt;
	if(isPerfectSquare(p, &p_sqrt) == 0){
		if(my_rank==0)
			fprintf(stderr, "\nNumber of processes must be perfect square!\n");
		MPI_Abort(MPI_COMM_WORLD, 0);
	}
	
	int sizeA;
	double *A, *B, *C;
	double startTime, endTime;


	if(my_rank == 0){
		if(argc != 6){
			fprintf(stderr, "\nmpirun must take 6 arguments ! Read format !\n");
			MPI_Abort(MPI_COMM_WORLD, 0);
		}
		int sizeB = -1;
		sizeA = -1;
		readMatrixFromFile(&A, &sizeA, sizeB, p_sqrt, argv[1]);
		readMatrixFromFile(&B, &sizeB, sizeA, p_sqrt, argv[2]);

		if(sizeB != sizeA){
			fprintf(stderr, "\nsizeA = %d, sizeB = %d\n", sizeA, sizeB);
			fprintf(stderr, "\nMatrix dimension A and B are not the same!\nABORT!\n");
			MPI_Abort(MPI_COMM_WORLD, 0);
		}

		C=calloc(sizeA*sizeA, sizeof(double));
		startTime = MPI_Wtime();
	}
	MPI_Bcast(&sizeA, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int local_n = sizeA / p_sqrt;


	double *locA=calloc(local_n*local_n, sizeof(double));
	double *locB=calloc(local_n*local_n, sizeof(double));
	
	// reserve memory
	double *locARes=calloc(local_n*local_n, sizeof(double));
	double *locBRes=calloc(local_n*local_n, sizeof(double));
	
	double *locC=calloc(local_n*local_n, sizeof(double));


	MPI_Datatype grid, gridtype;
	MPI_Type_vector(local_n, local_n, sizeA, MPI_DOUBLE, &grid);
	MPI_Type_commit(&grid);
	MPI_Type_create_resized(grid, 0, local_n*sizeof(double), &gridtype);
	MPI_Type_commit(&gridtype);

	int *sendcounts;
	int *displsA;
	int *displsB;
	int *Realdispls;

	if (my_rank == 0) {
		getDispls(&sendcounts, &displsA, &displsB, &Realdispls, p, p_sqrt, sizeA);
	}


	MPI_Scatterv(A, sendcounts, displsA, gridtype, locA, local_n*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatterv(B, sendcounts, displsB, gridtype, locB, local_n*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (my_rank == 0){
		free(displsA);free(displsB);
	}

	mulMat(locA, locB, locC, local_n);

	double* tmp;
	int i = my_rank / p_sqrt;
	int j = my_rank % p_sqrt;
	int left_send = shiftLeftTimes(i, j, p_sqrt, 1);
	int left_recv = shiftLeftTimes(i, j, p_sqrt, -1);
	int up_send = shiftUpTimes(i, j, p_sqrt, 1);
	int up_recv = shiftUpTimes(i, j, p_sqrt, -1);
	
	
  	MPI_Request reqs[2];
  	MPI_Status stat[2];
	
	for(int ii=0; ii<p_sqrt-1;ii++){				
		
		MPI_Isend(locA, local_n*local_n, MPI_DOUBLE, left_send, 0, MPI_COMM_WORLD, &reqs[0]);
		MPI_Isend(locB, local_n*local_n, MPI_DOUBLE, up_send, 1, MPI_COMM_WORLD, &reqs[1]);
		MPI_Recv(locARes, local_n*local_n, MPI_DOUBLE, left_recv, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(locBRes, local_n*local_n, MPI_DOUBLE, up_recv, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		

		mulMat(locARes, locBRes, locC, local_n);
		
		MPI_Waitall(2, reqs, stat);
		tmp = locARes;
		locARes = locA;//ne bitno
		locA = tmp;		
		
		tmp = locBRes;
		locBRes = locB;//ne bitno
		locB = tmp;		
	}	

	MPI_Gatherv(locC, local_n*local_n, MPI_DOUBLE, C, sendcounts, Realdispls, gridtype, 0, MPI_COMM_WORLD);

	free(locA);free(locB);free(locARes);free(locBRes);free(locC);

	if (my_rank == 0){
		endTime = MPI_Wtime();
		double p_time = endTime - startTime;
		printf("Number of processes = %3d, matrix %5d  x%5d, time -> %6.2lf, 1 core approx -> %6.2lf\n", p, sizeA, sizeA, p_time, p_time*p);

		// read in GOOD C (precomputed)
		double* goodC;
		readMatrixFromFile(&goodC, &sizeA, sizeA, p_sqrt, argv[3]);

		// double might be prone to making errors 
		int same = memcmp(C, goodC, sizeof(C));
		printf("Matrix mult is correct (0 -> yes, else -> no): %d\n\n", same);


		printMatrixToFile(C, sizeA, sizeA, argv[4]);
		appendBenchmarkToFile(p, p_time, sizeA, same, argv[5]);

		free(A);free(B);free(C);free(goodC);
		free(Realdispls);free(sendcounts);
	}

	MPI_Finalize();

	return 0;
}


