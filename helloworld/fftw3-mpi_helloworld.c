#include <fftw3-mpi.h>
#include <stdio.h>
//#include <mpi.h>
#include <math.h>
//#define M_PI 3.14159265358979323846


void print_complex_array(fftw_complex arr[], int n){
    int i;
    for(i=0; i < n; i++)
        printf("%f %f\n", arr[i][0],arr[i][1]);
}


int main(int argc, char ** argv){
    // DECLARING NECESSARY VARIABLES
    const ptrdiff_t N = 128;
    fftw_complex *in, *out;
    fftw_plan p;
    ptrdiff_t alloc_local, local_ni, local_no, local_i_start, local_o_start;
    
    // ALLOCATING AND LOADING TOY INPUT
    // It must be kind of pointless to be working in paralel and saving the whole sample in all processes but this is just for practice (the next 2 lines):
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    double x_values[N];
    for(int i = 0; i<N; i+=1){
        x_values[i] = i;
        in[i][1] = 0;
        in[i][0] = sin(2*M_PI*x_values[i] / 32);  // 32 - 256Hz   ..I think
        in[i][0] += sin(2*M_PI*x_values[i] / 64); // 64 - 128Hz   ..I think
    }

    MPI_Init(&argc, &argv);
    fftw_mpi_init();

    // FIGURING OUT HOW TO DIVIDE DATA INTO PROCESSES
    int rnk;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    alloc_local = fftw_mpi_local_size_1d(N, MPI_COMM_WORLD,
                                        FFTW_FORWARD, FFTW_ESTIMATE,
                                        &local_ni, &local_i_start,
                                        &local_no, &local_o_start);
    printf("I am process %d | local_no = %td | local_o_start = %td    || alloc_local = %td\n", rnk+1, local_ni, local_i_start, alloc_local);
    
    // ALLOCATING DATA (at individual processes)
    fftw_complex *local_in, *local_out;
    //local_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * alloc_local); // it can be done like this too
    local_in = fftw_alloc_complex(alloc_local);
    local_out = fftw_alloc_complex(alloc_local);


    // NOW THAT WE HAVE ALLOCATED SPACE FOR THE DATA, WE MAKE A PLAN
    p = fftw_mpi_plan_dft_1d(N, local_in, local_out, MPI_COMM_WORLD, FFTW_FORWARD, FFTW_ESTIMATE); //in and out pointers refer only to a portion of the transform data

    
    // THEN WE HAVE TO INITIALLIZE THE DATA (in each individual process)
    ptrdiff_t i;
    for (i=0;i<local_ni;i++)
        local_in[i][0] = in[local_i_start+i][0];

    MPI_Barrier(MPI_COMM_WORLD);

    // PRINTING THE INPUT (of each process)
    print_complex_array(local_in, local_ni);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);

    // NOW THE TRANSFORM
    fftw_execute(p);
    
    // PRINTING THE OUTPUT (of each process)
    print_complex_array(local_out, local_ni);
    
    
    // END AND CLEANUP
    fftw_mpi_cleanup();
    MPI_Finalize();
    fftw_mpi_cleanup();
        
    return 0;
}
/*

También hay que linkear mpich con -lmpich al parecer entonces queda:
gcc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich
y luego.. :
mpiexec -n 4 ./a.out 

TO DO:
- Reading input from file
- Printing input in order into file
*/