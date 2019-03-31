#include "test_graphics.h"
#include <stdlib.h>
#include <stdio.h>

TestGraphicsMatrix testMatrixCreate(int size, StatePrinter state_printer,
									ObjectGraphicalUpdater graphical_updater,
									int width, int height,
									double real_width, double real_height) {
	TestGraphicsMatrix matrix = (TestGraphicsMatrix) {
		.size = size, .state_printer = state_printer,
		.graphical_updater = graphical_updater,
		.width = width, .height = height, .real_width = real_width,
		.real_height = real_height,
		.entity_count = 0};

	matrix.entity_count = malloc( width * sizeof( int ** ) );
	for(int x = 0; x < width; x++) {
		matrix.entity_count[x] = malloc( height * sizeof( int * ) );
		for(int y = 0; y < height; y++) {
			matrix.entity_count[x][y] = malloc( size * sizeof( int ) );
			for(int k = 0; k < size; k++)
				matrix.entity_count[x][y][k] = 0;
		}
	}

	return matrix;
}

void testGraphicsPrint(TestGraphicsMatrix * matrix) {
	printf("╔");
	for(int x = 0; x < matrix->width; x++)
		printf("═");
	printf("╗\n");
	for(int y = matrix->height - 1; y >= 0; y--) {
		printf("║");
		for(int x = 0; x < matrix->width; x++) {
			for(int k = matrix->size - 1; k >= 0; k--) {
				if( matrix->entity_count[x][y][k] ) {
					printf("%c", matrix->state_printer(k) );
					break;
				}
				if(k == 0) printf(" ");
			}
		}
		printf("║\n");
	}
	printf("╚");
	for(int x = 0; x < matrix->width; x++)
		printf("═");
	printf("╝\n");
}

GraphicalObject testGraphicsFactory (void * graphical_data, GameObject * object) {
	if( ! graphical_data ) return graphicalObjectCreate(0, 0);

	TestGraphicsMatrix * matrix = graphical_data;

	return graphicalObjectCreate(matrix->graphical_updater, graphical_data);
}

/** ------------------ Printing trees ------------------ **/
char testGraphicsTreeStatePrint(int state) {
	switch(state) {
		case TREE_INITIAL : 		return 'i';
		case TREE_CANCELLED :		return 'x';
		case TREE_GROWING :			return 'g';
		case TREE_REPRODUCTION : 	return 'R';
		case TREE_MATURE :			return 'M';
		case TREE_DYING : 			return 'd';
		case TREE_DEAD :			return 'X';
		default : return '?';
	}
}

void testGraphicsTreeUpdate(GameObjectListLink * link) {
	TestGraphicsMatrix * matrix = link->object.graphics.graphical_data;

	if( ! matrix ) return;

	Tree * tree = (Tree *) &link->object.data;
	int tree_x = (tree->position.a / matrix->real_width) * matrix->width;
	int tree_y = (tree->position.b / matrix->real_height) * matrix->height;

	if( tree_x < 0 || tree_y < 0 ||
		tree_x >= matrix->width || tree_y >= matrix->height )
		return;

	int * array = matrix->entity_count[tree_x][tree_y];

	if(tree->state == TREE_INITIAL)
		array[TREE_INITIAL]++;
	else if(tree->state == TREE_CANCELLED )
		array[TREE_INITIAL]--;
	else if( tree->state == TREE_GROWING ) {
		array[TREE_INITIAL]--;
		array[TREE_GROWING]++;
	}
	else if(tree->state == TREE_DEAD)
		array[TREE_DYING]--;
	else {
		array[tree->state - 1]--;
		array[tree->state]++;
	}

	array[TREE_DYING] = 0;
}

/** ------------------ Printing test objects ------------------ **/
char testGraphicsTestStatePrint(int state) {
	switch(state) {
		case TEST_OBJECT_OFF : return '_';
		case TEST_OBJECT_ON  : return 'O';
		default : return '?';
	}
}

void testGraphicsTestUpdate(GameObjectListLink * link) {
	TestGraphicsMatrix * matrix = link->object.graphics.graphical_data;

	if( ! matrix ) return;

	TestObject * test = (TestObject *) &link->object.data;
	int test_x = (test->position.a / matrix->real_width) * matrix->width;
	int test_y = (test->position.b / matrix->real_height) * matrix->height;

	if( test_x < 0 || test_y < 0 ||
		test_x >= matrix->width || test_y >= matrix->height )
		return;

	int * array = matrix->entity_count[test_x][test_y];

	if( test->state == TEST_OBJECT_OFF ) {
		array[TEST_OBJECT_OFF]++;
		array[TEST_OBJECT_ON]--;
	}
	if( test->state == TEST_OBJECT_ON ) {
		array[TEST_OBJECT_OFF]--;
		array[TEST_OBJECT_ON]++;
	}

	if(array[TEST_OBJECT_OFF] < 0)
		array[TEST_OBJECT_OFF] = 0;
	if(array[TEST_OBJECT_ON] < 0)
		array[TEST_OBJECT_ON] = 0;

	//printf("%d\n", array[TEST_OBJECT_ON]);
}
