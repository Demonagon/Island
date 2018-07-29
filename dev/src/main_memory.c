#include "main_memory.h"

MainMemory mainMemoryCreate(void * graphic_data, GraphicFactory factory) {
	return (MainMemory) {
		.object_list = objectListCreate(),
		.size = 0,
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

	objectListLinkDestroy(link);
	memory->size--;
}
