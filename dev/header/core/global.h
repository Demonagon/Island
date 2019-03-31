#ifndef GLOBAL__H
#define GLOBAL__H

/**
* Contient toutes les gros objets globaux.
*/

#include "core/main_memory.h"
MainMemory MAIN_MEMORY;

#include "core/grid.h"
#define EVENT_GRID_WIDTH  100
#define EVENT_GRID_HEIGHT 100
EventGrid EVENT_GRID;

#include "core/update.h"
UpdateRegister UPDATE_REGISTER;

void globalInit(void * graphic_data, GraphicFactory factory);
void globalFree();

#endif
