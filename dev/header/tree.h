#ifndef TREE__H
#define TREE__H
//#include "update.h"
//#include "grid.h"
#include "util_math.h"
#include "list.h"
#include "update.h"
#include "grid.h"

#define TREE_SPAWN_RADIUS 5

/**
* Un arbre. C'est un objet qui a la particularité de pouvoir se reproduire.
*/

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

void treeStatePrint(TreeState state);

typedef struct Tree {
	/** Tree related data **/
	Complex position;
	TreeState state;

	/** Memory related data **/
	unsigned int memory_index;

	/** Updating related data **/
	UpdateHandle update_handle;

	/** Event related data **/
	GridBeacon grid_beacon;
} Tree;

Tree * treeCreate(Complex position);

void treeDestroy(Tree * tree);

void treeReproduce(Tree * tree);

void treePrint(Tree * tree);

/** Updating callbacks **/
void treeUpdateDeclaration(void * data);
void treeUpdateApplication(void * data);

/** Event managing callback **/
void treeHandleEvent(void * data, GridEvent event);

/** Memory setup and index updating callbacks **/
void treeSetupRoutine(struct GameObject * tree, unsigned int index);
void treeMemoryIndexUpdater(struct GameObject * tree,
							struct GameObject * old_tree,
							unsigned int index);

/** Test initialisation functions **/
void mainTreeMemoryTest();
void mainTreeLifeCycleTest();
void mainTreeStressTest(int stress_level);

#endif
