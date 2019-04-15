#include <stdlib.h>
#include <string.h>
#include "util/array.h"

Array array_create_empty() {
	return (Array) {
		.object_size = 0,
		.capacity = 0,
		.shortcut_index = 0,
		.data = NULL,
		.slot_availability_flag_array = NULL
	};
}

Array array_init(size_t object_size, int capacity) {
	return (Array) {
		.object_size = object_size,
		.capacity = capacity,
		.shortcut_index = 0,
		.data = malloc(object_size * capacity),
		.slot_availability_flag_array =
			calloc(
				( (capacity - 1) / 32 + 1),
				4				
			)
	};
}

void array_free(Array * array) {
	if( ! array ) return;
	if( array->data )
		free(array->data);
	if( array->slot_availability_flag_array )
		free(array->slot_availability_flag_array);
	(*array) = array_create_empty();
}

void * array_get(Array * array, ArrayIndex index) {
	return &array->data[index * array->object_size];
}

void array_set(Array * array, ArrayIndex index, void * value) {
	memcpy( array->data + ( index * array->object_size ),
			value,
			array->object_size
		);
}

int array_is_slot_available(Array * array, ArrayIndex index) {
	return ! (
		array->slot_availability_flag_array[ index / 32 ]
			& ( 1l << index % 32 )
	);
}

void array_reserve_slot(Array * array, ArrayIndex index) {
	array->slot_availability_flag_array[ index / 32 ]
		|=   ( 1l << index % 32 );

	if( array->shortcut_index == index ) array->shortcut_index++;
}

void array_free_slot(Array * array, ArrayIndex index) {
	array->slot_availability_flag_array[ index / 32 ]
		&= ~ ( 1l << index % 32 );

	if( array->shortcut_index > index ) array->shortcut_index = index;
}

ArrayIndex array_get_first_free_slot(Array * array) {
	for(ArrayIndex k = array->shortcut_index; k < array->capacity; k++)
		if( array_is_slot_available(array, k) )
			return k;
		else array->shortcut_index++;
	return -1;
}

ArrayIndex array_allocate_slot(Array * array, void * data) {
	ArrayIndex index = array_get_first_free_slot(array);
	if( index < 0 ) return -1;

	array_reserve_slot(array, index);
	array_set(array, index, data);
	return index;
}

/*#include <stdio.h>
int main(void) {
	printf("size = %ld\n", sizeof(int8_t) );
	Array array = array_init(sizeof(int8_t), 100);

	int8_t value1 = 14;
	int8_t value2 = 32;
	int8_t value3 = 124;
	int8_t value4 = 99;

	array_set(&array, 0, &value1);
	int8_t * value1b = array_get(&array, 0);
	array_set(&array, 1, &value2);
	int8_t * value2b = array_get(&array, 1);
	array_set(&array, 2, &value3);
	int8_t * value3b = array_get(&array, 2);

	printf("%d, %d, %d\n", *value1b, *value2b, *value3b);
	value1b = array_get(&array, 0);
	value2b = array_get(&array, 1);
	value3b = array_get(&array, 2);
	printf("%d, %d, %d\n", *value1b, *value2b, *value3b);
	array_set(&array, 1, &value4);
	value1b = array_get(&array, 0);
	value2b = array_get(&array, 1);
	value3b = array_get(&array, 2);
	printf("%d, %d, %d\n", *value1b, *value2b, *value3b);

	for(int k = 0; k < 3; k++)
		printf("%d ? -> %d\n", k, array_is_slot_available(&array, k));
	for(int k = 0; k < 3; k++)
		array_reserve_slot(&array, k);
	for(int k = 0; k < 3; k++)
		printf("%d ? -> %d\n", k, array_is_slot_available(&array, k));

	array_free_slot(&array, 0);
	array_free_slot(&array, 1);
	for(int k = 0; k < 3; k++)
		printf("%d ? -> %d\n", k, array_is_slot_available(&array, k));

	array_reserve_slot(&array, 0);
	for(int k = 0; k < 3; k++)
		printf("%d ? -> %d\n", k, array_is_slot_available(&array, k));

	array_reserve_slot(&array, 1);
	array_free_slot(&array, 2);
	for(int k = 0; k < 3; k++)
		printf("%d ? -> %d\n", k, array_is_slot_available(&array, k));
	for(int k = 0; k < 3; k++)
		array_free_slot(&array, k);

	int8_t values[4] = {value1, value2, value3, value4};

	for(int k = 0; k < 101; k++)
		printf("%d -> %d\n", k, array_allocate_slot(&array, values + (k%4)));

	array_free_slot(&array, 35);
	array_free_slot(&array, 62);

	printf("!! %d\n", array_allocate_slot(&array, &value1));
	printf("!! %d\n", array_allocate_slot(&array, &value2));

	array_free(&array);
}*/
