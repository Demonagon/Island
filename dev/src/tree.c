#include <stdio.h>
#include "tree.h"
#include "global.h"
#include "object.h"
#include "main_memory.h"

Tree * treeCreate(Complex position) {
	GameObjectData data;
	data.tree =
		(Tree) {.position = position, .memory_index = 0, .state = INITIAL };
	GameObject tree_object = gameObjectCreate(TREE, data);

	MemoryIndex index = mainMemoryAddObject(&MAIN_MEMORY, tree_object,
		treeSetupRoutine, treeMemoryIndexUpdater);

	GameObject * final_tree = mainMemoryAccessObject(&MAIN_MEMORY, index);

	return & final_tree->data.tree;
}

void treeDestroy(Tree * tree) {
	mainMemoryRemoveObject(&MAIN_MEMORY, tree->memory_index);
}

void treeSetupRoutine(GameObject * tree, MemoryIndex index) {
	printf("Je suis un joli arbre avec l'adresse %d !\n", index);
	tree->data.tree.memory_index = index;
	
}

void treeMemoryIndexUpdater(GameObject * tree, MemoryIndex index) {
	printf("Moi, l'arbre d'indice %d, passe à l'indice %d\n",tree->data.tree.memory_index, index);
	tree->data.tree.memory_index = index;
}

void mainTreeStressTest(int stress_level) {
	// Pour gérer le problème de ne pas pouvoir suivre un objet en mémoire
	// lorsqu'il est balancé à droite et à gauche par la main memory, ajouter
	// une collection de pointeurs sur fonction pour résoudre le soucis
	//Tree * forest[stress_level];
	for(int k = 0; k < stress_level; k++)
		treeCreate( complexCreate(0, 0) );

	for(int k = 0; k < stress_level; k++)
		mainMemoryRemoveObject(&MAIN_MEMORY, 0);
}
