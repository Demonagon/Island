#include <stdio.h>
#include "tree.h"
#include "global.h"
#include "object.h"
#include "main_memory.h"
#include "bird.h"

void treeStatePrint(TreeState state) {
	switch(state) {
		case TREE_INITIAL : 		printf("TREE_INITIAL"); 		break;
		case TREE_CANCELLED : 		printf("TREE_CANCELLED"); 		break;
		case TREE_GROWING : 		printf("TREE_GROWING"); 		break;
		case TREE_REPRODUCTION : 	printf("TREE_REPRODUCTION"); 	break;
		case TREE_MATURE : 			printf("TREE_MATURE"); 			break;
		case TREE_DYING : 			printf("TREE_DYING"); 			break;
		case TREE_DEAD : 			printf("TREE_DEAD"); 			break;
	}
}

Tree * treeCreate(Complex position) {
	GameObjectData data;

	data.tree = (Tree) {
		.position = position,
		.state = TREE_INITIAL,
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

	if(tree->state == TREE_INITIAL || tree->state == TREE_CANCELLED ) { 
			if( ! eventGridIsPointIn(&EVENT_GRID, tree->position) ) {
				//printf("out of bound check\n");
				tree->state = TREE_CANCELLED;
				return;
			}
			tree_data.spawning_tree = tree;
			//printf("(%p) launching event (%lf)\n", data, TREE_SPAWN_RADIUS * 0.9);
			eventGridBroadcast(
				&EVENT_GRID,
				gridEventCreate(
					TREE_SPAWNING_COLISION_CHECK_EVENT,
					tree_data,
					tree->position,
					TREE_SPAWN_RADIUS * 0.9
				)
			);
	}
}

void treeUpdateApplication(void * data) {
	Tree * tree = data;
	int sleep_time = 0;

	//treePrint(tree);
	//printf("\n");

	switch(tree->state) {
		case TREE_INITIAL :
			tree->state = TREE_GROWING;
			sleep_time = rand_int_a_b(2, 8);
			eventGridDeclareBeacon(&EVENT_GRID, &tree->grid_beacon, 100, TREE);
			break;
		case TREE_CANCELLED :
			gameObjectUpdateGraphics(tree->memory_link);
			treeDestroy(tree);
			return;
		case TREE_GROWING :
			tree->state = TREE_REPRODUCTION;
			break;
		case TREE_REPRODUCTION :
			treeReproduce(tree);
			treeReproduce(tree);
			treeReproduce(tree);
			tree->state = TREE_MATURE;
			sleep_time = 20;
			break;
		case TREE_MATURE :
			//* Immortality 
			//return;
			//*/
			tree->state = TREE_DYING;
			break;
		case TREE_DYING :
			tree->state = TREE_DEAD;
			gameObjectUpdateGraphics(tree->memory_link);
			break;
		case TREE_DEAD :
			treeDestroy(tree);
			return;
	}

	gameObjectUpdateGraphics(tree->memory_link);

	updateRegisterAdd(&UPDATE_REGISTER,
					  &tree->update_handle, sleep_time);
}

void treeHandleEvent(void * data, GridEvent event) {
	Tree * tree = data;

	//printf("catching event\n");

	if( event.type == OBJECT_SELF_DECLARATION_EVENT ) {
		if( event.data.declarating_object.type != BIRD ) return;
		Bird * bird = event.data.declarating_object.data;
		birdAddTraget(bird, tree);
		return;
	}

	if( event.type != TREE_SPAWNING_COLISION_CHECK_EVENT ) return;

	Tree * concurrent_tree = event.data.spawning_tree;

	if( tree == concurrent_tree ) return;

	//if( concurrent_tree->state == TREE_CANCELLED ) return;
	//if( tree->state == TREE_CANCELLED ) return;

	if( tree->state != TREE_INITIAL &&
		tree->state != TREE_CANCELLED )
		concurrent_tree->state = TREE_CANCELLED;
	else if( concurrent_tree->state != TREE_INITIAL &&
			 concurrent_tree->state != TREE_CANCELLED )
		return;
	else if( tree->memory_link > concurrent_tree->memory_link )
		concurrent_tree->state = TREE_CANCELLED;
	else tree->state = TREE_CANCELLED;
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
	
	gameObjectUpdateGraphics(link);
}

#include "string.h"
#include "test_graphics_2.h"

void mainTreeMemoryTest() {
	testGraphics2GlobalInit();

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
	testGraphics2GlobalInit();

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
