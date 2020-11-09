#include "stdio.h"
#include "mpi.h"
#include <math.h>

// to find nth element of the taylor series
double getTaylorElement(int n){

	/*
		Taylor series: 1/1 - 1/3 + 1/5 ...	
	*/
	
	// Calculating the value of the nth elemnt
	double value = 1.0 / ( (2.0 * n) + 1.0);

	/*
		Calculating sign of the nth element

		Description:
			Even Order elements added
			Odd Order elements substracted
			Example: 
				0th element : +1/1
				1th element : -1/3
				2th element : +1/5
	*/
	return (n % 2 == 0)? value : -value;
}

int main(argc,argv)

int argc; // number of arguments
char *argv[]; // arguments

{

	///////////// Variables /////////////
	int numOfTerms; 	// The precision of the prediction.
	int nodeId; 		// node id
	int numberOfNodes; 	// number of process working in nodes in parallel
    
	double PI25DT = 3.141592653589793238462643; // to calculate error

	/* Local Variables (For each parallel process) */
	double nodeSum = 0; // summation calculated each of parallel process
	double totalSum; 	// summation of values calculated by each parallel process
	double predictedPi; // predicted pi number

	///////////// Initialize /////////////
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numberOfNodes);
	MPI_Comm_rank(MPI_COMM_WORLD,&nodeId);
   
	/* Getting Arguments (number of parallel processes) */
	if (nodeId == 0) {
	    printf("Enter the number of intervals: (0 quits) ");
	    scanf("%d",&numOfTerms); // getting number of terms from user
	}

	MPI_Bcast(&numOfTerms, 1, MPI_INT, 0, MPI_COMM_WORLD);
	

	///////////// Parallel Computing /////////////
	if (numOfTerms == 0){
		return 0;
	}

	/* Calculating Partual Summation */
	for(int i = nodeId; i<numOfTerms; i += numberOfNodes){
		nodeSum += getTaylorElement(i);
	}
	printf("NodeId=[%d] Summation=[%.4f]\n", nodeId, nodeSum);
	

	///////////// Getting Result /////////////
	MPI_Reduce(&nodeSum, &totalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	/* Printing the results */
	if (nodeId == 0){
		predictedPi = 4.0 * totalSum;
		 printf("pi is approximately %.16f, Error is %.16f\n", predictedPi, fabs(predictedPi - PI25DT));
	}
	      
	MPI_Finalize();
	return 0;
}
