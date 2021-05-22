#include <mpi.h>
#include <stdio.h>

void print_array(int arr[], int n){
    int i;
    for(i=0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(int argc, char ** argv){
    
    int rank, data[10] = {0,1,2,3,4,5,6,7,8,9};
    
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0){
        printf("World size: %d\n", world_size);
        printf("Processor name: %s\n", processor_name);
        printf("I am %d of %d\n", rank+1, world_size);

        MPI_Send(data, 10, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    else if (rank == 1){
        int new_data[10];
        MPI_Recv(new_data, 10, MPI_INT, 0, 0, MPI_COMM_WORLD,  MPI_STATUS_IGNORE);
        printf("%d received data\n", rank+1);
        printf("Data: ");
        print_array(new_data, 10);
        printf("I am %d of %d\n", rank+1, world_size);
    }

    else
        printf("I am %d of %d\n", rank+1, world_size);


    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0)
        printf("Finished!\n");

    MPI_Finalize();
    return 0;
    
}

//mpiexec -n 2 ./hpc20212