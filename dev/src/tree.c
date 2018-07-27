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
		.memory_link = NULL,
		.update_handle = updateHandleCreateEmpty(),
		.grid_beacon = gridBeaconCreateEmpty()
	};

	GameObject tree_object = gameObjectCreate(TREE, data);

	GameObjectListLink * link = mainMemoryAddObject(&MAIN_MEMORY, tree_object);

	treeSetupRoutine(link);

	return &link->object.data.tree;
}
 
void treeDestroy(Tree * tree) {
	//printf("Destruction d'arbre d'index %d\n", tree->memory_index);
	gridBeaconRemove(&tree->grid_beacon);
	updateHandleRemove(&tree->update_handle);
	mainMemoryRemoveObject(&MAIN_MEMORY, tree->memory_link);
}

void treePrint(Tree * tree) {
	complexPrint(tree->position);
	printf("[%8p] -> ", (void *) tree->memory_link);
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
			if( ! eventGridIsPointIn(&EVENT_GRID, tree->position) ) {
				tree->state = CANCELLED;
				return;
			}
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

	treePrint(tree);
	printf("\n");

	switch(tree->state) {
		case INITIAL :
			tree->state = GROWING;
			sleep_time = rand_int_a_b(2, 8);
			break;
		case CANCELLED :
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
			/*if(tree->memory_index == 0)
				printf("updating\n");
			if( flipCoin() )*/
				tree->state = DYING;
			sleep_time = 5;
			break;
		case DYING :
			tree->state = DEAD;
			break;
		case DEAD :
			treeDestroy(tree);
			return;
	}

	updateRegisterAdd(&UPDATE_REGISTER,
					  &tree->update_handle, sleep_time);
}

void treeHandleEvent(void * data, GridEvent event) {
	Tree * tree = data;

	if( tree->state != INITIAL ) return;
	if( event.type != TREE_SPAWNING_COLISION_CHECK_EVENT ) return;

	Tree * concurrent_tree = event.data.spawning_tree;

	if( concurrent_tree->state == CANCELLED ) return;

	// The tree with maximum memory index gets to live
	if( tree->memory_link < concurrent_tree->memory_link )
		tree->state = CANCELLED;
}

void treeSetupRoutine(GameObjectListLink * link) {
	Tree * tree = &link->object.data.tree;
	tree->memory_link = link;

	updateHandleInit(
		&tree->update_handle,
		 tree,
		treeUpdateDeclaration,
		treeUpdateApplication);

	updateRegisterAdd(&UPDATE_REGISTER, &tree->update_handle, 0);

	gridBeaconInit(
		&tree->grid_beacon,
		 tree,
		&tree->position,
		treeHandleEvent
	);

	eventGridPlaceBeacon(&EVENT_GRID, &tree->grid_beacon);

}

#include "string.h"

void mainTreeMemoryTest() {
	globalInit();

	for(int k = 0; k < 98; k++)
		treeCreate( complexCreate(0, 0) );

	//Tree * tree98 = treeCreate( complexCreate(0, 0) );

	for(int k = 0; k < 4; k++)
		treeCreate( complexCreate(0, 0) );

	//Tree * tree103 = treeCreate( complexCreate(0, 0) );

	for(int k = 0; k < 100; k++)
		treeCreate( complexCreate(0, 0) );

	//printf("%d : %p, %d : %p\n", tree98->memory_index, tree98, tree103->memory_index, tree103);

	globalFree();
	
}

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
