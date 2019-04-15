#ifndef MAIN_MEMORY__H
#define MAIN_MEMORY__H

/*
* Refait par Pacôme le 27 Juillet 2018.
* Précédemment la mémoire principale était prise en charge par un tableau.
* Cependant cette méthode ne garanti pas la conservation de l'emplacement
* en mémoire, ce qui demandait de trop complexifier le code. 
*/

#include "core/object.h"
#include "core/object_list.h"
#include "util/list.h"
#include "core/graphics.h"

/**
* Lorsque une itération est en cours, on veut prévenir toute modification
* à la mémoire afin d'éviter que des objets soient retirés pendant qu'ils
* sont mis à jour.
* Pour cela, lorsqu'une itération commence, la mémoire passe en mode "différé",
* où toute modification de sa liste d'objet est mise de coté dans les
* listes removal_list et addition_list. Entre chaque itération, la mémoire
* passe rapidement par l'état "direct", au début duquel la mémoire retire
* tout les objets présents dans removal_list et rajoute tout les objets
* présents addition_list (lesquelles listes sont ensuite vidée).
*/

/*typedef enum MemoryAccessMode {
	MEMORY_DIRECT_ACCESS_MODE,
	MEMORY_DIFFERED_ACCESS_MODE
} MemoryAccessMode;*/

typedef struct MainMemory {
	GameObjectList object_list;
	unsigned int size;

	/*MemoryAccessMode access_mode;
	List removal_list;
	GameObjectList addition_list;*/

	GraphicFactory graphic_factory; // <- est-ce que cet objet doit être ici ?
	void * graphic_data; // <- de même ?
} MainMemory;

MainMemory mainMemoryCreate(void * graphic_data, GraphicFactory factory);
void mainMemoryClear(MainMemory * memory);
GameObjectListLink * mainMemoryAddObject(MainMemory * memory,
										 GameObject object);
void mainMemoryRemoveObject(MainMemory * memory, GameObjectListLink * link);

/*void mainMemorySetDirectAccessMode(MainMemory * memory);
void mainMemorySetDifferedAccessMode(MainMemory * memory);*/

#endif
