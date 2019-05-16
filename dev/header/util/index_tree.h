#ifndef INDEX_TREE__H
#define INDEX_TREE__H
#include "util/array.h"

/** Créé par Pacôme le 16 mai 2019
* Une pile triée selont un critère donné. Version modifiée de sorted_tree.h
* qui sert à contenir des index.
* 
*/

typedef struct IndexTreeNode {
	// META DATA
	ArrayIndex parent_index;
	ArrayIndex left_child_index;
	ArrayIndex right_child_index;

	// DATA
	ArrayIndex object_index;
} IndexTreeNode;

IndexTreeNode index_tree_node_create_empty();
IndexTreeNode index_tree_node_create(ArrayIndex object_index);
ArrayIndex index_tree_node_allocate(Array * nodes_array, ArrayIndex object_index);
ArrayIndex index_tree_add_node(Array * nodes_array,
						 ArrayIndex root,
						 ArrayIndex node);
char index_tree_contains(Array * nodes_array,
						 ArrayIndex root,
						 ArrayIndex object_idex);

// Renvoie le noeux qui remplace le noeud enlevé s'il y a lieu, -1 sinon
ArrayIndex index_tree_detach(Array * nodes_array, ArrayIndex node);

/*typedef struct SortedTree {
	TreeNode * root;

	TreeNode * min_node;
	TreeNode * max_node;

	TreeDataEvaluator evaluator;
	void * data;

	int size;
} SortedTree;

SortedTree sortedTreeCreate(TreeDataEvaluator evaluator, void * data);
TreeNode * sortedTreeAddData(SortedTree * tree, void * data);
void sortedTreeAddNode(SortedTree * tree, TreeNode * node);
TreeNode * sortedTreeSearchData(SortedTree * tree, void * data);
void sortedTreeRemoveNode(SortedTree * tree, TreeNode * node);
void sortedTreeUpdateNode(SortedTree * tree, TreeNode * node);

void sortedTreeRemoveMin(SortedTree * tree);
TreeNode * sortedTreeGetMax(SortedTree * tree);

typedef void (*TreeNodeApplication) (TreeNode *);
typedef void (*TreeNodeParametrisedApplication) (TreeNode *, void *);

void sortedTreeApplyAll(SortedTree * tree, TreeNodeApplication application);
void sortedTreeParameterApplyAll(SortedTree * tree,
	TreeNodeParametrisedApplication application, void * parameter);
void sortedTreeDestroy(SortedTree * tree);


void sortedTreeMainTest(void);*/
#endif
