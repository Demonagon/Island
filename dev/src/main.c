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
#include "test_graphics_2.h"
#include "sorted_tree.h"

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

void mainGridTest() {
	/*TestGraphicsMatrix matrix = testMatrixCreate(
		2, testGraphicsTestStatePrint, testGraphicsTestUpdate,
		100, 50, EVENT_GRID_WIDTH, EVENT_GRID_HEIGHT);

	globalInit(&matrix, testGraphicsFactory);*/
	testGraphics2GlobalInit();

	//TestObject * object = NULL;

	int f = 1; // factor

	for(int x = 0; x < EVENT_GRID_WIDTH / f; x++)
		for(int y = 0; y < EVENT_GRID_HEIGHT / f; y++)
			/*if(x == EVENT_GRID_WIDTH / (2*f) + mx && y == EVENT_GRID_HEIGHT / (2*f) + my)
				object = testCreate( complexCreate(x*f + 0.5, y*f + 0.5) );
			else*/
				testCreate( complexCreate(x*f + 0.5, y*f + 0.5) );

	/*if( object )
		object->activating_flag = 1;*/

	updateRegisterUpdate(&UPDATE_REGISTER);
	printf("\n");
	//testGraphicsPrint(&matrix);
	printList(&TEST_GRAPHICS_2_LIST);

	globalFree();
}

void mainTreeLifeCycleMainTest() {
	/*TestGraphicsMatrix matrix = testMatrixCreate(
		TREE_DEAD, testGraphicsTreeStatePrint, testGraphicsTreeUpdate,
		100, 50, EVENT_GRID_WIDTH, EVENT_GRID_HEIGHT);

	globalInit(&matrix, testGraphicsFactory);*/
	testGraphics2GlobalInit();

	/*int f = 1; // factor
	for(int x = 0; x < EVENT_GRID_WIDTH / f; x++)
		for(int y = 0; y < EVENT_GRID_HEIGHT / f; y++)
				treeCreate( complexCreate(x*f + 0.5, y*f + 0.5) );*/
	treeCreate(
		complexCreate(
			EVENT_GRID_WIDTH / 2,
			EVENT_GRID_HEIGHT / 2
		)
	);

	//for(int k; k < 40; k++) {
	for(;;) {

		if(UPDATE_REGISTER.clock == 10)
			birdCreate(
				complexCreate(
					EVENT_GRID_WIDTH / 2,
					EVENT_GRID_HEIGHT / 2
				)
			);

		printf("\033[s");

		//testGraphicsPrint(&matrix);
		printList(&TEST_GRAPHICS_2_LIST);
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
	//mainGridTest();
	//mainTreeMemoryTest();
	//testListManipulationMain();
	/*randomInit();
	for(;;)
		sortedTreeMainTest();*/
	
	/*globalInit();
	if(argc<2)printf("t'as oublié le paramètre coco\n");
	else mainTreeStressTest(atoi(argv[1]));
	globalFree();*/

	//fluidMatrixMainTest();

	return 0;
}
