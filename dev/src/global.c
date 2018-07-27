#include "global.h"
#include "graphics.h"

MainMemory MAIN_MEMORY;

EventGrid EVENT_GRID;

UpdateRegister UPDATE_REGISTER;

GraphicalObject defaultGraphicFactory(GameObject * object) {
	return graphicalObjectCreate(0, 0);
}

void globalInit() {
	MAIN_MEMORY = mainMemoryCreate(defaultGraphicFactory);
	eventGridInit(&EVENT_GRID, EVENT_GRID_WIDTH, EVENT_GRID_HEIGHT);
	updateRegisterInit(&UPDATE_REGISTER);

	randomInit();
	//randomInitSeed(1532496614);
}

void globalFree() {
	mainMemoryClear(&MAIN_MEMORY);
}
