#include <stdio.h>
#include "plant.h"
#include "global.h"
#include "object.h"
#include "main_memory.h"

void plantStatePrint(PlantState state) {
	switch(state) {
		case PLANT_INITIAL : 		printf("PLANT_INITIAL"); 		break;
		case PLANT_CANCELLED : 		printf("PLANT_CANCELLED"); 		break;
		case PLANT_GROWING : 		printf("PLANT_GROWING"); 		break;
		case PLANT_MATURE : 		printf("PLANT_MATURE"); 		break;
		case PLANT_DEAD : 			printf("PLANT_DEAD"); 			break;
	}
}

Plant * plantCreate(Complex position, double maximum_size) {
	GameObjectData data;

	data.plant = (Plant) {
		.position = position,
		.state = PLANT_INITIAL,
		.growth_stage = 0,
		.maximum_size = maximum_size,
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
	plant->state = PLANT_DEAD;
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
		plant->maximum_size * PLANT_RADIUS_CONSTANT * rand_double_a_b(1.8, 2.5);
	Complex son_relative_position =
		complexCreatePolar(random_angle, random_distance);
	plantCreate( complexAdd(plant->position, son_relative_position),
				 plantEvolveSize(plant->maximum_size) );
}

void plantPrint(Plant * plant) {
	complexPrint(plant->position);
	printf("[%8p] -> ", (void *) plant->memory_link);
	plantStatePrint(plant->state);
	printf(" (size = %4lf)", plant->maximum_size);
}

double plantSize(Plant * plant) {
	return ( (plant->growth_stage + 1.0) / PLANT_GROWTH_STAGES_COUNT );
}

double plantConcurrentArea(Plant * plant1, Plant * plant2) {
	double distance = complexDistance(plant1->position, plant2->position);
	return approximateCircleCommonArea(
			plantSize(plant1) * PLANT_RADIUS_CONSTANT,
			plantSize(plant2) * PLANT_RADIUS_CONSTANT,
			distance);
}

/** Updating callbacks **/
void plantUpdateDeclaration(void * data) {
	Plant * plant = data;
	GridEventData plant_data;

	if(plant->state == PLANT_INITIAL || plant->state == PLANT_CANCELLED ) { 
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
					plant->maximum_size * PLANT_TRUNK_SIZE_CONSTANT
				)
			);
	}
}

void plantUpdateApplication(void * data) {
	Plant * plant = data;
	int sleep_time = 0;

	/* TODO
	* Besoin de redesigner le système de sommeil puisqu'ici le croissance de la
	* plante est influencée par l'apparition et la disparition de plantes à
	* proximité...
	* Voir la croissance comme continue risque d'être trop complexe à simuler.
	* Solution à envisager : diviser la croissance en X segments, à chaque fois
	* redéclarant et recalculant le partage local des ressources.
	* Selont les ressources disponibles localement, la plante va décider selon
	* le processus suivant :
	* Si les ressources courantes remplissent la partie "survie" avec un
	* ratio de 0 <= R <= 1, alors la plante a une probabilité 1-R de mourir
	* instantanément. Dans l'autre cas, elle se contente de se conserver dans
	* son état jusqu'à la prochaine mise à jour.
	* Si les ressources courantes remplissent la partie "buffer" avec un ratio
	* 0 < R < 1, alors la plante se conserve.
	* Si les ressources courantes remplissent la partie "croisssance" avec un
	* ratio 0 <= R <= 1, alors la plante à une probabilité R de croître. Sinon
	* elle se conserve jusqu'à la prochaine étape. Si la plante est déjà à sa
	* taile maximale, elle se reproduit. Cette reproduction n'arrive qu'une
	* seule fois dans la vie de la plante.
	*/

	switch(plant->state) {
		case PLANT_INITIAL :
			plant->state = PLANT_GROWING;
			sleep_time = rand_int_a_b(2, 8);
			eventGridDeclareBeacon(&EVENT_GRID, &plant->grid_beacon,
				plant->maximum_size * PLANT_RADIUS_CONSTANT * 3, PLANT);
			break;
		case PLANT_CANCELLED :
			gameObjectUpdateGraphics(plant->memory_link);
			plantDestroy(plant);
			return;
		case PLANT_GROWING :
			/* calculer, puis vérifier les jauges de resources.
			* suivre la loi décidée au dessus. Une fois que la reproduction
			* arrive, on passe à l'était mature. */

			plant->state = PLANT_MATURE;
			break;

		case PLANT_MATURE :
			/* calculer, puis vérifier les jauges de resources.
			* suivre la loi décidée au dessus. */



			plant->state = PLANT_DEAD;
			break;
			
		case PLANT_DEAD :
			plantDestroy(plant);
			return;
	}

	gameObjectUpdateGraphics(plant->memory_link);

	updateRegisterAdd(&UPDATE_REGISTER,
					  &plant->update_handle, sleep_time);
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
