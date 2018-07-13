#include <stdio.h>
#include "util_math.h"
#include "update.h"
#include "island.h"
#include "list.h"
#include "grid.h"
#include "main_memory.h"

void plot() {
	#define PLOT_SIZE 80

	printf("Plot :\n");

	double values[PLOT_SIZE];
	for(int k = 0; k < PLOT_SIZE; k++)
		values[k] = decelerationInterpolation( k / (double) PLOT_SIZE );

	for(int y = PLOT_SIZE - 1; y >= 0; y--) {
		for(int x = 0; x < PLOT_SIZE; x++) {
			if(values[x] > y / (double) PLOT_SIZE)
				printf("█");
			else printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char **argv) {

	//plot();
	//updateTest();
	//island(argc , argv);
	//mainListTest();
	//mainEventGridTest();

	return 0;
}
