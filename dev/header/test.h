#ifndef TEST__H
#define TEST__H
#include "list.h"
#include "update.h"

struct GameObject;

typedef struct TestObject {
	/** Memory related data **/
	unsigned int memory_index;

	/** Updating related data **/
	UpdateHandle update_handle;
} TestObject;

TestObject * testCreate(void);

void testDestroy(TestObject * object);

/** Updating callbacks **/
void testUpdateDeclaration(void * data);
void testUpdateApplication(void * data);

/** Memory setup and index updating callbacks **/
void testSetupRoutine(struct GameObject * object, unsigned int index);
void testMemoryIndexUpdater(struct GameObject * object,
							struct GameObject * old_object,
							unsigned int index);

/** Test initialisation functions **/
void testListManipulationMain();

#endif
