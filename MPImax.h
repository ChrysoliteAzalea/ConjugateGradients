#include <mpi.h>
#include <iostream>
#include <cmath>
using namespace std;

double Max(double *X,int n) {
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
	int perprocess=n/world_size>2?n/world_size:2;
	double *buffer;
	buffer=new double[perprocess];
	double *curr;
	curr=new double[perprocess];
	double *fulfill;
	fulfill=new double[perprocess];
	double ownmax;
	MPI_Scatter(X,perprocess,MPI_DOUBLE,buffer,perprocess,MPI_DOUBLE,0,MPI_COMM_WORLD);
	ownmax=buffer[0];
	for (int i=1;i<perprocess;i++) ownmax=ownmax<buffer[i]?buffer[i]:ownmax;
	curr[world_rank]=ownmax;
	MPI_Reduce(curr,fulfill,perprocess,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	double max=fulfill[0];
	for (int i=0;i<perprocess;i++) max=max<fulfill[i]?fulfill[i]:max;
	delete [] buffer;
	delete [] curr;
	delete [] fulfill;
	return max;
}
