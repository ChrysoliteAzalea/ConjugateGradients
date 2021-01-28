#include <mpi.h>
#include <cstdlib>
using namespace std;

double dabs(double x) {
	return x>=0?x:-x;
}

double Finderror(double *X,double *Y,int N) {
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm comm;
	double *Xbuffer;
	Xbuffer=new double[N/world_size];
	double *Ybuffer;
	Ybuffer=new double[N/world_size];
	double *curr;
	curr=new double[N/world_size];
	double *fulfill;
	fulfill=new double[N/world_size];
	MPI_Scatter(X,N/world_size,MPI_DOUBLE,Xbuffer,N/world_size,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Scatter(Y,N/world_size,MPI_DOUBLE,Ybuffer,N/world_size,MPI_DOUBLE,0,MPI_COMM_WORLD);
	for (int i=0;i<N/world_size;i++) curr[i]=dabs(Xbuffer[i]-Ybuffer[i]);
	MPI_Reduce(curr,fulfill,N/world_size,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	double Result=0;
	for (int i=0;i<N/world_size;i++) Result=fulfill[i]>Result?fulfill[i]:Result;
	delete [] Xbuffer;
	delete [] Ybuffer;
	delete [] curr;
	delete [] fulfill;
	return Result;
}
