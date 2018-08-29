#include "main_memory.h"
#include <stdio.h>

MainMemory mainMemoryCreate(void * graphic_data, GraphicFactory factory) {
	return (MainMemory) {
		.object_list = objectListCreate(),
		.size = 0,
		/*.access_mode = MEMORY_DIRECT_ACCESS_MODE,
		.removal_list = listCreate(),
		.addition_list = objectListCreate(),*/
		.graphic_factory = factory,
		.graphic_data = graphic_data
	};
}
void mainMemoryClear(MainMemory * memory) {
	objectListClear(&memory->object_list);
	memory->size = 0;
}

GameObjectListLink * mainMemoryAddObject(MainMemory * memory,
										 GameObject object) {
	GameObjectListLink * link = objectListLinkAllocate(object);

	if(object.type == TEST_OBJECT)
		printf("test object ! \nA\nA\nA\nA\nA\nA\n");

	/*if( memory->access_mode == MEMORY_DIFFERED_ACCESS_MODE ) {
		objectListAdd( &memory->addition_list, link );
		return link;
	}*/

	objectListAdd( &memory->object_list, link );
	memory->size++;

	if( memory->graphic_factory )
		link->object.graphics = memory->graphic_factory(
			memory->graphic_data, 
			&link->object
		);

	return link;
}

void mainMemoryRemoveObject(MainMemory * memory, GameObjectListLink * link) {
	if( ! link ) return;

	/*if( memory->access_mode == MEMORY_DIFFERED_ACCESS_MODE ) {
		ListLink * temporary_link = malloc(sizeof(ListLink));
		(*temporary_link) = listLinkCreate(link);
		listAdd( &memory->removal_list, temporary_link );
		return;
	}*/

	objectListLinkDestroy(link);
	memory->size--;
}

/*void memoryAdditionApplication(void * data, void * parameter) {
	mainMemoryAddObject(
}

void mainMemorySetDirectAccessMode(MainMemory * memory) {
	memory->access_mode = MAIN_MEMORY_DIRECT_ACCESS_MODE;

	listParameterizedApplyAll(List list, ParameterizedListApplication application,
				  void * parameters);
}

void mainMemorySetDifferedAccessMode(MainMemory * memory) {
	
}*/
