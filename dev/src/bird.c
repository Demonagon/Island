#include <stdlib.h>
#include <stdio.h>
#include "bird.h"
#include "global.h"
#include "object.h"
#include "main_memory.h"

BirdState birdStateCreate() {
	BirdStateData data;
	data.target_tree = NULL;
	return (BirdState) {.id = BIRD_STATE_INITIAL, .data = data};
}

BirdAI birdAICreate() {
	return (BirdAI) {
		.tree_eating_action_node = MindNodeCreateEmpty()
	};
}

Bird * birdCreate(Complex position) {
	GameObjectData data;

	data.bird = (Bird) {
		.position = position,
		.state = birdStateCreate(),
		.ai = birdAICreate(),
		.memory_link = NULL,
		.update_handle = updateHandleCreateEmpty(),
		.grid_beacon = gridBeaconCreateEmpty()
	};

	GameObject bird_object = gameObjectCreate(BIRD, data);

	GameObjectListLink * link = mainMemoryAddObject(&MAIN_MEMORY, bird_object);

	birdSetupRoutine(link);

	return &link->object.data.bird;
}

void birdDestroy(Bird * bird) {
	mindNodeDestroy(&bird->ai.tree_eating_action_node);
	gridBeaconRemove(&bird->grid_beacon);
	updateHandleRemove(&bird->update_handle);
	mainMemoryRemoveObject(&MAIN_MEMORY, bird->memory_link);
}

void birdAddTraget(Bird * bird, Tree * tree) {
	mindNodeAddIngredient(&bird->ai.tree_eating_action_node, tree);
}

/** Updating callbacks **/
void birdUpdateDeclaration(void * data) {
	Bird * bird = data;

	switch(bird->state.id) {
		case BIRD_STATE_INITIAL :
			bird->state.id = BIRD_STATE_STATIONNARY;
			bird->state.data.target_tree = NULL;
			eventGridDeclareBeacon(&EVENT_GRID, &bird->grid_beacon, 100, BIRD);
			return;
		case BIRD_STATE_STATIONNARY :
			return;
		case BIRD_STATE_MOVING :
			return;
	}
}

void birdUpdateApplication(void * data) {
	Bird * bird = data;
	Tree * target = NULL;
	
	switch(bird->state.id) {
		case BIRD_STATE_INITIAL :
			return;
		case BIRD_STATE_STATIONNARY :
			return;
		case BIRD_STATE_MOVING :
			target = bird->state.data.target_tree;
			bird->position = target->position;
			eventGridPlaceBeacon(&EVENT_GRID, &bird->grid_beacon);
			target->state = TREE_DEAD;
			bird->state.id = BIRD_STATE_STATIONNARY;
			bird->state.data.target_tree = NULL;
			MindNodeDeleteIngredient(& bird->ai.tree_eating_action_node, target);
			eventGridDeclareBeacon(&EVENT_GRID, &bird->grid_beacon, 100, BIRD);
			gameObjectUpdateGraphics(bird->memory_link);
			return;
	}
}

/** Event managing callback **/
void birdHandleEvent(void * data, GridEvent event) {
	if( event.type != OBJECT_SELF_DECLARATION_EVENT ) return;
	if( event.data.declarating_object.type != TREE ) return;
	Bird * bird = data;
	Tree * tree = event.data.declarating_object.data;

	birdAddTraget(bird, tree);
}

/** For bird AI initialisation **/
void birdAINewSolutionOutput(
	void * data,
	void * solution,
	int solution_value) {
	Bird * bird = data;
	Tree * target = solution;

	if( bird->state.id == BIRD_STATE_MOVING ) {
		Tree * old_target = bird->state.data.target_tree;
		target = complexDistance( bird->position, target->position ) <
				 complexDistance( bird->position, old_target->position ) ?
				target : old_target;
	}
	else {
		bird->state.id = BIRD_STATE_MOVING;
		bird->state.data.target_tree = target;

		updateRegisterAdd(&UPDATE_REGISTER, &bird->update_handle, 3);
	}
}

/** For bird AI initialisation **/
void birdAIDeletedSolutionOutput(
	void * data,
	void * solution,
	int solution_value) {
	Bird * bird = data;
	Tree * target = solution;

	if( bird->state.id != BIRD_STATE_MOVING ) return;
	if( bird->state.data.target_tree != target ) return;

	bird->state.id = BIRD_STATE_STATIONNARY;
	bird->state.data.target_tree = NULL;
	eventGridDeclareBeacon(&EVENT_GRID, &bird->grid_beacon, 100, BIRD);
}

void birdIngredientProcessorFunction(struct MindNode * node, void * ingredient) {
	mindNodeAddProduct(node, ingredient);
}

char birdIngredientCheckFunction(void * product, void * ingredient) {
	return (product == ingredient);
}

int birdDataEvaluator(void * context, void * data) {
	Bird * bird = context;
	Tree * tree = data;
	return (int) ( complexDistance(bird->position, tree->position) * -10 );
}

/** Memory setup callback **/
void birdSetupRoutine(GameObjectListLink * link) {
	Bird * bird = &link->object.data.bird;
	bird->memory_link = link;

	mindNodeInit(
		&bird->ai.tree_eating_action_node, 
		mindNodeParentOutputCreate(bird,
			birdAINewSolutionOutput,
			birdAIDeletedSolutionOutput),
		mindNodeIngredientProcessorCreate(
			birdIngredientProcessorFunction,
			birdIngredientCheckFunction
		),
		3,
		birdDataEvaluator,
		bird);

	updateHandleInit(
		&bird->update_handle,
		 bird,
		birdUpdateDeclaration,
		birdUpdateApplication);

	updateRegisterAdd(&UPDATE_REGISTER, &bird->update_handle, 0);

	gridBeaconInit(
		&bird->grid_beacon,
		 bird,
		&bird->position,
		birdHandleEvent
	);

	eventGridPlaceBeacon(&EVENT_GRID, &bird->grid_beacon);
	
	gameObjectUpdateGraphics(link);
}

#include <stdio.h>
#include "test_graphics_2.h"

/** Test initialisation functions **/
void birdTest() {
	testGraphics2GlobalInit();

	treeCreate( complexCreate(EVENT_GRID_WIDTH / 2, EVENT_GRID_HEIGHT / 2) );
	birdCreate( complexCreate(EVENT_GRID_WIDTH / 3, EVENT_GRID_HEIGHT / 3) );

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
