## fftw_helloworld.c
##### IN:
```
gcc fftw_helloworld.c -lfftw3 -lm
./a.out > ft.txt
python graph.py
```   
##### OUT:
![fft](Figure_1.png)

## fftw3-mpi_helloworld.c
##### IN:
```
gcc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich
mpiexec -n 4 ./a.out
```
##### OUT:
```
I am process 1 | local_n0 = 32 | local_0_start = 0    || alloc_local = 32
I am process 2 | local_n0 = 32 | local_0_start = 32    || alloc_local = 32
I am process 3 | local_n0 = 32 | local_0_start = 64    || alloc_local = 32
I am process 4 | local_n0 = 32 | local_0_start = 96    || alloc_local = 32
```
\* Update: now it also prints the input and the output (but in no particular order)
