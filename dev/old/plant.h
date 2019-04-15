#ifndef PLANT__H
#define PLANT__H
#include "util/util_math.h"
#include "util/list.h"
#include "core/update.h"
#include "core/grid.h"

#define PLANT_RADIUS_CONSTANT 1.0
#define PLANT_ROOT_START_CONSTANT 1.0
#define PLANT_ROOT_END_CONSTANT 1.2
#define PLANT_ROOT_DEPTH_CONSTANT (PLANT_ROOT_END_CONSTANT - PLANT_ROOT_START_CONSTANT)
#define PLANT_TRUNK_SIZE_CONSTANT 0.1

#define PLANT_MIN_SIZE 1.0
#define PLANT_MAX_SIZE 10.0

#define PLANT_LIFE_REQUIREMENT_CONSTANT 0.3
#define PLANT_REPRODUCTION_REQUIREMENT_CONSTANT 0.6

/**
* Une plante, qui se propage, avec un facteur génétique qui décrit sa taille.
* La taille de la plante influence la taille de son tronc, la taille de ses
* racines et la profondeur de ses racines.
* Les racines servent à récolter les ressources nécessaire à la conservation
* et la croissance de la plante jusqu'à sa taille maximale, d'où la plante
* se propage en lançant des graines de façon similaire à tree.
* Une plante ne mourra pas de vieillesse. La seule façon pour une plante de
* disparaître est d'être privée de ressources par sa competition locale.
*/

struct GameObjectListLink;

typedef enum PlantState {
	PLANT_INITIAL,
	PLANT_CANCELLED, // quand on pousse sur un tronc
	PLANT_GROWING, // état où le calcul des racines doit être opéré
	PLANT_MATURE, // état de vie stable
	PLANT_DEAD // état quand la plante est morte et doit être retirée
} PlantState;

void plantStatePrint(PlantState state);

typedef struct Plant {
	/** plant related data **/
	Complex position;
	PlantState state;
	double size;
	double current_ressources;

	/** Memory related data **/
	struct GameObjectListLink * memory_link;

	/** Updating related data **/
	UpdateHandle update_handle;

	/** Event related data **/
	GridBeacon grid_beacon;
} Plant;

Plant * plantCreate(Complex position, double size);

void plantDestroy(Plant * plant);

double plantEvolveSize(double initial_size);
void plantReproduce(Plant * plant);

void plantPrint(Plant * plant);

double plantSize(Plant * plant);

double plantMinimalRessourceLifeRequirement(Plant * plant);

double plantMinimalRessourceReproductionRequirement(Plant * plant);

double plantConcurrentArea(Plant * plant1, Plant * plant2);

/** Updating callbacks **/
void plantUpdateDeclaration(void * data);
void plantUpdateApplication(void * data);

/** Event managing callback **/
void plantHandleEvent(void * data, GridEvent event);

/** Memory setup callback **/
void plantSetupRoutine(struct GameObjectListLink * link);

/** Test initialisation functions **/
/*void mainplantMemoryTest();
void mainplantLifeCycleTest();
void mainplantStressTest(int stress_level);*/

#endif
