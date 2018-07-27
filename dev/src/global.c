#include "global.h"

MainMemory MAIN_MEMORY;

EventGrid EVENT_GRID;

UpdateRegister UPDATE_REGISTER;

void globalInit() {
	MAIN_MEMORY = mainMemoryCreate();
	eventGridInit(&EVENT_GRID, EVENT_GRID_WIDTH, EVENT_GRID_HEIGHT);
	updateRegisterInit(&UPDATE_REGISTER);

	randomInitSeed(1532496614);
}

void globalFree() {
	mainMemoryClear(&MAIN_MEMORY);
}
