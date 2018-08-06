#include "sorted_tree.h"
#include <stdlib.h>

/** ------------------------------- TREE NODE ------------------------------ **/

TreeNode treeNodeCreate(void * data, TreeDataEvaluator evaluator) {
	TreeNode node = (TreeNode) {
		.data = data,
		.value = 0,
		.parent = NULL,
		.left_child = NULL,
		.right_child = NULL };

	node.value = evaluator(data);
	return node;
}

void treeNodeAdd(TreeNode * root, TreeNode * node) {
	if( ! root || ! node ) return;

	if( node->value <= root->value ) {
		if( ! root->left_child ) {
			root->left_child = node;
			node->parent = root;
		}
		else
			treeNodeAdd(root->left_child, node);
	}
	else {
		if( ! root->right_child ) {
			root->right_child = node;
			node->parent = root;
		}
		else
			treeNodeAdd(root->right_child, node);
	}
}

TreeNode * treeNodeSearch(TreeNode * root, void * data, int value) {
	if( ! root ) return NULL;
	if( root->data == data ) return root;

	if( value <= root->value )
		return treeNodeSearch(root->left_child, data, value);
	else
		return treeNodeSearch(root->right_child, data, value);
}

TreeNode * treeNodeFindMin(TreeNode * node) {
	while(node->left_child)
		node = node->left_child;
	return node;
}

TreeNode * treeNodeFindMax(TreeNode * node) {
	while(node->right_child)
		node = node->right_child;
	return node;
}

void treeNodeReplaceNodeInParent(TreeNode * old, TreeNode * new) {
	if( ! old->parent ) return;
	if( old->parent->left_child == old )
		old->parent->left_child = new;
	else old->parent->right_child = new;
}

void treeNodeMoveAt(TreeNode * node, TreeNode * destination) {
	node->parent = destination->parent != node ?
		destination->parent :
		node->parent;
	node->left_child = destination->left_child != node ?
		destination->left_child :
		node->left_child;
	node->right_child = destination->right_child != node ?
		destination->right_child :
		node->right_child;
}

void treeNodeInversePosition(TreeNode * t1, TreeNode * t2) {
	TreeNode copy = *t1;
	if(t1->parent)
		treeNodeReplaceNodeInParent(t1, t2);
	if(t2->parent)
		treeNodeReplaceNodeInParent(t2, t1);
	if(t1->left_child)
		t1->left_child->parent = t2;
	if(t1->right_child)
		t1->right_child->parent = t2;
	if(t2->left_child)
		t2->left_child->parent = t1;
	if(t2->right_child)
		t2->right_child->parent = t1;
	treeNodeMoveAt(t1, t2);
	treeNodeMoveAt(t2, &copy);
}

TreeNode * treeNodeDetach(TreeNode * node) {
	if( ! node ) return NULL;

	if( node->left_child && node->right_child ) {
		TreeNode * successor = treeNodeFindMin(node->right_child);
		treeNodeInversePosition(node, successor);
		treeNodeDetach(node);
		return successor;
	}
	if( node->left_child ) {
		node->left_child->parent = node->parent;
		treeNodeReplaceNodeInParent(node, node->left_child);
		return node->left_child;
	}
	if( node->right_child ) {
		node->right_child->parent = node->parent;
		treeNodeReplaceNodeInParent(node, node->right_child);
		return node->right_child;
	}

	treeNodeReplaceNodeInParent(node, NULL);
	return NULL;
}

/** ------------------------- SORTED TREE ---------------------------------- **/

SortedTree sortedTreeCreate(TreeDataEvaluator evaluator) {
	return (SortedTree) {
		.root = NULL, .min_node = NULL, .max_node = NULL,
		.evaluator = evaluator, .size = 0
	};
}

void sortedTreeAddData(SortedTree * tree, void * data) {
	TreeNode * node = malloc(sizeof(TreeNode));
	(*node) = treeNodeCreate(data, tree->evaluator);

	sortedTreeAddNode(tree, node);
}

void sortedTreeAddNode(SortedTree * tree, TreeNode * node) {
	if( ! tree->root ) {
		tree->root = node;
		tree->min_node = node;
		tree->max_node = node;
	}
	else {
		treeNodeAdd(tree->root, node);
		if( node->value <= tree->min_node->value )
			tree->min_node = node;
		else if ( node-> value > tree->max_node->value )
			tree->max_node = node;
	}

	tree->size++;
}

TreeNode * sortedTreeSearchData(SortedTree * tree, void * data) {
	return treeNodeSearch(tree->root, data, tree->evaluator(data));
}

void sortedTreeRemoveNode(SortedTree * tree, TreeNode * node) {
	if( node == tree->min_node ) {
		if( node->right_child )
			tree->min_node = treeNodeFindMin(node->right_child);
		else if( node->parent )
			tree->min_node = node->parent;
		else tree->min_node = NULL;
	}
	if( node == tree->max_node ) {
		if( node->left_child )
			tree->max_node = treeNodeFindMax(node->left_child);
		else if( node->parent )
			tree->max_node = node->parent;
		else tree->max_node = NULL;
	}

	if( node == tree->root )
		tree->root = treeNodeDetach(node);
	else treeNodeDetach(node);

	tree->size--;
}

void sortedTreeUpdateNode(SortedTree * tree, TreeNode * node) {
	int old_value = node->value;
	node->value = tree->evaluator(node->data);
	if( node->value == old_value ) return;

	sortedTreeRemoveNode(tree, node);
	sortedTreeAddNode(tree, node);
}

void sortedTreeRemoveMin(SortedTree * tree) {
	sortedTreeRemoveNode(tree, tree->min_node);
}

TreeNode * sortedTreeGetMax(SortedTree * tree) {
	return tree->max_node;
}

void treeNodeApplyAll(TreeNode * node, TreeNodeApplication application) {
	if( ! node ) return;

	treeNodeApplyAll(node->left_child, application);
	application(node);
	treeNodeApplyAll(node->right_child, application);
}

void treeNodeParameterApplyAll(TreeNode * node,
	TreeNodeParametrisedApplication application, void * parameter) {
	if( ! node ) return;

	treeNodeParameterApplyAll(node->left_child, application, parameter);
	application(node, parameter);
	treeNodeParameterApplyAll(node->right_child, application, parameter);
}

void sortedTreeApplyAll(SortedTree * tree, TreeNodeApplication application) {
	treeNodeApplyAll(tree->root, application);
}

void sortedTreeParameterApplyAll(SortedTree * tree,
	TreeNodeParametrisedApplication application, void * parameter) {
	treeNodeParameterApplyAll(tree->root, application, parameter);
}

void treeNodeFreeApplication(TreeNode * node) {
	free(node);
}

void sortedTreeDestroy(SortedTree * tree) {
	sortedTreeApplyAll(tree, treeNodeFreeApplication);
	tree->root = NULL;
	tree->min_node = NULL;
	tree->max_node = NULL;
	tree->size = 0;
}

#include <stdio.h>
#include "util_math.h"

int testTreeDataEvaluator(void * data) {
	return data ? * ( (int *) data ) : -1;
}

#define SIZE 10000

int counter = 0;

void testTreeNodeApplication(TreeNode * node) {
	counter++;
	if(counter >= 3 * SIZE)
		abort();
	int pvalue = node->parent ? testTreeDataEvaluator(node->parent->data) : -1;
	int value = testTreeDataEvaluator(node->data);
	int lvalue = node->left_child ? testTreeDataEvaluator(node->left_child->data)
					: -2;
	int rvalue = node->right_child ? testTreeDataEvaluator(node->right_child->data)
					: -2;
	printf(" (%d)%d", pvalue, value);
	if( lvalue >= 0 && rvalue >= 0 )
		printf("(%d, %d)", lvalue, rvalue);
	else if (lvalue >= 0)
		printf("(%d)", lvalue);
	else if (rvalue >= 0)
		printf("(%d)", rvalue);
}

void sortedTreeMainTest(void) {
	//randomInitSeed(1533548688);
	int n = SIZE;
	int values[n];
	SortedTree tree = sortedTreeCreate( testTreeDataEvaluator );

	printf("Raw :");

	for(int k = 0; k < n; k++) {
		values[k] = rand_int_a_b(0, n);
		printf(" %d", values[k]);
		sortedTreeAddData( &tree, values + k );
	}
	printf("\nSorting :");

	sortedTreeApplyAll(&tree, testTreeNodeApplication);
	printf("\nWithout root :");

	sortedTreeRemoveNode(&tree, tree.root);

	sortedTreeApplyAll(&tree, testTreeNodeApplication);
	printf("\nWithout a few random nodes :");

	printf("\n");
	for(int k = 0; k < n/10; k++) {
		TreeNode * random_node = NULL;
		int random_index;
		do {
			random_index = rand_int_a_b(0, n);
			random_node = sortedTreeSearchData(&tree, values + random_index);
		} while( ! random_node );
		printf("removing %d\n", random_node->value);
		sortedTreeRemoveNode(&tree, random_node);
	}

	counter = 0;

	sortedTreeApplyAll(&tree, testTreeNodeApplication);
	printf("\n");

	sortedTreeDestroy(&tree);
}
