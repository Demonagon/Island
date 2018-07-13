#ifndef OBJECT__H
#define OBJECT__H

//#include "main_memory.h"

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

	unsigned int memory_index;
} GameObject;

//GameObject gameObjectCreate(GameObjectType type, GameObjectData data);
//void gameObjectInit(GameObject * game_object);

#endif
