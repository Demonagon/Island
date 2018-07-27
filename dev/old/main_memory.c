#include <stdlib.h>
#include <stdio.h>
#include "main_memory.h"

MainMemory mainMemoryCreate() {
	return (MainMemory) {
		.dynamical_array = NULL,
		.last_chunk_size = 0,
		.before_last_chunk_size = 0,
		.dynamical_array_size = 0
	};
}

void mainMemoryFree(MainMemory * memory) {
	for(int k = 0; k < memory->dynamical_array_size; k++)
		free(memory->dynamical_array[k]);
	free(memory->dynamical_array);
}

void mainMemoryAddArray(MainMemory * memory) {
	MemoryObject ** old_array = memory->dynamical_array;
	int old_size = memory->dynamical_array_size;

	memory->dynamical_array
		= malloc( sizeof(MemoryObject *) * (old_size + 1) );

	for(int k = 0; k < old_size; k++)
		memory->dynamical_array[k] = old_array[k];

	memory->dynamical_array[old_size]
		= malloc( sizeof(MemoryObject) * MAIN_MEMORY_CHUNK_SIZE );

	memory->dynamical_array_size++;
	memory->before_last_chunk_size = memory->last_chunk_size;
	memory->last_chunk_size = 0;

	free(old_array);
}

void mainMemoryRemoveArray(MainMemory * memory) {
	MemoryObject ** old_array = memory->dynamical_array;
	int old_size = memory->dynamical_array_size;

	free( memory->dynamical_array[old_size - 1] );

	memory->dynamical_array
		= malloc( sizeof(MemoryObject *) * (old_size - 1) );

	for(int k = 0; k < old_size - 1; k++)
		memory->dynamical_array[k] = old_array[k];

	memory->dynamical_array_size--;

	memory->last_chunk_size = memory->before_last_chunk_size;
	memory->last_chunk_size = 0;

	free(old_array);
}

unsigned int mainMemoryGetTotalSize(MainMemory * memory) {
	switch ( memory->dynamical_array_size ) {
		case 0 : return 0;
		case 1 : return memory->last_chunk_size;
		default :
			return (memory->dynamical_array_size - 2) * MAIN_MEMORY_CHUNK_SIZE
			+ memory->before_last_chunk_size + memory->last_chunk_size;
	}
}

unsigned int mainMemoryGetTotalCapacity(MainMemory * memory) {
	return memory->dynamical_array_size * MAIN_MEMORY_CHUNK_SIZE
		   - mainMemoryGetTotalSize(memory);
}

unsigned int mainMemoryGetIndexChunk(MemoryIndex index) {
	return index / MAIN_MEMORY_CHUNK_SIZE;
}

unsigned int mainMemoryGetInChunkIndex(MemoryIndex index) {
	return index % MAIN_MEMORY_CHUNK_SIZE;
}

MemoryIndex mainMemoryAddObject(MainMemory * memory, GameObject object,
								SetupRoutine setup_routine,
								IndexUpdater index_updater) {
	if( ! mainMemoryGetTotalCapacity(memory) )
		mainMemoryAddArray(memory);

	MemoryIndex new_index = mainMemoryGetTotalSize(memory);

	MemoryIndex chunk = 0;
	MemoryIndex chunk_index = 0;

	if(memory->dynamical_array_size == 1) {
		chunk = 0;
		chunk_index = memory->last_chunk_size;
	} else if( memory->before_last_chunk_size == MAIN_MEMORY_CHUNK_SIZE ) {
		chunk = memory->dynamical_array_size - 1;
		chunk_index = memory->last_chunk_size;
	} else {
		chunk = memory->dynamical_array_size - 2;
		chunk_index = memory->before_last_chunk_size;
	}

	MemoryObject memory_object =
		(MemoryObject) {.index_updater = index_updater, .object = object};

	memory->dynamical_array[chunk][chunk_index] = memory_object;

	if(chunk == memory->dynamical_array_size - 1)
		memory->last_chunk_size++;
	else if(chunk == memory->dynamical_array_size - 2)
		memory->before_last_chunk_size++;

	// Once the new object is placed, we call the setup routine

	if( setup_routine )
		setup_routine( & memory->dynamical_array[chunk][chunk_index].object,
						new_index );

	return new_index;
}

GameObject mainMemoryRemoveObject(MainMemory * memory, MemoryIndex index) {
	if( index >= mainMemoryGetTotalSize(memory) ) {
		fprintf(stderr, "(%s, line %d) => Memory received a out of bounds",
			__FILE__, __LINE__);
		fprintf(stderr, " memory index for removal (value = %d, size = %d)\n",
			index, mainMemoryGetTotalSize(memory));
		fprintf(stderr, "Are you sure the index you gave was properly ");
		fprintf(stderr, "maintained by the index updater ?\n");
		abort();
	}

	unsigned int chunk = mainMemoryGetIndexChunk(index);
	unsigned int chunk_index = mainMemoryGetInChunkIndex(index);

	// Object retrieval
	MemoryObject remove_object = memory->dynamical_array[chunk][chunk_index];

	// Taking the last objet in memory
	unsigned int last_object_index = mainMemoryGetTotalSize(memory) - 1;

	printf("%d <- %d\n", index, last_object_index);

	unsigned int last_chunk = mainMemoryGetIndexChunk(last_object_index);
	unsigned int last_chunk_index = mainMemoryGetInChunkIndex(last_object_index);

	// Filling empty room with last object
	memory->dynamical_array[chunk][chunk_index].index_updater =
		memory->dynamical_array[last_chunk][last_chunk_index].index_updater;
	memory->dynamical_array[chunk][chunk_index].object =
		memory->dynamical_array[last_chunk][last_chunk_index].object;

	// Notifying newly replaced objet of the change of it's index

	if( memory->dynamical_array[chunk][chunk_index].index_updater )
		memory->dynamical_array[chunk][chunk_index].index_updater(
			&memory->dynamical_array[chunk][chunk_index].object, 
			&remove_object.object,
			&memory->dynamical_array[last_chunk][last_chunk_index].object,
			index
		);

	// Applying memory size shrink
	if( memory->last_chunk_size )
		memory->last_chunk_size--;
	else
		memory->before_last_chunk_size--;

	// If there is enought room open, remove the last chunk
	if( memory->dynamical_array_size > 1 &&
		! memory->last_chunk_size &&
		! memory->before_last_chunk_size )
		mainMemoryRemoveArray(memory);

	return remove_object.object;
}

GameObject * mainMemoryAccessObject(MainMemory * memory, MemoryIndex index) {
	if( index >= mainMemoryGetTotalSize(memory) ) {
		fprintf(stderr, "(%s, line %d) => Memory received a out of bounds",
			__FILE__, __LINE__);
		fprintf(stderr, " memory index to access to (value = %d, size = %d)\n",
			index, mainMemoryGetTotalSize(memory));
		fprintf(stderr, "Are you sure the index you gave was properly ");
		fprintf(stderr, "maintained by the index updater ?\n");
		abort();
	}

	unsigned int chunk = mainMemoryGetIndexChunk(index);
	unsigned int chunk_index = mainMemoryGetInChunkIndex(index);

	return & memory->dynamical_array[chunk][chunk_index].object;
}

/*--------------------------- GAME OBJECT FUNCTIONS --------------------------*/

/*void standardGameObjectIndexUpdater(GameObject * object, MemoryIndex index) {
	object->memory_index = index;
}

void gameObjectCopyAndSetup(MainMemory * memory, GameObject object,
							SetupRoutine setup_routine) {
	mainMemoryAddObject(memory, object,
		standardGameObjectIndexUpdater, setup_routine);
}*/

