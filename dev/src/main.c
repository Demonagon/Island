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
	globalInit();

	treeCreate( complexCreate(EVENT_GRID_WIDTH / 2, EVENT_GRID_HEIGHT / 2) );

	//char input;
	for(int k; k < 40; k++) {
	//for(;;) {
		printf("[%5ld] :\n", UPDATE_REGISTER.clock);
		updateRegisterUpdate(&UPDATE_REGISTER);

		GameObject * first_object = mainMemoryAccessObject(&MAIN_MEMORY, 0);
		Tree * first_tree = &first_object->data.tree;

		printf("Original :\n");
		treePrint(first_tree);
		printf("\n");

		/*
		input = getc(stdin);
		if(input == 'x') break;
		if(input == '\n') continue;
		else while(getc(stdin) != '\n');
		//*/
	}

	globalFree();
}

int main(int argc, char **argv) {

	//plot();
	//updateTest();
	//island(argc , argv);
	//mainListTest();
	//mainEventGridTest();
	//mainTreeLifeCycleMainTest();
	//mainTreeMemoryTest();
	testListManipulationMain();
	
	/*globalInit();
	if(argc<2)printf("t'as oublié le paramètre coco\n");
	else mainTreeStressTest(atoi(argv[1]));
	globalFree();*/

	//fluidMatrixMainTest();

	return 0;
}
