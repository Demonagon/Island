#ifndef MIND_MEMORY__H
#define MIND_MEMORY__H
#include "sorted_tree.h"

/** Créé par Pacôme le 5 août 2018
* La mémoire de l'esprit comporte une liste de souvenirs organisés selont un
* ordre total. La mémoire peut supprimer les n souvenirs les moins importants.
* Des callbacks d'ajout et de retrait de souvenirs sont disponibles.
*/

typedef void (*MemoryCallback) (void * memory);

typedef struct MindMemory {
	SortedTree memory_tree;

	MemoryCallback on_new_memory_callback;
	MemoryCallback on_deleted_memory_callback;
} MindMemory;

#endif
