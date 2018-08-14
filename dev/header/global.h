#ifndef GLOBAL__H
#define GLOBAL__H

#include "main_memory.h"
MainMemory MAIN_MEMORY;

#include "grid.h"
#define EVENT_GRID_WIDTH  100
#define EVENT_GRID_HEIGHT 100
EventGrid EVENT_GRID;

#include "update.h"
UpdateRegister UPDATE_REGISTER;

void globalInit(void * graphic_data, GraphicFactory factory);
void globalFree();

#endif
