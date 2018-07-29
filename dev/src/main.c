#include <stdio.h>
#include <stdlib.h>
#include "util_math.h"
#include "global.h"
#include "update.h"
#include "island.h"
#include "heightmap.h"
#include "list.h"
#include "grid.h"
#include "main_memory.h"
#include "fluid.h"
#include "test.h"
#include "test_graphics.h"

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

void mainTreeLifeCycleMainTest() {
	TestGraphicsMatrix matrix = testMatrixCreate(100, 50, EVENT_GRID_WIDTH, EVENT_GRID_HEIGHT);

	globalInit(&matrix, testGraphicsFactory);

	Tree * tree = treeCreate(
		complexCreate(
			EVENT_GRID_WIDTH / 2,
			EVENT_GRID_HEIGHT / 2
		)
	);

	//for(int k; k < 40; k++) {
	for(;;) {

		printf("\033[s");

		testGraphicsPrint(&matrix);
		//printf("[%5ld] :\n", UPDATE_REGISTER.clock);

		//printf("tree state = %d\n", tree->state);
		//*
		char input;
		input = getc(stdin);
		if(input == 'x') break;
		if(input != '\n') while(getc(stdin) != '\n');
		//*/

		printf("\033[u");

		updateRegisterUpdate(&UPDATE_REGISTER);
	}

	globalFree();
}

int main(int argc, char **argv) {

	//plot();
	//updateTest();
	//island(argc , argv);
	//mainListTest();
	//mainEventGridTest();
	mainTreeLifeCycleMainTest();
	//mainTreeMemoryTest();
	//testListManipulationMain();
	
	/*globalInit();
	if(argc<2)printf("t'as oublié le paramètre coco\n");
	else mainTreeStressTest(atoi(argv[1]));
	globalFree();*/

	//fluidMatrixMainTest();

	return 0;
}
