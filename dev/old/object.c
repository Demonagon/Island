#include "core/object.h"
#include "core/object_list.h"

GameObject gameObjectCreate(GameObjectType type,
							GameObjectData data) {
	return (GameObject) {
		.type = type,
		.data = data,
		.graphics = graphicalObjectCreate(0, 0)
	};
}

void gameObjectUpdateGraphics(GameObjectListLink * object_link) {
	if( object_link->object.graphics.update_call_back )
		object_link->object.graphics.update_call_back(object_link);
}
