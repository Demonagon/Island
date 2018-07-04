#include <stdio.h>
#include <stdlib.h>
#include "update.h"

UpdateHandle update_register_arrays
	[UPDATE_REGISTER_ARRAY_SIZE * UPDATE_REGISTER_CYCLE_COUNT];
int update_register_sizes[UPDATE_REGISTER_CYCLE_COUNT];
int update_register_current_array;

/***************************** UPDATE HANDLE ******************************/

UpdateHandle handleCreate(void * data, CallBack function) {
	return (UpdateHandle) { .data = data, .function = function };
}

void handleCall(UpdateHandle handle) {
	handle.function(handle.data);
}

/***************************** UPDATE REGISTER ****************************/

void updateRegisterInit() {
	for(int k = 0; k < UPDATE_REGISTER_CYCLE_COUNT; k++)
		update_register_sizes[k] = 0;
	update_register_current_array = 0;
}

UpdateHandle * updateRegisterGetCurrentArray() {
	return update_register_arrays +
				 update_register_current_array * UPDATE_REGISTER_ARRAY_SIZE;
}

void updateRegisterClearCurrentArray() {
	update_register_sizes[update_register_current_array] = 0;
}

UpdateHandle * updateRegisterGetKthArray(int k) {
	return update_register_arrays +
			( (update_register_current_array + k) % UPDATE_REGISTER_CYCLE_COUNT )
			* UPDATE_REGISTER_ARRAY_SIZE;
}

int updateRegisterGetKthArraySize(int k) {
	return update_register_sizes[(update_register_current_array + k) % 
		UPDATE_REGISTER_CYCLE_COUNT];
}

void updateRegisterSetKthArraySize(int k, int size) {
	update_register_sizes[(update_register_current_array + k) % 
		UPDATE_REGISTER_CYCLE_COUNT] = size;
}

void updateRegisterSwitch() {
	update_register_current_array++;
	update_register_current_array %= UPDATE_REGISTER_CYCLE_COUNT;
}

void updateAdd(UpdateHandle handle, int delay) {
	if(delay < 0) {
		fprintf(stderr, "Update Register : updateAdd(%p, %d)",
						(void*) &handle, delay);
		fprintf(stderr, " : Error : negative delay parameter\n");
		exit(-1);
	}
	if(delay > UPDATE_REGISTER_CYCLE_COUNT - 1) {
		fprintf(stderr, "Update Register : updateAdd(%p, %d)",
						(void*) &handle, delay);
		fprintf(stderr, " : Error : delay parameter cannot exceed ");
		fprintf(stderr, "UPDATE_REGISTER_CYCLE_COUNT - 2 ( = %d)\n",
						UPDATE_REGISTER_CYCLE_COUNT - 2);
		exit(-1);
	}

	int array_size = updateRegisterGetKthArraySize(delay); 

	if(array_size >= UPDATE_REGISTER_ARRAY_SIZE) {
		fprintf(stderr, "Update Register : updateAdd(%p, %d)", 
						(void*) &handle, delay);
		fprintf(stderr, " : Warning : update array full. Handle pushback");
		fprintf(stderr, " dismissed.\n");
		return;
	}

	UpdateHandle * array = updateRegisterGetKthArray(delay);

	array[array_size] = handle;

	updateRegisterSetKthArraySize(delay, array_size + 1);
}

void updateHandles() {
	UpdateHandle * array = updateRegisterGetCurrentArray();
	int array_size = updateRegisterGetKthArraySize(0);
	for(int k = 0; k < array_size; k++)
		handleCall(array[k]);

	updateRegisterClearCurrentArray();
	updateRegisterSwitch();
}

/**************************** TEST ********************************/

void callbackA(void * a) {
	printf("A !\n");
}

void callbackB(void * b) {
	printf("B ! \n");
}

void callbackC(void * c) {
	printf("C ! \n");
}

void updateTest() {
	updateRegisterInit();

	printf("tout en même temps\n");
	updateAdd( handleCreate(NULL, callbackA), 0);
	updateAdd( handleCreate(NULL, callbackC), 0);
	updateAdd( handleCreate(NULL, callbackB), 0);
	updateAdd( handleCreate(NULL, callbackA), 1);
	updateAdd( handleCreate(NULL, callbackC), 1);
	updateAdd( handleCreate(NULL, callbackB), 2);
	updateAdd( handleCreate(NULL, callbackA), 2);
	updateAdd( handleCreate(NULL, callbackC), 3);
	updateHandles();
	printf("\n");
	updateHandles();
	printf("\n");
	updateHandles();
	printf("\n");
	updateHandles();
	printf("\n");

	printf("par salves\n");
	updateAdd( handleCreate(NULL, callbackA), 0);
	updateAdd( handleCreate(NULL, callbackC), 0);
	updateAdd( handleCreate(NULL, callbackB), 0);
	updateHandles();
	printf("\n");
	updateAdd( handleCreate(NULL, callbackA), 0);
	updateAdd( handleCreate(NULL, callbackC), 0);
	updateHandles();
	printf("\n");
	updateAdd( handleCreate(NULL, callbackB), 0);
	updateAdd( handleCreate(NULL, callbackA), 0);
	updateHandles();
	printf("\n");
	updateAdd( handleCreate(NULL, callbackC), 0);
	updateHandles();
	printf("\n");

	printf("par salves mélangées\n");
	updateAdd( handleCreate(NULL, callbackC), 3);
	updateAdd( handleCreate(NULL, callbackA), 0);
	updateAdd( handleCreate(NULL, callbackA), 1);
	updateAdd( handleCreate(NULL, callbackC), 0);
	updateAdd( handleCreate(NULL, callbackB), 0);
	updateHandles();
	printf("\n");
	updateAdd( handleCreate(NULL, callbackC), 0);
	updateAdd( handleCreate(NULL, callbackB), 1);
	updateHandles();
	printf("\n");
	updateAdd( handleCreate(NULL, callbackA), 0);
	updateHandles();
	printf("\n");
	updateHandles();
	printf("\n");

	
}
