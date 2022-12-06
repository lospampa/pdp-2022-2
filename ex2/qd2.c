/*Program which estimates the integral of f(x,y) over [0,1]x[0,1]
MPI-2 father code -Quad2D
To compile: mpicc mpi2_qd2_father.c -o mpi2_qd2_father -lm -O3
To run: mpirun -np 1 .mpi2_qd2_father #proc_child
To change the input size, change the lines #define nx and ny*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define	nx 256
#define	ny 256

double f ( double x, double y ){
	double value;
	value = 1.0 / ( 1.0 - x * y );
	return value;
}

int main ( int argc, char *argv[] ){
	double a, b, error, exact, pi, total, wtime, x, y;
	int i, j, n;
	a = 0.0;
	b = 1.0;
	n = nx * ny;
	pi = 3.141592653589793;
	exact = pi * pi / 6.0;

	total = 0.0;
	for ( i = 1; i <= nx; i++ ){
		x = ( ( 2 * nx - 2 * i + 1 ) * a + ( 2 * i - 1 ) * b ) / ( 2 * nx );
		for ( j = 1; j <= ny; j++ ){
			y = ( ( 2 * ny - 2 * j + 1 ) * a + ( 2 * j - 1 ) * b ) / ( 2 * ny );
			total = total + f ( x, y );
		}
	}

	total = (b-a)*(b-a)*total/(double)(nx)/(double)(ny);
	error = fabs(total-exact);

	printf("Estimate = %24.10f\n", total);
	printf("Error    = %e\n", error);
	
	return 0;
}
