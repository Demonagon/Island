#ifndef MIND_MEMORY__H
#define MIND_MEMORY__H
#include "sorted_tree.h"

/** Créé par Pacôme le 5 août 2018
* La mémoire de l'esprit comporte une liste de souvenirs organisés selont un
* ordre total. La mémoire peut supprimer les n souvenirs les moins importants.
* Des callbacks d'ajout et de retrait de souvenirs sont disponibles.
*/

/** le premier paramètre est l'information globale, le second l'information
* locale du souvenir en question, le troisième pour l'évaluation du souvenir **/
typedef void (*MemoryTokenCallback) (void *, void *, int);
typedef TreeDataEvaluator MemoryTokenEvaluator;

typedef struct MindMemory {
	SortedTree memory_tree;

	int max_size;

	void * data;
	MemoryTokenCallback on_new_memory_callback;
	MemoryTokenCallback on_deleted_memory_callback;
} MindMemory;

MindMemory mindMemoryCreate(void * data, int max_size,
	MemoryTokenCallback on_new_memory_callback,
	MemoryTokenCallback on_deleted_memory_callback,
	MemoryTokenEvaluator memory_evaluator,
	void * context_data);

void mindMemoryDestroy(MindMemory * memory);

void mindMemoryForgetOne(MindMemory * memory);
void mindMemoryForget(MindMemory * memory, int to_forget);
void mindMemoryForgetHalf(MindMemory * memory);
void mindMemoryForgetUpToNumber(MindMemory * memory, int target_size);

void mindMemoryProcessToken(MindMemory * memory, void * token);
void mindMemoryRemoveToken(MindMemory * memory, void * token);
void mindMemoryUpdateToken(MindMemory * memory, void * token);
void mindMemoryRemoveNode(MindMemory * memory, TreeNode * node);

void * mindMemoryGetMaxToken(MindMemory * memory);
/* Return the number of tokens successfully recovered */
//int mindMemoryGetNMaxToken(MindMemory * memory, int n, void * result);

/* Implies to resort the entire sorted tree */
void mindMemoryChangeTokenEvaluator(MindMemory * memory, 
	MemoryTokenEvaluator evaluator);

#endif
