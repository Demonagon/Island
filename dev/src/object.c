#include "object.h"

GameObject gameObjectCreate(GameObjectType type, GameObjectData data) {
	return (GameObject) {.type = type, .data = data };
}
