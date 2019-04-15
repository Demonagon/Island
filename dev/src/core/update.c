#include <stdio.h>
#include <stdlib.h>
#include "core/update.h"

/***************************** UPDATE HANDLE ******************************/

UpdateHandle update_handle_create_empty() {
	return (UpdateHandle) {
		.data = NULL,
		.declaration_function = NULL,
		.application_function = NULL,
		.flags = 0,
		.previous_handle = UPDATE_MANAGER_LIST_NO_OBJECT,
		.next_handle = UPDATE_MANAGER_LIST_NO_OBJECT,
		.cycle_list = UPDATE_MANAGER_NO_LIST
	};
}

UpdateHandle update_handle_init(void * data,
						CallBack declaration_function,
						CallBack application_function) {
	return (UpdateHandle) {
		.data = data,
		.declaration_function = declaration_function,
		.application_function = application_function,
		.flags = 0,
		.previous_handle = UPDATE_MANAGER_LIST_NO_OBJECT,
		.next_handle = UPDATE_MANAGER_LIST_NO_OBJECT,
		.cycle_list = UPDATE_MANAGER_NO_LIST
	};
}

void update_handle_mark_obsolete(UpdateHandle * handle) {
	handle->flags = 1;
}

int update_handle_is_obsolete(UpdateHandle * handle) {
	return !! handle->flags;
}

void update_handle_declaration_call(UpdateHandle * handle) {
	if( ! handle ) return;
	if( update_handle_is_obsolete(handle) ) return;
	if( ! handle->declaration_function ) return;
	handle->declaration_function(handle->data);
}

void update_handle_application_call(UpdateHandle * handle) {
	if( ! handle ) return;
	if( update_handle_is_obsolete(handle) ) return;
	if( ! handle->application_function ) return;
	handle->application_function(handle->data);
}

void update_handle_detach_link(UpdateHandle * handle) {
	handle->previous_handle = UPDATE_MANAGER_LIST_NO_OBJECT;
	handle->next_handle = UPDATE_MANAGER_LIST_NO_OBJECT;
	handle->cycle_list = UPDATE_MANAGER_NO_LIST;
}

/***************************** UPDATE REGISTER ****************************/

/** MEMORY ALLOCATION **/

UpdateManager update_manager_create_empty() {
	return (UpdateManager) {
		.handles_array = array_create_empty(),
		.cycle_count = 0,
		.phase = UPDATE_MANAGER_OFF_PHASE,
		.cycle_lists = NULL,
		.application_propagation_list = 0,
		.current_cycle_index = 0,
		.clock = 0
	};
}

ArrayIndex * update_manager_cycle_list_allocate(int cycle_count) {
	ArrayIndex * cycle_lists = malloc( sizeof(ArrayIndex) * cycle_count);
	for(int k = 0; k < cycle_count; k++)
		cycle_lists[k] = UPDATE_MANAGER_LIST_NO_OBJECT;
	return cycle_lists;
}

UpdateManager update_manager_init(int handle_capacity, int cycle_count, int cycle_capacity) {
	return (UpdateManager) {
		.handles_array = array_init(sizeof(UpdateHandle), handle_capacity),
		.cycle_count = cycle_count,
		.phase = UPDATE_MANAGER_OFF_PHASE,
		.cycle_lists = update_manager_cycle_list_allocate(cycle_count),
		.application_propagation_list = UPDATE_MANAGER_LIST_NO_OBJECT,
		.current_cycle_index = 0,
		.clock = 0
	};
}

void update_manager_free(UpdateManager * manager) {
	if( ! manager ) return;
	if( manager->cycle_lists ) free(manager->cycle_lists);
	(*manager) = update_manager_create_empty();
}

ArrayIndex update_manager_allocate_handle(UpdateManager * manager,
						void * data, 
						CallBack declaration_function,
						CallBack application_function) {
	UpdateHandle handle = update_handle_init(data,
											declaration_function,
											application_function);

	ArrayIndex index = array_allocate_slot(&manager->handles_array, &handle);

	if(index < 0 ) {
		fprintf(stderr, "ERROR : Update Manager : full update handle array ");
		fprintf(stderr, "(size = %d)\n", manager->handles_array.capacity);
	}
	
	return index;
}

void update_manager_free_handle(UpdateManager * manager, ArrayIndex index) {
	if(index < 0) return;

	if( manager->phase != UPDATE_MANAGER_OFF_PHASE &&
		manager->phase != UPDATE_MANAGER_CLEANUP_PHASE ) {
		update_handle_mark_obsolete( array_get(&manager->handles_array, index) );
		return;
	}

	update_manager_list_detach_handle(manager, index);
	(*(UpdateHandle*)array_get(&manager->handles_array, index)) = update_handle_create_empty();
	array_free_slot(&manager->handles_array, index);
}

/** CYCLE LIST ACCESS **/

ArrayIndex * update_manager_k_cycle_list(UpdateManager * manager, int k) {
	return manager->cycle_lists +
		(manager->current_cycle_index + k) % manager->cycle_count;
}

ArrayIndex * update_manager_current_cycle_list(UpdateManager * manager) {
	return update_manager_k_cycle_list(manager, 0);
}

/** CYCLE LIST MANIPULATION **/

void update_manager_list_add_handle(UpdateManager * manager, ArrayIndex index, int cycle) {
	UpdateHandle * current_handle = array_get(&manager->handles_array, index);
	if( current_handle->cycle_list > UPDATE_MANAGER_NO_LIST ) return;
	current_handle->previous_handle = UPDATE_MANAGER_LIST_INITIAL_OBJECT;
	current_handle->next_handle = manager->cycle_lists[cycle];
	current_handle->cycle_list = cycle;

	if( manager->cycle_lists[cycle] > UPDATE_MANAGER_LIST_NO_OBJECT ) {
		UpdateHandle * next_handle = array_get(&manager->handles_array, manager->cycle_lists[cycle]);
		next_handle->previous_handle = index;
	}

	manager->cycle_lists[cycle] = index;
}

void update_manager_list_propagate_handle(UpdateManager * manager, ArrayIndex index) {
	UpdateHandle * current_handle = array_get(&manager->handles_array, index);
	if( current_handle->cycle_list > UPDATE_MANAGER_NO_LIST ) return;
	current_handle->previous_handle = UPDATE_MANAGER_LIST_INITIAL_OBJECT;
	current_handle->next_handle = manager->application_propagation_list;
	current_handle->cycle_list = UPDATE_MANAGER_PROPAGATION_LIST;

	if( manager->application_propagation_list > UPDATE_MANAGER_LIST_NO_OBJECT ) {
		UpdateHandle * next_handle = array_get(
			&manager->handles_array,
			manager->application_propagation_list);
		next_handle->previous_handle = index;
	}

	manager->application_propagation_list = index;
}

UpdateHandle * update_manager_list_remove_previous(UpdateManager * manager, ArrayIndex index) {
	UpdateHandle * current_handle = array_get(&manager->handles_array, index);
	if(current_handle->previous_handle == UPDATE_MANAGER_LIST_NO_OBJECT)
		fprintf(stderr, "ERROR : update handle has empty previous handle in list\n");
	if(current_handle->previous_handle == UPDATE_MANAGER_LIST_INITIAL_OBJECT) {
		int cycle = current_handle->cycle_list;
		if( cycle == UPDATE_MANAGER_NO_LIST ) {
			fprintf(stderr, "ERROR : removing update handle without list from a list\n");
			return NULL;
		}
		if( cycle == UPDATE_MANAGER_PROPAGATION_LIST )
			manager->application_propagation_list = current_handle->next_handle;
		else
			manager->cycle_lists[cycle] = current_handle->next_handle;
	}
	else return array_get(&manager->handles_array, current_handle->previous_handle);
	return NULL;
}

UpdateHandle * update_manager_list_remove_next(UpdateManager * manager, ArrayIndex index) {
	UpdateHandle * current_handle = array_get(&manager->handles_array, index);
	if(current_handle->next_handle == UPDATE_MANAGER_LIST_INITIAL_OBJECT)
		fprintf(stderr, "ERROR : update handle has initial next element\n");
	else if(current_handle->next_handle != UPDATE_MANAGER_LIST_NO_OBJECT)
		return array_get(&manager->handles_array, current_handle->next_handle);
	return NULL;
}

void update_manager_list_detach_handle(UpdateManager * manager, ArrayIndex index) {
	if(index < 0) return;
	//if( manager->phase != UPDATE_MANAGER_OFF_PHASE &&
	//	manager->phase != UPDATE_MANAGER_CLEANUP_PHASE ) {
	//	update_handle_mark_obsolete( array_get(&manager->handles_array, index) );
	//	return;
	//}

	UpdateHandle * current_handle = array_get(&manager->handles_array, index);

	if( current_handle->previous_handle == UPDATE_MANAGER_LIST_NO_OBJECT ||
		current_handle->cycle_list == UPDATE_MANAGER_NO_LIST ) {
		update_handle_detach_link(current_handle);
		return;
	}

	UpdateHandle * previous_handle = update_manager_list_remove_previous(manager, index);
	UpdateHandle * next_handle = update_manager_list_remove_next(manager, index);

	if(previous_handle)
		previous_handle->next_handle = current_handle->next_handle;
	if(next_handle)
		next_handle->previous_handle = current_handle->previous_handle;

	update_handle_detach_link(current_handle);
}

void update_manager_cleanup_list(UpdateManager * manager, ArrayIndex index) {
	while(index >= 0) {
		UpdateHandle * current_handle = array_get(&manager->handles_array, index);
		index = current_handle->next_handle;
		update_handle_detach_link(current_handle);
		if( manager->phase == UPDATE_MANAGER_OFF_PHASE ||
			manager->phase == UPDATE_MANAGER_CLEANUP_PHASE )
			if( update_handle_is_obsolete(current_handle) )
				update_manager_free_handle(manager, index);
	}
}

void update_manager_cleanup_cycle_list(UpdateManager * manager, ArrayIndex index) {
	update_manager_cleanup_list(manager, manager->cycle_lists[index]);
	manager->cycle_lists[index] = UPDATE_MANAGER_LIST_NO_OBJECT;
}

void update_manager_cleanup_propagation_list(UpdateManager * manager) {
	update_manager_cleanup_list(manager, manager->application_propagation_list);
	manager->application_propagation_list = UPDATE_MANAGER_LIST_NO_OBJECT;
}

void update_manager_switch_cycle(UpdateManager * manager) {
	manager->current_cycle_index = 
		manager->current_cycle_index >= manager->cycle_count - 1 ?
			0 : manager->current_cycle_index + 1;
	manager->clock++;
}

/** HANDLE REGISTER **/

int update_manager_get_cycle_from_delay(UpdateManager * manager, int delay) {
	return (manager->current_cycle_index + delay) % manager->cycle_count;
}

int update_manager_get_index_current_cycle(UpdateManager * manager, ArrayIndex index) {
	UpdateHandle * handle = array_get(&manager->handles_array, index);
	return handle->cycle_list;
}

int update_manager_is_cycle_update_worthy(UpdateManager * manager, int old, int new) {
	if( old < 0 ) return 1;
	if( old == new ) return 0;
	if( old == manager->current_cycle_index ) return 0;
	if( ( new <  manager->current_cycle_index &&	// current update is before 
		  old <= manager->current_cycle_index ) ||	// or after both indexes
		( new >= manager->current_cycle_index &&	
		  old >  manager->current_cycle_index ) )
			return new < old; // sooner is better

	return	( old <  manager->current_cycle_index && // old is after new in time
			  new >= manager->current_cycle_index ); // because it loops back
}

void update_manager_register_handle(UpdateManager * manager, ArrayIndex index,
									int delay) {
	if(index < 0) return;
	int current_cycle = update_manager_get_index_current_cycle(manager, index);
	if(current_cycle == UPDATE_MANAGER_PROPAGATION_LIST) return;
	if(current_cycle > UPDATE_MANAGER_NO_LIST) {
		int new_cycle = update_manager_get_cycle_from_delay(manager, delay);
		if( ! update_manager_is_cycle_update_worthy(manager, current_cycle, new_cycle) )
			return;
		update_manager_list_detach_handle(manager, index);
	}

	if(delay == 0)
		update_manager_register_handle_now(manager, index);
	else update_manager_list_add_handle(
			manager,
			index,
			update_manager_get_cycle_from_delay(manager, delay)
		);
}

void update_manager_register_handle_now(UpdateManager * manager, ArrayIndex index) {
	if(index < 0) return;
	UpdateHandle * handle = array_get(&manager->handles_array, index);
	if( handle->cycle_list == UPDATE_MANAGER_PROPAGATION_LIST ||
		handle->cycle_list == manager->current_cycle_index )
		return;
	if( handle->cycle_list > UPDATE_MANAGER_NO_LIST )
		update_manager_list_detach_handle(manager, index);

	switch( manager->phase ) {
		case UPDATE_MANAGER_OFF_PHASE :
			update_manager_list_add_handle(manager, index, manager->current_cycle_index);
			break;
		case UPDATE_MANAGER_DECLARATION_PHASE :	
			update_manager_list_propagate_handle(manager, index);
			break;
		case UPDATE_MANAGER_APPLICATION_PHASE :
			update_manager_register_handle(manager, index, 1);
			break;
		case UPDATE_MANAGER_CLEANUP_PHASE :
			update_manager_register_handle(manager, index, 1);
			break;
	}
}

/** UPDATE **/

void update_manager_update_declaration_phase(UpdateManager * manager) {
	manager->phase = UPDATE_MANAGER_DECLARATION_PHASE;
	ArrayIndex index = *update_manager_current_cycle_list(manager);

	while(index > UPDATE_MANAGER_LIST_NO_OBJECT) {
		UpdateHandle * handle = array_get(&manager->handles_array, index);
		update_handle_declaration_call( handle );
		index = handle->next_handle;
	}
}

void update_manager_update_application_phase(UpdateManager * manager) {
	manager->phase = UPDATE_MANAGER_APPLICATION_PHASE;
	ArrayIndex index = *update_manager_current_cycle_list(manager);

	while(index > UPDATE_MANAGER_LIST_NO_OBJECT) {
		UpdateHandle * handle = array_get(&manager->handles_array, index);
		update_handle_application_call( handle );
		index = handle->next_handle;
	}

	index = manager->application_propagation_list;

	while(index > UPDATE_MANAGER_LIST_NO_OBJECT) {
		UpdateHandle * handle = array_get(&manager->handles_array, index);
		update_handle_application_call( handle );
		index = handle->next_handle;
	}
}

void update_manager_update_cleanup_phase(UpdateManager * manager) {
	manager->phase = UPDATE_MANAGER_CLEANUP_PHASE;
	update_manager_cleanup_cycle_list(manager, manager->current_cycle_index);
	update_manager_cleanup_propagation_list(manager);

	update_manager_switch_cycle(manager);

	manager->phase = UPDATE_MANAGER_OFF_PHASE;
}

void update_manager_update(UpdateManager * manager) {

	update_manager_update_declaration_phase(manager);

	update_manager_update_application_phase(manager);

	update_manager_update_cleanup_phase(manager);
}

/**************************** TEST ********************************/

typedef struct UpdateTestData {
	UpdateManager * manager;
	char symbol;
	ArrayIndex cascade;
} UpdateTestData;

UpdateTestData test_data_init(UpdateManager * manager, char symbol, ArrayIndex cascade) {
	return (UpdateTestData) {
		.manager = manager,
		.symbol = symbol,
		.cascade = cascade
	};
}

void test_letter_call_back_declaration(void * v_data) {
	UpdateTestData * data = v_data;
	printf("%c...\n", data->symbol);
	if( data->cascade >= 0 )
		update_manager_register_handle_now(data->manager, data->cascade);
}

void test_letter_call_back_application(void * v_data) {
	UpdateTestData * data = v_data;
	printf("%c !\n", data->symbol);
}

void test_2_declaration(void * v_data) {
	UpdateTestData * data = v_data;
	printf("%c...\n", data->symbol);
	if( data->cascade >= 0 )
		update_manager_register_handle(data->manager, data->cascade, 1);
}

void test_igniter_declaration(void * v_data) {
	UpdateTestData * data = v_data;
	printf("%c...\n", data->symbol);
	if( data->cascade >= 0 )
		update_manager_register_handle(data->manager, data->cascade, 1);
	update_manager_register_handle(data->manager, data->cascade - 1, 1);
}

void test_killer_declaration(void * v_data) {
	UpdateTestData * data = v_data;
	printf("%c...\n", data->symbol);
	if( data->cascade >= 0 )
		update_manager_free_handle(data->manager, data->cascade);
}

int main(void) {
	UpdateManager manager = update_manager_init(100, 10, 100);

	UpdateTestData data_A = test_data_init(&manager, 'A' + 0, 1);
	UpdateTestData data_B = test_data_init(&manager, 'A' + 1, 2);
	UpdateTestData data_C = test_data_init(&manager, 'A' + 2, 3);
	UpdateTestData data_D = test_data_init(&manager, 'A' + 3, 4);
	UpdateTestData data_E = test_data_init(&manager, 'A' + 4, 0);

	ArrayIndex index_A = update_manager_allocate_handle(&manager,
						&data_A, 
						test_letter_call_back_declaration,
						test_letter_call_back_application);
	ArrayIndex index_B = update_manager_allocate_handle(&manager,
						&data_B, 
						test_letter_call_back_declaration,
						test_letter_call_back_application);
	ArrayIndex index_C = update_manager_allocate_handle(&manager,
						&data_C, 
						test_letter_call_back_declaration,
						test_letter_call_back_application);
	ArrayIndex index_D = update_manager_allocate_handle(&manager,
						&data_D, 
						test_letter_call_back_declaration,
						test_letter_call_back_application);
	ArrayIndex index_E = update_manager_allocate_handle(&manager,
						&data_E, 
						test_letter_call_back_declaration,
						test_letter_call_back_application);
	
	update_manager_register_handle(&manager, index_A, 0);
	update_manager_register_handle(&manager, index_A, 1);
	update_manager_register_handle(&manager, index_B, 1);
	update_manager_register_handle(&manager, index_C, 1);
	update_manager_register_handle(&manager, index_D, 2);
	update_manager_register_handle(&manager, index_E, 2);

	for(int k = 0; k < 3; k++) {
		printf("Update %d :\n", k + 1);
		update_manager_update(&manager);
	}

	UpdateTestData data_F = test_data_init(&manager, 'A' + 5, 6);
	UpdateTestData data_G = test_data_init(&manager, 'A' + 6, 5);
	ArrayIndex index_F = update_manager_allocate_handle(&manager,
						&data_F, 
						test_2_declaration,
						test_letter_call_back_application);
	ArrayIndex index_G = update_manager_allocate_handle(&manager,
						&data_G,
						test_2_declaration,
						test_letter_call_back_application);
	index_G = index_G + 0; // removing unused warning

	update_manager_register_handle_now(&manager, index_F);

	for(int k = 3; k < 20; k++) {
		printf("Update %d :\n", k + 1);
		update_manager_update(&manager);
	}

	update_manager_free_handle(&manager, index_F);
	update_manager_free_handle(&manager, index_G);

	UpdateTestData data_H = test_data_init(&manager, 'A' + 7, -1);
	UpdateTestData data_I = test_data_init(&manager, 'A' + 8, -1);
	ArrayIndex index_H = update_manager_allocate_handle(&manager,
						&data_H, 
						test_igniter_declaration,
						test_letter_call_back_application);
	ArrayIndex index_I = update_manager_allocate_handle(&manager,
						&data_I,
						test_killer_declaration,
						test_letter_call_back_application);

	data_H.cascade = index_I;
	data_I.cascade = index_H;

	update_manager_register_handle_now(&manager, index_H);

	for(int k = 20; k < 40; k++) {
		printf("Update %d :\n", k + 1);
		update_manager_update(&manager);
	}

	update_manager_free(&manager);
}
