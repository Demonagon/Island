#ifndef TEST_GRAPHICS__H
#define TEST_GRAPHICS__H
#include "graphics.h"
#include "tree.h"
#include "test.h"
#include "object_list.h"

typedef char (*StatePrinter) (int);
typedef void (*ObjectGraphicalUpdater) (GameObjectListLink *);

typedef struct TestGraphicsMatrix {
	int size;
	StatePrinter state_printer;
	ObjectGraphicalUpdater graphical_updater;

	int width;
	int height;
	double real_width;
	double real_height;

	int *** entity_count;
} TestGraphicsMatrix;

TestGraphicsMatrix testMatrixCreate(int size, StatePrinter state_printer,
									ObjectGraphicalUpdater graphical_updater,
									int width, int height,
									double real_width, double real_height);

void testGraphicsPrint(TestGraphicsMatrix * matrix);

GraphicalObject testGraphicsFactory (void * graphical_data, GameObject * object);

/** Printing trees **/
char testGraphicsTreeStatePrint(int state);
void testGraphicsTreeUpdate(GameObjectListLink * link);

/** Printing test objects **/
char testGraphicsTestStatePrint(int state);
void testGraphicsTestUpdate(GameObjectListLink * link);

#endif
