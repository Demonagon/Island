#include "mind_node.h"

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
	node->ingredient_processor.on_new(node, solution, value);
}

void MindNodeDefaultDeletedOutputCallback(void * data, void * solution, int value) {
	MindNode * node = data;
	node->ingredient_processor.on_deleted(node, solution, value);
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
	MindProductEvaluator memory_evaluator);

void mindNodeAddProduct(MindNode * node, void * product) {
	mindMemoryProcessToken(&node->product_memory, product);
}

void mindNodeForgetHalf(MindNode * node) {
	mindMemoryForgetHalf(&node->product_memory);
}
