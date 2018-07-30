#ifndef TEST__H
#define TEST__H
#include "list.h"
#include "update.h"
#include "grid.h"
#include "util_math.h"

struct GameObjectListLink;

typedef enum TestObjectState {
	TEST_OBJECT_OFF,
	TEST_OBJECT_ON
} TestObjectState;

typedef struct TestObject {
	/** Test related data **/
	Complex position;
	TestObjectState state;
	int activating_flag;

	/** Memory related data **/
	struct GameObjectListLink * memory_link;

	/** Updating related data **/
	UpdateHandle update_handle;

	/** Event related data **/
	GridBeacon grid_beacon;
} TestObject;

TestObject * testCreate(Complex position);

void testDestroy(TestObject * object);

/** Updating callbacks **/
void testUpdateDeclaration(void * data);
void testUpdateApplication(void * data);

/** Event managing callback **/
void testHandleEvent(void * data, GridEvent event);

/** Memory setup callback **/
void testSetupRoutine(struct GameObjectListLink * link);

/** Test initialisation functions **/
void testListManipulationMain();

#endif
