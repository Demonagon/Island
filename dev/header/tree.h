#ifndef TREE__H
#define TREE__H
#include "update.h"
#include "grid.h"
//#include "main_memory.h"

typedef enum TreeState {
	INITIAL,
	CANCELLED,
	GROWING,
	REPRODUCTION,
	MATURE,
	DYING,
	DEAD
} TreeState;

typedef struct Tree {
	UpdateHandle update_handle;
	GridBeacon grid_beacon;

	/*---*/

	TreeState state;
} Tree;

#endif
