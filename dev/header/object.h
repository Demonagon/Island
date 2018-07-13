#ifndef OBJECT__H
#define OBJECT__H

/* TODO */
/** -- INCLUDE OBJECTS CODE FILES HERE -- **/
#include "tree.h"
/** -- *-* *-* *-* *-* *-* *-* *-* *-* -- **/

typedef enum GameObjectType {
	TREE
} GameObjectType;

typedef union GameObjectData {
	Tree tree;
} GameObjectData;

typedef struct GameObject {
	GameObjectType type;
	GameObjectData data;
} GameObject;

GameObject gameObjectCreate(GameObjectType type, GameObjectData data);

#endif
