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

void treeNodeDetach(TreeNode * node) {
	if( ! node ) return;

	if( ! node->left_child && ! node->right_child ) {
		if( ! node->parent ) return;
		if( node->parent->left_child == node )
			node->parent->left_child = NULL;
		else node->parent->right_child = NULL;
		return;
	}

	if( node->left_child && ! node->right_child ) {
		node->left_child->parent = node->parent;
		if( ! node->parent ) return;
		if( node->parent->left_child == node )
			node->parent->left_child = node->left_child;
		else node->parent->right_child = node->left_child;
		return;
	}

	if( ! node->left_child && node->right_child ) {
		node->right_child->parent = node->parent;
		if( ! node->parent ) return;
		if( node->parent->left_child == node )
			node->parent->left_child = node->right_child;
		else node->parent->right_child = node->right_child;
		return;
	}

	if( node->left_child && node->right_child ) {
		//TODO
	}
}

/** ------------------------- SORTED TREE ---------------------------------- **/

SortedTree sortedTreeCreate(TreeDataEvaluator evaluator) {
	return (SortedTree) {
		.root = NULL, .min_node = NULL, .max_node = NULL,
		.evaluator = evaluator
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
}

TreeNode * sortedTreeSearchData(SortedTree * tree, void * data) {
	return treeNodeSearch(tree->root, data, tree->evaluator(data));
}

void sortedTreeRemoveNode(SortedTree * tree, TreeNode * node) {
	if( node == tree->min_node ) {
		if( node->parent )
			tree->min_node = node->parent;
		else if( node->right_child )
			tree->min_node = node->right_child;
		else tree->min_node = NULL;
	}
	if( node == tree->max_node ) {
		if( node->parent )
			tree->max_node = node->parent;
		else if( node->left_child )
			tree->max_node = node->left_child;
		else tree->max_node = NULL;
	}
	if( node == tree->root )
		tree->root = node->left_child ? node->left_child : node->right_child;

	treeNodeDetach(node);
}

void sortedTreeUpdateNode(SortedTree * tree, TreeNode * node) {
	int old_value = node->value;
	node->value = tree->evaluator(node->data);
	if( node->value == old_value ) return;

	sortedTreeRemoveNode(tree, node);
	sortedTreeAddNode(tree, node);
}
