#ifndef TEST_GRAPHICS_2__H
#define TEST_GRAPHICS_2__H
#include "graphics.h"
#include "list.h"
#include "object_list.h"

typedef struct TestGraphicsList {
	List graphic_list;
	int width;
	int height;
	double real_width;
	double real_height;
} TestGraphicsList;

TestGraphicsList TEST_GRAPHICS_2_LIST;

char getObjectCharacter(GameObject * link);
char getCharacterAtLocation(TestGraphicsList * list, int x, int y);
void printList(TestGraphicsList * list);

typedef struct TestGraphics2Object {
	TestGraphicsList * list;
	ListLink graphical_link;
} TestGraphics2Object;

void testGraphic2UpdateCallback(GameObjectListLink * object);
GraphicalObject testGraphics2Factory(void * data, GameObject * object);

void testGraphics2GlobalInit();

#endif
