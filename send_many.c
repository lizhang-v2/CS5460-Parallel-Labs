#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXCOUNT 1073741824

int main(int argc, char** argv) {
  int sz, myid;
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &sz);
  MPI_Comm_rank (MPI_COMM_WORLD, &myid);
  // We are assuming at least 2 processes for this task
  if (sz < 2) {
    printf("World size must be greater than 1\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  long count = 1;
  int number = 1;
  int i = 0;
  while (count <= MAXCOUNT) {
      if (myid == 0) {
        while (count < MAXCOUNT) {
          // If we are  0, set the number to 1 and send it to process 1
          double starttime, endtime;
          starttime = MPI_Wtime();
          for (i = 0; i < count; i++) {
            MPI_Send(&number, 0, MPI_INT, 1, 0, MPI_COMM_WORLD);
          }
          endtime   = MPI_Wtime();
          count *= 2;
          printf("Sending %d times of int took %f seconds\n", count, 
endtime-starttime);
        }
      } else if (myid == 1) {
        MPI_Recv(&number, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE);
      }
  }
  MPI_Finalize();
  exit(0);
}
