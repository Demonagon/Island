#ifndef SORTED_TREE__H
#define SORTED_TREE__H

/** Créé par Pacôme le 5 août 2018
* Une pile triée selont un critère donné.
*/

typedef struct TreeNode {
	void * data;
	int value;

	struct TreeNode * parent;
	struct TreeNode * left_child;
	struct TreeNode * right_child;
} TreeNode;

//first : context, second : data to evaluate
typedef int (*TreeDataEvaluator) (void *, void *);

TreeNode treeNodeCreate(void * data, TreeDataEvaluator evaluator, void * context_data);
void treeNodeAdd(TreeNode * root, TreeNode * node);
TreeNode * treeNodeSearch(TreeNode * root, void * data, int value);
// Renvoie le noeux qui remplace le noeud enlevé s'il y a lieu
TreeNode * treeNodeDetach(TreeNode * node);

typedef struct SortedTree {
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
typedef void (*TreeNodeParametrisedApplication) (void *, TreeNode *);

void sortedTreeApplyAll(SortedTree * tree, TreeNodeApplication application);
void sortedTreeParameterApplyAll(SortedTree * tree,
	TreeNodeParametrisedApplication application, void * parameter);
void sortedTreeDestroy(SortedTree * tree);


void sortedTreeMainTest(void);
#endif
