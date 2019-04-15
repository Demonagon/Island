#ifndef ARRAY__H
#define ARRAY__H
#include <stdint.h>
#include <stddef.h>

typedef int ArrayIndex;

typedef struct Array {

	// META DATA
	size_t object_size;
	int capacity;
	ArrayIndex shortcut_index;

	// DATA
	int8_t * data;
	int32_t * slot_availability_flag_array;
} Array;

Array array_create_empty();
Array array_init(size_t object_size, int capacity);
void array_free(Array * array);
void * array_get(Array * array, ArrayIndex index);
void array_set(Array * array, ArrayIndex index, void * value);
int array_is_slot_available(Array * array, ArrayIndex index);
void array_reserve_slot(Array * array, ArrayIndex index);
void array_free_slot(Array * array, ArrayIndex index);
ArrayIndex array_get_first_free_slot(Array * array);
ArrayIndex array_allocate_slot(Array * array, void * data);

#endif
