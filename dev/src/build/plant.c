#include <stdio.h>
#include "build/plant.h"
#include "core/global.h"
#include "core/object.h"
#include "core/main_memory.h"

void plantStatePrint(PlantState state) {
	switch(state) {
		case PLANT_INITIAL : 		printf("PLANT_INITIAL"); 		break;
		case PLANT_CANCELLED : 		printf("PLANT_CANCELLED"); 		break;
		case PLANT_GROWING : 		printf("PLANT_GROWING"); 		break;
		case PLANT_MATURE : 		printf("PLANT_MATURE"); 		break;
		case PLANT_DEAD : 			printf("PLANT_DEAD"); 			break;
	}
}

Plant * plantCreate(Complex position, double size) {
	GameObjectData data;

	data.plant = (Plant) {
		.position = position,
		.state = PLANT_INITIAL,
		.size = size,
		.current_ressources = 0,
		.memory_link = NULL,
		.update_handle = updateHandleCreateEmpty(),
		.grid_beacon = gridBeaconCreateEmpty()
	};

	GameObject plant_object = gameObjectCreate(PLANT, data);

	GameObjectListLink * link = mainMemoryAddObject(&MAIN_MEMORY, plant_object);

	plantSetupRoutine(link);

	return &link->object.data.plant;
}

void plantDestroy(Plant * plant) {
	gameObjectUpdateGraphics(plant->memory_link);
	gridBeaconRemove(&plant->grid_beacon);
	updateHandleRemove(&plant->update_handle);
	mainMemoryRemoveObject(&MAIN_MEMORY, plant->memory_link);
}

double plantEvolveSize(double initial_size) {
	if(initial_size < PLANT_MIN_SIZE) initial_size = PLANT_MIN_SIZE;
	if(initial_size > PLANT_MAX_SIZE) initial_size = PLANT_MAX_SIZE;

	double new_size = initial_size * rand_double_a_b(0.8, 1.2);

	return new_size < PLANT_MIN_SIZE ? PLANT_MIN_SIZE :
		   new_size > PLANT_MAX_SIZE ? PLANT_MAX_SIZE : new_size;
}

void plantReproduce(Plant * plant) {
	double random_angle = randomAngle();
	double random_distance =
		plant->size * PLANT_RADIUS_CONSTANT * rand_double_a_b(1.8, 2.5);
	Complex son_relative_position =
		complexCreatePolar(random_angle, random_distance);
	plantCreate( complexAdd(plant->position, son_relative_position),
				 plantEvolveSize(plantSize(plant)) );
}

void plantPrint(Plant * plant) {
	complexPrint(plant->position);
	printf("[%8p] -> ", (void *) plant->memory_link);
	plantStatePrint(plant->state);
	printf(" (size = %4lf)", plant->size);
}

double plantSize(Plant * plant) {
	return plant->size;
}

double plantMinimalRessourceLifeRequirement(Plant * plant) {
	return plant->size * plant->size * plant->size
		 * PLANT_LIFE_REQUIREMENT_CONSTANT; 
}

double plantMinimalRessourceReproductionRequirement(Plant * plant) {
	return plant->size * plant->size * plant->size
		 * PLANT_REPRODUCTION_REQUIREMENT_CONSTANT; 
}

double plantConcurrentArea(Plant * plant1, Plant * plant2) {
	double distance = complexDistance(plant1->position, plant2->position);
	return approximateCircleCommonArea(
			plantSize(plant1) * PLANT_RADIUS_CONSTANT,
			plantSize(plant2) * PLANT_RADIUS_CONSTANT,
			distance);
}

void plantUpdateInitial(Plant * plant) {
	GridEventData plant_data;
	if( ! eventGridIsPointIn(&EVENT_GRID, plant->position) ) {
		// out of bound check
		plant->state = PLANT_CANCELLED;
		return;
	}

	plant_data.spawning_plant = plant;
	eventGridBroadcast(
		&EVENT_GRID,
		gridEventCreate(
			PLANT_SPAWNING_COLISION_CHECK_EVENT,
			plant_data,
			plant->position,
			plant->size * PLANT_TRUNK_SIZE_CONSTANT
		)
	);
}

/** Updating callbacks **/
void plantUpdateDeclaration(void * data) {
	Plant * plant = data;

	switch(plant->state) {
		case PLANT_INITIAL :
			// On vérifie si on pousse sur un tronc ou une autre pousse en
			// initial. Si il y a colision on vérifie les priorités. Si on
			// a la priorité, on passe en GROWING. Sinon on passe en CANCELED.

			break;
		case PLANT_CANCELLED :
			// On détruit la pousse.
			break;
		case PLANT_GROWING :
			// On grandit, en ajoutant aux voisins la diminution due au partage
			// des ressources (ainsi qu'à soit).
			// On vérifie intialiement les ressources disponibles : si pas
			// assez pour vivre on passe en DEAD, si assez pour vivre on passe
			// en MATURE.
			break;
		case PLANT_MATURE :
			// Cet état est le seul stable, et se met à jour uniquement lorsque
			// les ressources sont mises à jour. À chaque mise à jour des
			// ressources :
			// -> si on a pas assez pour vivre, on passe à DEAD
			// -> si on a assez pour se reproduire et que l'on se s'est jamais
			//    reproduit, on se reproduit
			break;
		case PLANT_DEAD :
			// On dégrandit avant de disparaître, ce qui veut dire faire l'effet
			// inverse à l'étape GROWING. Puis l'objet est enlevé.
			break;
	}
}

void plantUpdateApplication(void * data) {
	Plant * plant = data;

	switch(plant->state) {
		case PLANT_INITIAL :
			// Check la priorité, et mettre à jour l'état en fonction
			break;
		case PLANT_CANCELLED :
			plantDestroy(plant);
			return;

		case PLANT_GROWING :
			// Check la valeur qu'on obtient, et mettre à jour l'état en fonction
			break;

		case PLANT_MATURE :
			// Check la valeur qu'on obtient, et mettre à jour l'état en fonction
			break;
			
		case PLANT_DEAD :
			plantDestroy(plant);
			return;
	}

	gameObjectUpdateGraphics(plant->memory_link);

	//updateRegisterAdd(&UPDATE_REGISTER,
	//				  &plant->update_handle, sleep_time);
}

/** Event managing callback **/
void plantHandleEvent(void * data, GridEvent event) {
	Plant * plant = data;

	//printf("catching event\n");

	// Handle trunk colision
	if( event.type == PLANT_SPAWNING_COLISION_CHECK_EVENT ) {

		Plant * concurrent_plant = event.data.spawning_plant;

		if( plant == concurrent_plant ) return;

		if( plant->state != PLANT_INITIAL &&
			plant->state != PLANT_CANCELLED )
			concurrent_plant->state = PLANT_CANCELLED;
		else if( concurrent_plant->state != PLANT_INITIAL &&
				 concurrent_plant->state != PLANT_CANCELLED )
			return;
		else if( plant->memory_link > concurrent_plant->memory_link )
			concurrent_plant->state = PLANT_CANCELLED;
		else plant->state = PLANT_CANCELLED;

		return;
	}
}

/** Memory setup callback **/
void plantSetupRoutine(struct GameObjectListLink * link) {
	Plant * plant = &link->object.data.plant;
	plant->memory_link = link;

	updateHandleInit(
		&plant->update_handle,
		 plant,
		plantUpdateDeclaration,
		plantUpdateApplication);

	updateRegisterAdd(&UPDATE_REGISTER, &plant->update_handle, 0);

	gridBeaconInit(
		&plant->grid_beacon,
		 plant,
		&plant->position,
		plantHandleEvent
	);

	eventGridPlaceBeacon(&EVENT_GRID, &plant->grid_beacon);
	
	gameObjectUpdateGraphics(link);
}
