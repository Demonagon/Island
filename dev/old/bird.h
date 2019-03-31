#ifndef BIRD__H
#define BIRD__H
#include "update.h"
#include "grid.h"
#include "tree.h"
#include "mind_node.h"

/**
* Un oiseau, entité qui a la capacité de se déplacer, et qui a besoin de
* ressources pour se reproduire.
* Version test : l'oiseau se nourris d'arbres matures qu'il dévore goulument.
* il n'a pas vraiment besoin de le faire mais il le fait quand même.
* DEPRECATED
*/

typedef enum BirdStateID {
	BIRD_STATE_INITIAL,
	BIRD_STATE_STATIONNARY,
	BIRD_STATE_MOVING
} BirdStateID;

typedef union BirdStateData {
	Tree * target_tree; // BIRD_STATE_MOVING
} BirdStateData;

typedef struct BirdState {
	BirdStateID id;
	BirdStateData data;
} BirdState;

BirdState birdStateCreate();

typedef struct BirdAI {
	MindNode tree_eating_action_node;
} BirdAI;

BirdAI birdAICreate();

typedef struct Bird {
	/** internal data **/
	Complex position;
	BirdState state;
	BirdAI ai;

	/** Memory related data **/
	struct GameObjectListLink * memory_link;

	/** Updating related data **/
	UpdateHandle update_handle;

	/** Event related data **/
	GridBeacon grid_beacon;
} Bird;

typedef enum BirdActionType {
	BIRD_ACTION_MOVE,
	BIRD_ACTION_EAT
} BirdActionType;

Bird * birdCreate(Complex position);

void birdDestroy(Bird * bird);

void birdAddTraget(Bird * bird, Tree * tree);

/** Updating callbacks **/
void birdUpdateDeclaration(void * data);
void birdUpdateApplication(void * data);

/** Event managing callback **/
void birdHandleEvent(void * data, GridEvent event);

/** Memory setup callback **/
void birdSetupRoutine(struct GameObjectListLink * link);

/** Test initialisation functions **/
void birdTest();

#endif
