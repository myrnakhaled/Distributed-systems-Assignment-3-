#include <iostream>
#include <mpi.h>
#include <math.h>
#define PI 3.14159265
using namespace std;


double fact(double x)
{
	x *= 2;
	double fact = 1;
	for (int i = 1; i <= x; i++)
		fact *= i;

	return fact;
}







int main(int argc, char** argv)
{

	//what will be provided by the user
	//n->processes num   i->uppercase
	//x-> the value to calculate the cos(x) for
	
	int upper_i, num_processes;
	double time1, time2, duration, global;
	double local_sum = 0;
	double x;

	// Initialize the MPI environment
	MPI_Init(&argc, &argv);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

	// Get the rank of the process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// root process acquire upper limit to i
	if (rank == 0)
	{
		scanf_s("%d", &upper_i);
		scanf_s("%lf", &x);
		printf("upper limit to i is %d, Number of processes is %d  ,x= %lf \n", upper_i, num_processes, x);
	}
	
	// Broadcast the upper limit to the rest of processes
	MPI_Bcast(&upper_i, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	// Synchronize processes (No process go beyond this line until all of then reaches it)
	MPI_Barrier(MPI_COMM_WORLD);

	time1 = MPI_Wtime();
	for (int i = rank; i < upper_i; i += num_processes)
	{
		local_sum += (pow(-1, i) * pow((x * (PI / 180.0)), 2 * i)) / fact(i);
		// printf("process %d doing iteration %d in upper_i %d \n", rank, i, upper_i);    
	}

	// Reduce operation to sum all local PIs into one variable in the root process (p0)
	double global_sum;
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// if root process => print the final value of pi
	if (rank == 0)
	{
		printf("cos(x)  %f \n", global_sum);
	}
	time2 = MPI_Wtime();
	duration = time2 - time1;

	// Get the max time value from all processes to print
	MPI_Reduce(&duration, &global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Global runtime is %f\n", global);
	}
	// Finalize the MPI environment.
	MPI_Finalize();



	return 0;
}

