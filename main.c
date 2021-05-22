#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "element.h"

int main(int argn,char **argc){

  // mpi
  MPI_Init(&argn, &argc);

  Element element;
  int x;
  if (argn > 1){
    element = new_Element(argc[1]);
    print_Element(element);
    //printf("%s %i\n",argc[1],argn);
  }else{
    printf("We need more arguments!\n");
  }

  // mpi
  MPI_Finalize();

  return 0;
}
