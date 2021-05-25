#include <fftw3.h>
#include <stdio.h>
#include <math.h>
#define M_PI 3.14159265358979323846


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

int main(){
    /*x_values = np.arange(0, N, 1)

    x = np.sin((2*np.pi*x_values / 32.0)) # 32 - 256Hz
    x += np.sin((2*np.pi*x_values / 64.0)) # 64 - 128Hz*/
    // int N = 128;
    // double x_values[N], x_1[N], x_2[N];
    // for(int i = 0; i<128; i+=1){
    //     x_values[i] = i;
    //     x_1[i] = 2*M_PI*x_values[i] / 32;
    //     x_2[i] = 2*M_PI*x_values[i] / 64;
    // }

    //print_double_array(x_values,n);
    //print_double_array(x_1,n);
    //print_double_array(x_2,n);

    fftw_complex *in, *out;
    fftw_plan p;

    int N = 128;
    //double x_values[N], x_1[N], x_2[N];
    //...
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    double x_values[N];
    for(int i = 0; i<128; i+=1){
        x_values[i] = i;
        in[i][1] = 0;
        in[i][0] = 2*M_PI*x_values[i] / 32;
        in[i][0] += 2*M_PI*x_values[i] / 64;
    }

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    //...
    fftw_execute(p); // repeat as needed 
    //...
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    print_complex_array(out, N);
    
    return 0;
}
/*
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