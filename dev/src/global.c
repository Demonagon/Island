#include "global.h"
#include "graphics.h"

MainMemory MAIN_MEMORY;

EventGrid EVENT_GRID;

UpdateRegister UPDATE_REGISTER;

GraphicalObject defaultGraphicFactory(void * data, GameObject * object) {
	return graphicalObjectCreate(0, 0);
}

void globalInit(void * graphic_data, GraphicFactory factory) {
	MAIN_MEMORY = mainMemoryCreate(graphic_data, factory);
	eventGridInit(&EVENT_GRID, EVENT_GRID_WIDTH, EVENT_GRID_HEIGHT);
	updateRegisterInit(&UPDATE_REGISTER);

	randomInit();
	//randomInitSeed(1535457078);
}

void globalFree() {
	mainMemoryClear(&MAIN_MEMORY);
}
