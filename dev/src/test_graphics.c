#include "test_graphics.h"
#include <stdlib.h>
#include <stdio.h>

TestGraphicsMatrix testMatrixCreate(int width, int height,
									double real_width, double real_height) {
	TestGraphicsMatrix matrix = (TestGraphicsMatrix) {.entity_count = 0,
		.width = width, .height = height, .real_width = real_width,
		.real_height = real_height};

	matrix.entity_count = malloc( width * sizeof( int ** ) );
	for(int x = 0; x < width; x++) {
		matrix.entity_count[x] = malloc( height * sizeof( int * ) );
		for(int y = 0; y < height; y++) {
			matrix.entity_count[x][y] = malloc( DEAD * sizeof( int ) );
			for(int k = 0; k < DEAD; k++)
				matrix.entity_count[x][y][k] = 0;
		}
	}

	return matrix;
}

char testGraphicsGetStateChar(TreeState state) {
	switch(state) {
		case INITIAL : 		return 'i';
		case CANCELLED :	return 'x';
		case GROWING :		return 'g';
		case REPRODUCTION : return 'R';
		case MATURE :		return 'M';
		case DYING : 		return 'd';
		case DEAD :			return 'X';
		default : return '?';
	}
}

void testGraphicsPrint(TestGraphicsMatrix * matrix) {
	printf("╔");
	for(int x = 0; x < matrix->width; x++)
		printf("═");
	printf("╗\n");
	for(int y = matrix->height - 1; y >= 0; y--) {
		printf("║");
		for(int x = 0; x < matrix->width; x++) {
			for(int k = DYING - 1; k >= INITIAL; k--) {
				if( matrix->entity_count[x][y][k] ) {
					printf("%c", testGraphicsGetStateChar(k) );
					break;
				}
				if(k == INITIAL) printf(" ");
			}
		}
		printf("║\n");
	}
	printf("╚");
	for(int x = 0; x < matrix->width; x++)
		printf("═");
	printf("╝\n");
}

void testGraphicsTreeUpdate (GameObjectListLink * tree_link) {
	TestGraphicsMatrix * matrix = tree_link->object.graphics.graphical_data;

	if( ! matrix ) return;

	Tree * tree = (Tree *) &tree_link->object.data;
	int tree_x = (tree->position.a / matrix->real_width) * matrix->width;
	int tree_y = (tree->position.b / matrix->real_height) * matrix->height;

	if( tree_x < 0 || tree_y < 0 ||
		tree_x >= matrix->width || tree_y >= matrix->height )
		return;

	int * array = matrix->entity_count[tree_x][tree_y];

	if(tree->state == INITIAL)
		array[INITIAL]++;
	else if(tree->state == CANCELLED )
		array[INITIAL]--;
	else if( tree->state == GROWING ) {
		array[INITIAL]--;
		array[GROWING]++;
	}
	else if(tree->state == DEAD)
		array[DYING]--;
	else {
		array[tree->state - 1]--;
		array[tree->state]++;
	}

	/*printf("\n");
	printf("state = %d\n", tree->state);
	for(int k = 0; k < DEAD; k++) printf("%d", array[k]);
	printf("\n");*/
}

GraphicalObject testGraphicsFactory (void * graphical_data, GameObject * object) {

	if( ! graphical_data ) return graphicalObjectCreate(0, 0);
	if( object->type != TREE ) return graphicalObjectCreate(0, 0);

	return graphicalObjectCreate(testGraphicsTreeUpdate, graphical_data);
}
