#include <stdio.h>
#include "tree.h"
#include "global.h"
#include "object.h"
#include "main_memory.h"

void treeStatePrint(TreeState state) {
	switch(state) {
		case INITIAL : 		printf("INITIAL"); 		break;
		case CANCELLED : 	printf("CANCELLED"); 	break;
		case GROWING : 		printf("GROWING"); 		break;
		case REPRODUCTION : printf("REPRODUCTION"); break;
		case MATURE : 		printf("MATURE"); 		break;
		case DYING : 		printf("DYING"); 		break;
		case DEAD : 		printf("DEAD"); 		break;
	}
}

Tree * treeCreate(Complex position) {
	GameObjectData data;

	data.tree = (Tree) {
		.position = position,
		.state = INITIAL,
		.memory_index = 0,
		.update_handle = updateHandleCreateEmpty(),
		.grid_beacon = gridBeaconCreateEmpty()
	};

	data.tree.position = position;
	data.tree.state = INITIAL;
	data.tree.memory_index = 0;

	GameObject tree_object = gameObjectCreate(TREE, data);

	MemoryIndex index = mainMemoryAddObject(&MAIN_MEMORY, tree_object,
		treeSetupRoutine, treeMemoryIndexUpdater);

	GameObject * final_tree = mainMemoryAccessObject(&MAIN_MEMORY, index);

	return & final_tree->data.tree;
}

void treeDestroy(Tree * tree) {
	printf("destruction d'arbre d'index %d\n", tree->memory_index);
	gridBeaconRemove(&tree->grid_beacon);
	updateHandleRemove(&tree->update_handle);
	mainMemoryRemoveObject(&MAIN_MEMORY, tree->memory_index);
}

void treePrint(Tree * tree) {
	complexPrint(tree->position);
	printf("[%4d] -> ", tree->memory_index);
	treeStatePrint(tree->state);
}

void treeReproduce(Tree * tree) {
	double random_angle = randomAngle();
	Complex son_relative_position =
		complexCreatePolar(random_angle, TREE_SPAWN_RADIUS);
	treeCreate( complexAdd(tree->position, son_relative_position) );
}

void treeUpdateDeclaration(void * data) {
	Tree * tree = data;
	GridEventData tree_data;

	switch(tree->state) {
		case INITIAL :
			tree_data.spawning_tree = tree;
			eventGridBroadcast(
				&EVENT_GRID,
				gridEventCreate(
					TREE_SPAWNING_COLISION_CHECK_EVENT,
					tree_data,
					tree->position,
					TREE_SPAWN_RADIUS + 0.1
				)
			);
			break;
		default : break;
	}
}

void treeUpdateApplication(void * data) {
	Tree * tree = data;
	int sleep_time = 0;

	switch(tree->state) {
		case INITIAL :
			tree->state = GROWING;
			sleep_time = rand_int_a_b(2, 8);
			break;
		case CANCELLED :
			//treePrint(tree);
			//printf("\n");
			treeDestroy(tree);
			return;
		case GROWING :
			tree->state = REPRODUCTION;
			break;
		case REPRODUCTION :
			treeReproduce(tree);
			treeReproduce(tree);
			tree->state = MATURE;
			break;
		case MATURE :
			if( flipCoin() )
				tree->state = DYING;
			sleep_time = 5;
			break;
		case DYING :
			tree->state = DEAD;
			break;
		case DEAD :
			//treePrint(tree);
			//printf("\n");
			treeDestroy(tree);
			return;
	}

	//treePrint(tree);
	//printf("\n");

	updateRegisterAdd(&UPDATE_REGISTER,
					  &tree->update_handle, sleep_time);
}

void treeHandleEvent(void * data, GridEvent event) {
	Tree * tree = data;

	if( tree->state != INITIAL ) return;
	if( event.type != TREE_SPAWNING_COLISION_CHECK_EVENT ) return;

	Tree * concurrent_tree = event.data.spawning_tree;

	// The tree with maximum memory index gets to live
	if( tree->memory_index < concurrent_tree->memory_index )
		tree->state = CANCELLED;
}

void treeSetupRoutine(GameObject * tree, MemoryIndex index) {
	tree->data.tree.memory_index = index;
	printf("creation d'arbre d'index %d\n", index);

	updateHandleInit(
		&tree->data.tree.update_handle,
		&tree->data.tree,
		treeUpdateDeclaration,
		treeUpdateApplication);

	updateRegisterAdd(&UPDATE_REGISTER,
					  &tree->data.tree.update_handle, 0);

	gridBeaconInit(
		&tree->data.tree.grid_beacon,
		&tree->data.tree,
		&tree->data.tree.position,
		treeHandleEvent);

	eventGridPlaceBeacon(&EVENT_GRID, &tree->data.tree.grid_beacon);

}

void treeMemoryIndexUpdater(GameObject * tree, MemoryIndex index) {
	tree->data.tree.memory_index = index;

	updateHandleUpdateMemoryLocation(
		&tree->data.tree.update_handle,
		&tree->data.tree
	);

	gridBeaconUpdateMemoryLocation(
		&tree->data.tree.grid_beacon,
		&tree->data.tree,
		&tree->data.tree.position
	);
}

#include "string.h"

void mainTreeLifeCycleTest() {
	globalInit();

	treeCreate( complexCreate(EVENT_GRID_WIDTH / 2, EVENT_GRID_HEIGHT / 2) );

	char input;
	for(;;) {
		printf("[%5ld] :\n", UPDATE_REGISTER.clock);
		updateRegisterUpdate(&UPDATE_REGISTER);
		input = getc(stdin);
		if(input == 'x') break;
		if(input == '\n') continue;
		else while(getc(stdin) != '\n');
	}

	globalFree();
}

void mainTreeStressTest(int stress_level) {
	// Pour gérer le problème de ne pas pouvoir suivre un objet en mémoire
	// lorsqu'il est balancé à droite et à gauche par la main memory, ajouter
	// une collection de pointeurs sur fonction pour résoudre le soucis
	//Tree * forest[stress_level];
	for(int k = 0; k < stress_level; k++)
		treeCreate( complexCreate(0, 0) );

	for(int k = 0; k < stress_level; k++)
		mainMemoryRemoveObject(&MAIN_MEMORY, 0);
}
