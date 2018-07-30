#include "test.h"
#include <stdio.h>
#include "global.h"
#include "object.h"
#include "main_memory.h"

TestObject * testCreate(Complex position) {
	GameObjectData data;

	data.test_object = (TestObject) {
		.position = position,
		.state = TEST_OBJECT_OFF,
		.activating_flag = rand_int_a_b(1, 1000) == 1 ? 1 : 0,
		.memory_link = NULL,
		.update_handle = updateHandleCreateEmpty(),
		.grid_beacon = gridBeaconCreateEmpty()
	};

	GameObject test_object = gameObjectCreate(TEST_OBJECT, data);

	GameObjectListLink * link = mainMemoryAddObject(&MAIN_MEMORY, test_object);

	testSetupRoutine(link);

	return & link->object.data.test_object;
}

void testDestroy(TestObject * object) {
	updateHandleRemove(&object->update_handle);
	mainMemoryRemoveObject(&MAIN_MEMORY, object->memory_link);
}

/** Updating callbacks **/
void testUpdateDeclaration(void * data) {
	TestObject * object = data;
	GridEventData object_data;

	if( ! object->activating_flag ) {
		/*if( object->state == TEST_OBJECT_ON ) {
			object->state = TEST_OBJECT_OFF; 
			gameObjectUpdateGraphics(object->memory_link);
		}*/
		return;
	}

	eventGridBroadcast(
		&EVENT_GRID,
		gridEventCreate(
			TEST_OBJECT_TURN_ON_EVENT,
			object_data,
			object->position,
			10
		)
	);

	object->activating_flag = 0;
}

void testUpdateApplication(void * data) {}

/** Event managing callback **/
void testHandleEvent(void * data, GridEvent event) {
	TestObject * object = data;

	if( event.type != TEST_OBJECT_TURN_ON_EVENT ) return;

	object->state = TEST_OBJECT_ON;
	gameObjectUpdateGraphics(object->memory_link);
}

/** Memory setup function **/
void testSetupRoutine(GameObjectListLink * link) {
	TestObject * test_object = &link->object.data.test_object;
	test_object->memory_link = link;

	updateHandleInit(
		&test_object->update_handle,
		 test_object,
		testUpdateDeclaration,
		testUpdateApplication);

	updateRegisterAdd(&UPDATE_REGISTER, &test_object->update_handle, 0);

	gridBeaconInit(
		&test_object->grid_beacon,
		 test_object,
		&test_object->position,
		testHandleEvent
	);

	eventGridPlaceBeacon(&EVENT_GRID, &test_object->grid_beacon);
	
	gameObjectUpdateGraphics(link);
}

/** Test initialisation functions **/
void testListManipulationMain() {
	globalInit();

	for(int k = 0; k < 10; k++)
		testCreate( complexCreate(0, 0) );

	char input;
	//for(int k; k < 40; k++) {
	for(;;) {
		printf("[%5ld] :\n", UPDATE_REGISTER.clock);
		updateRegisterUpdate(&UPDATE_REGISTER);

		/*GameObject * first_object = mainMemoryAccessObject(&MAIN_MEMORY, 0);
		test_object * first_test_object = &first_object->data.test_object;

		printf("Original :\n");
		test_objectPrint(first_test_object);
		printf("\n");*/


		input = getc(stdin);
		if(input == 'x') break;
		if(input == '\n') continue;
		else while(getc(stdin) != '\n');
	}

	globalFree();
}
