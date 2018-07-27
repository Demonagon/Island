#ifndef TEST__H
#define TEST__H
#include "list.h"
#include "update.h"

struct GameObjectListLink;

typedef struct TestObject {
	/** Memory related data **/
	struct GameObjectListLink * memory_link;

	/** Updating related data **/
	UpdateHandle update_handle;
} TestObject;

TestObject * testCreate(void);

void testDestroy(TestObject * object);

/** Updating callbacks **/
void testUpdateDeclaration(void * data);
void testUpdateApplication(void * data);

/** Memory setup and index updating callbacks **/
void testSetupRoutine(struct GameObjectListLink * link);

/** Test initialisation functions **/
void testListManipulationMain();

#endif
