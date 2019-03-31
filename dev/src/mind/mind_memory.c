#include "mind/mind_memory.h"

MindMemory mindMemoryCreate(void * data, int max_size,
	MemoryTokenCallback on_new_memory_callback,
	MemoryTokenCallback on_deleted_memory_callback,
	MemoryTokenEvaluator memory_evaluator,
	void * context_data) {
	return (MindMemory) {
		.memory_tree = sortedTreeCreate(memory_evaluator, context_data),
		.max_size = max_size,
		.data = data,
		.on_new_memory_callback = on_new_memory_callback,
		.on_deleted_memory_callback = on_deleted_memory_callback
	};
}

void mindMemoryDestroy(MindMemory * memory) {
	sortedTreeDestroy(&memory->memory_tree);
}

void mindMemoryForgetOne(MindMemory * memory) {
	if( ! memory->memory_tree.size ) return;
	
	if(memory->on_deleted_memory_callback)
		memory->on_deleted_memory_callback(
			memory->data,
			memory->memory_tree.min_node->data,
			memory->memory_tree.min_node->value);
	sortedTreeRemoveMin(&memory->memory_tree);
}

void mindMemoryForget(MindMemory * memory, int to_forget) {
	for(int k = 0; k < to_forget; k++) {
		if( memory->memory_tree.size <= 0 ) break;
		mindMemoryForgetOne(memory);
	}
}

void mindMemoryForgetHalf(MindMemory * memory) {
	mindMemoryForget(memory, memory->memory_tree.size / 2);
}

void mindMemoryForgetUpToNumber(MindMemory * memory, int target_size) {
	if( target_size >= memory->memory_tree.size ) return;
	mindMemoryForget(memory, memory->memory_tree.size - target_size);
}

void mindMemoryProcessToken(MindMemory * memory, void * token) {
	if( memory->memory_tree.size >= memory->max_size ) {
		int value = memory->memory_tree.evaluator(memory->memory_tree.data, token);
		if( value <= memory->memory_tree.min_node->value ) return;
		else while( memory->memory_tree.size >= memory->max_size )
			sortedTreeRemoveMin(&memory->memory_tree);
	}

	TreeNode * node = sortedTreeAddData(&memory->memory_tree, token);
	if(memory->on_new_memory_callback)
		memory->on_new_memory_callback(memory->data, token, node->value);
}

void mindMemoryRemoveToken(MindMemory * memory, void * token) {
	TreeNode * node = sortedTreeSearchData(&memory->memory_tree, token);

	mindMemoryRemoveNode(memory, node);
}

void mindMemoryUpdateToken(MindMemory * memory, void * token) {
	TreeNode * node = sortedTreeSearchData(&memory->memory_tree, token);
	if( ! node ) return;
	sortedTreeUpdateNode(&memory->memory_tree, node);
}

void mindMemoryRemoveNode(MindMemory * memory, TreeNode * node) {
	if( ! node ) return;

	if(memory->on_deleted_memory_callback)
		memory->on_deleted_memory_callback(memory->data, node->data, node->value);
	sortedTreeRemoveNode(&memory->memory_tree, node);
}

void mindMemoryCopyNodeApplication(TreeNode * node, void * data) {
	SortedTree * new_tree = data;
	sortedTreeAddData(new_tree, node->data);
}

void * mindMemoryGetMaxToken(MindMemory * memory) {
	return memory->memory_tree.max_node->data;
}

/*int mindMemoryGetNMaxToken(MindMemory * memory, int n, void * result) {
	if( n <= 0 ) return 0;
	TreeNode * max_node = memory->memory_tree.max_node->data;

	int k = 0;

	while( n - k > 0 && max_node ) {
		result[k] = max_node;
		if( max_node->left_child )
			max_node = treeNodeFindMax(node->left_child);
		else if( max_node->parent )
			max_node = max_node->parent;
		else max_node = NULL;
		k++;
	}

	return k;
}*/

void mindMemoryChangeTokenEvaluator(MindMemory * memory, 
	MemoryTokenEvaluator evaluator) {
	SortedTree new_tree = sortedTreeCreate(evaluator, memory->memory_tree.data);
	sortedTreeParameterApplyAll(&memory->memory_tree,
		mindMemoryCopyNodeApplication, &new_tree);
	sortedTreeDestroy(&memory->memory_tree);
	memory->memory_tree = new_tree;
}
