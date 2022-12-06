/*Sequential code - Calculation of the Number Pi
To compile: gcc seq_pi.c -o seq_pi -O3
To run: ./seq_pi
To change the input size, modify the line "#define NUM_POINTS ..."*/

#include <stdio.h>

#define NUM_POINTS 4000000000

int main(int argc, char **argv){
	long long int i;
	double pi=0.0;
	
	for(i=0;i<NUM_POINTS;i++){
		pi += 4.0/(4.0*(double)i+1.0);
		pi -= 4.0/(4.0*(double)i+3.0);
	}
	
	printf("%.10lf\n", pi);
	return 0;

}
