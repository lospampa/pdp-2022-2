/*Sequential Code - Monte Carlo Simulation
To compile: gcc seq_mc.c -o seq_mc -O3 -lm
To run: ./seq_mc
To change the input size, change the lines "#define N and iterations"*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define N 2048
#define iterations 100
#define thresh 0.50

int seed = 123456789;

int i4_max ( int i1, int i2 ){
  	int value;
  	if ( i2 < i1 ){
		  value = i1;
  	}else{
  		  value = i2;
  	}
  	return value;
}

int i4_min ( int i1, int i2 ){
  	int value;
  	if ( i1 < i2 ){
  		  value = i1;
  	}else{
  		  value = i2;
  	}
	return value;
}

int i4_modp ( int i, int j ){
 	int value;
  	if ( j == 0 ){
  		fprintf ( stderr, "\n" );
		fprintf ( stderr, "I4_MODP - Fatal error!\n" );
		fprintf ( stderr, "  I4_MODP ( I, J ) called with J = %d\n", j );
		exit ( 1 );
  	}
  	value = i % j;
  	if ( value < 0 ){
    		value = value + abs ( j );
  	}
  	return value;
}

int i4_wrap ( int ival, int ilo, int ihi ){
  	int jhi, jlo, value, wide;
  	jlo = i4_min ( ilo, ihi );
  	jhi = i4_max ( ilo, ihi );
  	wide = jhi + 1 - jlo;
  	if ( wide == 1 ){
    		value = jlo;
  	}else{
    		value = jlo + i4_modp ( ival - jlo, wide );
  	}
  	return value;
}

void ising_2d_agree (int *c5, int *c1){
	int i, im, ip, j, jm, jp;
	for ( i = 0; i < N; i++ ){
    	ip = i4_wrap ( i + 1, 0, N - 1 );
    	im = i4_wrap ( i - 1, 0, N - 1 );
    	for ( j = 0; j < N; j++ ){
      		jp = i4_wrap ( j + 1, 0, N - 1 );
       		jm = i4_wrap ( j - 1, 0, N - 1 );
       		c5[(i*N)+j] = c1[(i*N)+j] + c1[(ip*N)+j] + c1[(im*N)+j] + c1[(i*N)+jm] + c1[(i*N)+jp];
       		if ( 0 < c1[(i*N)+j] ){
       			c5[(i*N)+j] = ( 5 + c5[(i*N)+j] ) / 2;
       		}
       		else{
       			c5[(i*N)+j] = ( 5 - c5[(i*N)+j] ) / 2;
       		}
    	}
	}
}

void r8mat_uniform_01 (double *r){ //Função não paralelizável!
    int i, j, k;
    for ( i = 0; i < N; i++ ){
        for ( j = 0; j < N; j++ ){
            k = seed / 127773;
            seed = 16807 * ( seed - k * 127773 ) - k * 2836;
            if ( seed < 0 ){
                seed = seed + 2147483647;
            }
            r[(i*N)+j] = (float) seed * 4.656612875E-10;
        }
    }
}

void check(int *c1){
    int i, j, pos_count, neg_count;
    double pos_percent, neg_percent;
    printf ( "  Step     Positives       Negatives\n" );
    printf ( "             #    %%          #    %%\n" );
    printf ( "\n" );
    pos_count = 0;
    for ( j = 0; j < N; j++ ){
        for ( i = 0; i < N; i++ ){
            if ( 0 < c1[i+j*N] ){
                 pos_count = pos_count + 1;
            }
        }
    }
    neg_count = (N * N) - pos_count;
    pos_percent = ( double ) ( 100 * pos_count ) / ( double ) ( N * N );
    neg_percent = ( double ) ( 100 * neg_count ) / ( double ) ( N * N );
    printf ( "  %4d  %6d  %6.2f  %6d  %6.2f\n", iterations, pos_count, pos_percent, neg_count, neg_percent );
}

void transition(int *c1, int *c5, double *r){
    int i, j, step;
    double prob[5] = { 0.98, 0.85, 0.50, 0.15, 0.02 }; 
    for(step = 1; step <= iterations; step++){
        ising_2d_agree(c5, c1);
        r8mat_uniform_01(r);
		for ( i = 0; i < N; i++ ){
            for ( j = 0; j < N; j++ ){
                if ( r[(i*N)+j] < prob[c5[(i*N)+j]-1] ){
                    c1[(i*N)+j] = - c1[(i*N)+j];
                }
            }
        }
    }
    check(c1);
}

void initialize(int *c1){
	int i, j;
    double *r;
    r = ( double * ) malloc ( N * N * sizeof ( double ) );
    r8mat_uniform_01 (r );
    for ( j = 0; j < N; j++ ){
        for ( i = 0; i < N; i++ ){
            if ( r[i+j*N] <= thresh ){
                c1[i+j*N] = -1;
            }else{
                c1[i+j*N] = +1;
            }
        }
    }
    free(r);		
}

int main ( int argc, char *argv[] ){

	int *c1, *c5;
	double *r;
	//Inicio Alocacao
	r =  malloc(N*N*sizeof(double));
	c1 = malloc(N*N*sizeof(int));
	c5 = malloc(N*N*sizeof(int));
	//Fim Alocacao

	initialize(c1);
	
	transition(c1, c5, r); //kernel da aplicacao

		
	free(c1);
	free(c5);
	free(r);

	return 0;

}
