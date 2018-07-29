#ifndef TEST_GRAPHICS__H
#define TEST_GRAPHICS__H
#include "graphics.h"
#include "tree.h"
#include "object_list.h"

typedef struct TestGraphicsMatrix {
	int *** entity_count;
	int width;
	int height;
	double real_width;
	double real_height;
} TestGraphicsMatrix;

TestGraphicsMatrix testMatrixCreate(int width, int height,
									double real_width, double real_height);

char testGraphicsGetStateChar(TreeState state);

void testGraphicsPrint(TestGraphicsMatrix * matrix);

void testGraphicsTreeUpdate (GameObjectListLink * tree_link);

GraphicalObject testGraphicsFactory (void * graphical_data, GameObject * object);

#endif
