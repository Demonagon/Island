#ifndef TREE__H
#define TREE__H
//#include "update.h"
//#include "grid.h"
#include "util_math.h"
#include "list.h"

struct GameObject;

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
	Complex position;
	unsigned int memory_index;

	/*---*/

	TreeState state;
} Tree;

Tree * treeCreate(Complex position);

void treeDestroy(Tree * tree);

void treeSetupRoutine(struct GameObject * tree, unsigned int index);

void treeMemoryIndexUpdater(struct GameObject * tree, unsigned int index);

void mainTreeStressTest(int stress_level);

#endif
