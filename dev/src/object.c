#include "object.h"

GameObject gameObjectCreate(GameObjectType type,
							GameObjectData data) {
	return (GameObject) {
		.type = type,
		.data = data,
		.graphics = graphicalObjectCreate(0, 0)
	};
}
