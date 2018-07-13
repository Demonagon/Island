#include <stdio.h>
#include "tree.h"
#include "global.h"
#include "object.h"
#include "main_memory.h"

Tree * treeCreate(Complex position) {
	GameObjectData data;
	data.tree =
		(Tree) {.position = position, .memory_index = 0, .state = INITIAL};
	GameObject tree_object = gameObjectCreate(TREE, data);

	printf("ref -> %p\n", & TreeMemoryIndexUpdater);

	MemoryIndex index = mainMemoryAddObject(&MAIN_MEMORY, tree_object,
		TreeSetupRoutine, TreeMemoryIndexUpdater);

	GameObject * final_tree = mainMemoryAccessObject(&MAIN_MEMORY, index);

	return & final_tree->data.tree;
}

void treeDestroy(Tree * tree) {
	mainMemoryRemoveObject(&MAIN_MEMORY, tree->memory_index);
}

void TreeSetupRoutine(GameObject * tree, MemoryIndex index) {
	tree->data.tree.memory_index = index;
}

void TreeMemoryIndexUpdater(GameObject * tree, MemoryIndex index) {
	printf("coucou");
	printf("Moi, l'arbre d'indice %d, passe à l'indice %d\n",tree->data.tree.memory_index, index);
	tree->data.tree.memory_index = index;
}

void mainTreeStressTest(int stress_level) {
	Tree * my_forest[ stress_level ];
	for(int k = 0; k < stress_level; k++)
		my_forest[k] = treeCreate( complexCreate(0, 0) );

	for(int k = 0; k < stress_level; k++)
		printf("Je suis un joli arbre avec l'adresse %d !\n",
				my_forest[k]->memory_index);

	for(int k = 0; k < stress_level; k++)
		treeDestroy( my_forest[k] );
}
