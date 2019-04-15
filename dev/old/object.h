#ifndef OBJECT__H
#define OBJECT__H

#include "core/graphics.h"

struct GameObjectListLink;

/* TODO */
/** -- INCLUDE OBJECTS CODE FILES HERE -- **/
#include "build/plant.h"
/** -- *-* *-* *-* *-* *-* *-* *-* *-* -- **/

typedef enum GameObjectType {
	PLANT
} GameObjectType;

// TODO Important : très mauvaise gestion de la mémoire... Envisager un design
// entity, où les objets ne contiennent pas d'information mais seulement des
// pointeurs et index vers les données qui les intéresse
typedef union GameObjectData {
	Plant plant;
} GameObjectData;

typedef struct GameObject {
	GameObjectType type;
	GameObjectData data;
	GraphicalObject graphics;
} GameObject;

GameObject gameObjectCreate(GameObjectType type,
							GameObjectData data);

void gameObjectUpdateGraphics(struct GameObjectListLink * object_link);

#endif
