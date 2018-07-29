#ifndef OBJECT__H
#define OBJECT__H

#include "graphics.h"

struct GameObjectListLink;

/* TODO */
/** -- INCLUDE OBJECTS CODE FILES HERE -- **/
#include "test.h"
#include "tree.h"
/** -- *-* *-* *-* *-* *-* *-* *-* *-* -- **/

typedef enum GameObjectType {
	TEST_OBJECT,
	TREE
} GameObjectType;

typedef union GameObjectData {
	TestObject test_object;
	Tree tree;
} GameObjectData;

typedef struct GameObject {
	GameObjectType type;
	GameObjectData data;
	GraphicalObject graphics;
} GameObject;

GameObject gameObjectCreate(GameObjectType type,
							GameObjectData data);

void gameObjectUpdateGraphics(struct GameObjectListLink * object_link);

#endif
