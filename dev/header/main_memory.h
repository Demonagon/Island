#ifndef MAIN_MEMORY__H
#define MAIN_MEMORY__H

/*
* Refait par Pacôme le 27 Juillet 2018.
* Précédemment la mémoire principale était prise en charge par un tableau.
* Cependant cette méthode ne garanti pas la conservation de l'emplacement
* en mémoire, ce qui demandait de trop complexifier le code. 
*/

#include "object.h"
#include "object_list.h"
#include "graphics.h"

typedef struct MainMemory {
	GameObjectList object_list;
	unsigned int size;

	GraphicFactory graphic_factory;
	void * graphic_data;
} MainMemory;

MainMemory mainMemoryCreate(void * graphic_data, GraphicFactory factory);
void mainMemoryClear(MainMemory * memory);
GameObjectListLink * mainMemoryAddObject(MainMemory * memory,
										 GameObject object);
void mainMemoryRemoveObject(MainMemory * memory, GameObjectListLink * link);

#endif
