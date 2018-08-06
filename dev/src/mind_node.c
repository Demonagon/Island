#include "mind_node.h"
#include <stdlib.h>

MindNodeIngredientProcessor mindNodeIngredientProcessorCreate(
	MindNodeIngredientProcessorFunction on_new,
	MindNodeIngredientCheckFunction ingredient_checker) {
	return (MindNodeIngredientProcessor) {
		.on_new = on_new,
		.ingredient_checker = ingredient_checker
	};
}

/** ---------------------- MIND NODE PARENT ------------------------ **/

MindNodeParent mindNodeParentOutputCreate(void * data,
	MindNodeOutputCallback on_new_solution,
	MindNodeOutputCallback on_deleted_solution) {
	return (MindNodeParent) {
		.data = data,
		.on_new_solution = on_new_solution,
		.on_deleted_solution = on_deleted_solution
	};
}

void MindNodeDefaultNewOutputCallback(void * data, void * solution, int value) {
	MindNode * node = data;
	if(node->ingredient_processor.on_new)
		node->ingredient_processor.on_new(node, solution, value);
}

void MindNodeDefaultDeletedOutputCallback(void * data, void * solution, int value) {
	MindNode * node = data;
	MindNodeOnDeletedIngredient(node, solution);
}

MindNodeParent mindNodeParentNodeCreate(MindNode * node) {
	return mindNodeParentOutputCreate(
		node,
		MindNodeDefaultNewOutputCallback,
		MindNodeDefaultDeletedOutputCallback
	);
}

/** ------------------------ MIND NODE ----------------------------- **/

void MindNodeDefaultNewTokenCallback(void * data, void * token, int value) {
	MindNode * node = data;
	node->parent.on_new_solution(data, token, value);
}

void MindNodeDefaultDeletedTokenCallback(void * data, void * token, int value) {
	MindNode * node = data;
	node->parent.on_deleted_solution(data, token, value);
}

void mindNodeInit(
	MindNode * node, 
	MindNodeParent parent,
	MindNodeIngredientProcessor ingredient_processor,
	int max_products,
	MindProductEvaluator memory_evaluator) {
	*node = (MindNode) {
		.parent = parent,
		.ingredient_processor = ingredient_processor,
		.product_memory = mindMemoryCreate(
			node,
			max_products,
			MindNodeDefaultNewTokenCallback,
			MindNodeDefaultDeletedTokenCallback,
			memory_evaluator)
	};
}

void mindChildNodeInit(
	MindNode * node, 
	MindNode * parent_node,
	MindNodeIngredientProcessor ingredient_processor,
	int max_products,
	MindProductEvaluator memory_evaluator) {
	mindNodeInit(
		node,
		mindNodeParentNodeCreate(parent_node),
		ingredient_processor,
		max_products,
		memory_evaluator);
}

void mindNodeAddProduct(MindNode * node, void * product) {
	mindMemoryProcessToken(&node->product_memory, product);
}

void mindNodeForgetHalf(MindNode * node) {
	mindMemoryForgetHalf(&node->product_memory);
}

typedef struct {
	void * ingredient;
	TreeNode ** array;
	MindNode * node;
} IngredientDeletingData;

void ingredientDeletingApplication(void * data, TreeNode * node) {
	IngredientDeletingData * d_data = data;
	if( d_data->node->ingredient_processor.ingredient_checker
	 && d_data->node->ingredient_processor.ingredient_checker(
			node->data,
			d_data->ingredient)
	) {
		d_data->array[0] = node;
		d_data->array++;
		d_data->array[0] = NULL;
	}
}

void MindNodeOnDeletedIngredient(MindNode * node, void * ingredient) {
	TreeNode ** treeNodeToDelete =
		malloc( sizeof(TreeNode *) *
				node->product_memory.memory_tree.size + 1);
	treeNodeToDelete[0] = NULL;

	IngredientDeletingData data = (IngredientDeletingData) {
		.ingredient = ingredient,
		.array = treeNodeToDelete,
		.node = node
	};

	sortedTreeParameterApplyAll(
		&node->product_memory.memory_tree,
		ingredientDeletingApplication,
		&data
	);

	while( treeNodeToDelete[0] ) {
		mindMemoryRemoveNode(&node->product_memory, treeNodeToDelete[0]);
		treeNodeToDelete++;
	}

	free(treeNodeToDelete);
}
