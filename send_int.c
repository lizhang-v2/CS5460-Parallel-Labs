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
  int* buffer;
  while (count <= MAXCOUNT) {
    buffer = (int *) calloc(count, sizeof(MPI_INT));
    if (buffer) {
      if (myid == 0) {
        // If we are  0, set large buffer and send it to process 1
        double starttime, endtime;
        starttime = MPI_Wtime();
        MPI_Send(buffer, count, MPI_INT, 1, 0, MPI_COMM_WORLD);
        endtime   = MPI_Wtime();
        printf("Sending %d integers took %f seconds\n", count, 
endtime-starttime);
      } else if (myid == 1) {
        MPI_Recv(buffer, count, MPI_INT, 0, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE);
      }
    free(buffer);
    }
    count *= 2;
  }
  MPI_Finalize();
  exit(0);
}
