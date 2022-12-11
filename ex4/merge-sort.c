#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_SIZE 64

void generate_list(int *x, int n) {
   int i,j,t;
   for (i = 0; i < n; i++)
     x[i] = i;
   for (i = 0; i < n; i++) {
     j = rand() % n;
     t = x[i];
     x[i] = x[j];
     x[j] = t;
   }
}

void print_list(int * x, int n) {
   int i;
   for (i = 0; i < n; i++) {
      printf("%d ",x[i]);
   }
}

void merge(int * X, int n, int * tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;
   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; 
	 j++;
      }
   }
   while (i<n/2) { /* finish up lower half */
      tmp[ti] = X[i];
	ti++;
	i++;
   }
      while (j<n) { /* finish up upper half */
         tmp[ti] = X[j];
         ti++; 
	 j++;
   }
   memcpy(X, tmp, n*sizeof(int));

} // end of merge()

void mergesort(int * X, int n, int * tmp)
{
   if (n < 2) return;

   //#pragma omp task firstprivate (X, n, tmp)
   mergesort(X, n/2, tmp);

   //#pragma omp task firstprivate (X, n, tmp)
   mergesort(X+(n/2), n-(n/2), tmp);
	
   //Wait for both paralel tasks to complete execution
   //#pragma omp taskwait

    /* merge sorted halves into sorted list */
   merge(X, n, tmp);
}


int main()
{
   int n = 10;
   double start, stop;

   int data[MAX_SIZE], tmp[MAX_SIZE];

   generate_list(data, n);
   printf("Lista Antes da Ordenação...\n");
   print_list(data, n);
   start = omp_get_wtime();
   
   //#pragma omp parallel
   //{
   //   #pragma omp single
      mergesort(data, n, tmp);
   //}
  
   stop = omp_get_wtime();
   printf("\nLista Após Ordenação...\n");
   print_list(data, n);
   printf("\nTempo: %g\n",stop-start);
}
