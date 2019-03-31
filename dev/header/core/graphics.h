#ifndef GRAPHICS__H
#define GRAPHICS__H

struct GameObject;
struct GameObjectListLink;

typedef void (*GraphicUpdateCallback) (struct GameObjectListLink *);

typedef struct GraphicalObject {
	GraphicUpdateCallback update_call_back;
	void * graphical_data;
} GraphicalObject;

GraphicalObject graphicalObjectCreate(
				GraphicUpdateCallback call_back,
				void * data);

typedef GraphicalObject (*GraphicFactory) (void *, struct GameObject *);

#endif
