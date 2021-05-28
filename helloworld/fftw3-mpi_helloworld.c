#include <fftw3-mpi.h>
#include <stdio.h>
//#include <mpi.h>
#include <math.h>
//#define M_PI 3.14159265358979323846


void print_int_array(int arr[], int n){
    int i;
    for(i=0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void print_double_array(double arr[], int n){
    int i;
    for(i=0; i < n; i++)
        printf("%f ", arr[i]);
    printf("\n");
}

void print_complex_array(fftw_complex arr[], int n){
    int i;
    for(i=0; i < n; i++)
        printf("%f %f\n", arr[i][0],arr[i][1]);
    //printf("\n");
}

void print_labeled_processed_out(fftw_complex arr[], int n, int rnk){
    printf("# %d\n",rnk);
    print_complex_array(arr, n);
}

int main(int argc, char ** argv){
    
    /*x_values = np.arange(0, N, 1)

    x = np.sin((2*np.pi*x_values / 32.0)) # 32 - 256Hz
    x += np.sin((2*np.pi*x_values / 64.0)) # 64 - 128Hz*/

    const ptrdiff_t N = 128;
    fftw_complex *in, *out;
    fftw_plan p;
    ptrdiff_t alloc_local, local_ni, local_no, local_i_start, local_o_start;
    
    // It must be kind of pointless to be working in paralel and saving the whole sample in all processes but this is just for practice (the next 2 lines):
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    double x_values[N];
    for(int i = 0; i<128; i+=1){
        x_values[i] = i;
        in[i][1] = 0;
        in[i][0] = sin(2*M_PI*x_values[i] / 32);
        in[i][0] += sin(2*M_PI*x_values[i] / 64);
    }

    MPI_Init(&argc, &argv);
    fftw_mpi_init();

    int rnk;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    alloc_local = fftw_mpi_local_size_1d(N, MPI_COMM_WORLD,
                                        FFTW_FORWARD, FFTW_ESTIMATE,
                                        &local_ni, &local_i_start,
                                        &local_no, &local_o_start);
    printf("I am process %d | local_no = %td | local_o_start = %td    || alloc_local = %td\n", rnk+1, local_ni, local_i_start, alloc_local);
    
    
    fftw_complex *local_in, *local_out;
    //local_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * alloc_local);
    local_in = fftw_alloc_complex(alloc_local);
    local_out = fftw_alloc_complex(alloc_local);


    // NOW THAT WE HAVE ALLOCATED SPACE FOR THE DATA, WE MAKE A PLAN
    /*fftw_plan fftw_mpi_plan_dft_1d(ptrdiff_t n0, fftw_complex *in, fftw_complex *out,
                               MPI_Comm comm, int sign, unsigned flags);*/

    p = fftw_mpi_plan_dft_1d(N, local_in, local_out, MPI_COMM_WORLD, FFTW_FORWARD, FFTW_ESTIMATE); //in and out pointers refer only to a portion of the transform data

    
    // THEN WE HAVE TO INITIALLIZE THE DATA (in each individual process)
    ptrdiff_t i;
    for (i=0;i<local_ni;i++)
        local_in[i][0] = in[local_i_start+i][0];

    MPI_Barrier(MPI_COMM_WORLD);
    print_complex_array(local_in, local_ni);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);
    // NOW THE TRANSFORM
    fftw_execute(p);
    
    // NOW THE OUTPUT
    //print_complex_array(local_out, local_ni);
    print_labeled_processed_out(local_out, local_ni, rnk);  

    /*
    p = fftw_mpi_plan_dft_1d(N, in, out, MPI_COMM_WORLD, FFTW_FORWARD, FFTW_ESTIMATE);
    //...
    fftw_execute(p); // repeat as needed 
    //...
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
    */
    fftw_mpi_cleanup();
    MPI_Finalize();
    fftw_mpi_cleanup();
    
    //print_complex_array(out, N);
    
    return 0;
}
/*

Programs using the MPI FFTW routines should be linked 
with -lfftw3_mpi -lfftw3 -lm on Unix in double precision, 
-lfftw3f_mpi -lfftw3f -lm in single precision, and so on

también hay que linkear mpich con -lmpich al parecer entonces queda:
gcc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich
y luego.. :
mpiexec -n 4 ./a.out 



{
    fftw_complex *in, *out;
    fftw_plan p;
    ...
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    ...
    fftw_execute(p); // repeat as needed 
    ...
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}
*/