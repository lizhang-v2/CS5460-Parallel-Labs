#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MAXCOUNT 65536

int main(int argc, char* argv[]) 
{
  float dot;
  float a[MAXCOUNT],b[MAXCOUNT],loc_dots[MAXCOUNT];
  int loc_n;
  int en,bn;
  MPI_Status  status;
  int n = MAXCOUNT;
  int i;
  for(i = 0;i<n;i++) {
    a[i] = i;
    b[i] = i+1;
  }
  
  int sz, my_rank;
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD,&sz);
  MPI_Comm_rank (MPI_COMM_WORLD,&my_rank);

  double startTime = MPI_Wtime();
  /* Each processor computes a local dot product */
  loc_n = n/sz;
  bn = 1+(my_rank)*loc_n;
  en = bn + loc_n-1;
  //printf("my_rank = %d loc_n = %d\n",my_rank,loc_n);
  //printf("my_rank = %d bn = %d\n",my_rank,bn);
  //printf("my_rank = %d en = %d\n",my_rank,en);
  
  float loc_dot = 0.0;
  for (i = bn;i <= en; i++) {
    loc_dot = loc_dot + a[i]*b[i];
  }
  printf("rank %d local_dot = %f\n",my_rank,loc_dot);
  /* mpi_gather sends and recieves all local dot products */
  /* to the array loc_dots in processor 0. */
  MPI_Gather(&loc_dot,1,MPI_FLOAT,&loc_dots,1,MPI_FLOAT,0,
                        MPI_COMM_WORLD);
  /* Processor 0 sums the local dot products. */
  if (my_rank == 0) {
    dot = loc_dot;

  int source;
  for (source = 1;source <= sz-1; source++) {
    dot = dot + loc_dots[source];
  }
    printf( "dot product = %f\n",dot);
  }

  double endTime = MPI_Wtime();
  printf("Compute dot product on rank %d took %f seconds\n", my_rank, endTime - startTime);
      
  MPI_Finalize();
  exit (0);
}
