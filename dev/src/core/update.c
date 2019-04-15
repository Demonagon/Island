#include <stdio.h>
#include <stdlib.h>
#include "core/update.h"

/***************************** UPDATE HANDLE ******************************/

UpdateHandle updateHandleCreateEmpty() {
	return (UpdateHandle) {
		.data = NULL,
		.declaration_function = NULL,
		.application_function = NULL,
		.flags = 0
	};
}

UpdateHandle update_handle_init(void * data,
						CallBack declaration_function,
						CallBack application_function) {
	return (UpdateHandle) {
		.data = data,
		.declaration_function = declaration_function,
		.application_function = application_function,
		.flags = 0
	};
}

int update_handle_is_declaration_updated(UpdateHandle * handle) {
	return !! ( ( handle->flags ) & ( 1 << 0 ) );
}

void update_handle_set_declaration_flag(UpdateHandle * handle, int value) {
	if( value )
		handle->flags |=  ( 1 << 0 );
	else
		handle->flags &= ~( 1 << 0 );
}

int update_handle_is_application_updated(UpdateHandle * handle) {
	return !! ( ( handle->flags ) & ( 1 << 1 ) );
}

void update_handle_set_application_flag(UpdateHandle * handle, int value) {
	if( value )
		handle->flags |=  ( 1 << 1 );
	else
		handle->flags &= ~( 1 << 1 );
}

void update_handle_reset_flags(UpdateHandle * handle) {
	handle->flags = 0;
}

void update_handle_declaration_call(UpdateHandle * handle) {
	if( ! handle->declaration_function ) return;
	if( update_handle_is_declaration_updated(handle) ) return;
	handle->declaration_function(handle->data);
	update_handle_set_declaration_flag(handle, 1);
}

void update_handle_application_call(UpdateHandle * handle) {
	if( ! handle->application_function ) return;
	if( update_handle_is_application_updated(handle) ) return;
	handle->application_function(handle->data);
	update_handle_set_application_flag(handle, 1);
}

/***************************** UPDATE REGISTER ****************************/


UpdateManager update_manager_create_empty() {
	return (UpdateManager) {
		.handles_array = array_create_empty(),
		.cycle_count = 0,
		.cycle_capacity = 0,
		.phase = UPDATE_MANAGER_OFF_PHASE,
		.cycles = NULL,
		.cycles_length = NULL,
		.current_cycle_index = 0,
		.current_cycle_application_length = 0,
		.clock = 0
	};
}

UpdateManager update_manager_init(int handle_capacity, int cycle_count, int cycle_capacity) {
	return (UpdateManager) {
		.handles_array = array_init(sizeof(UpdateHandle), handle_capacity),
		.cycle_count = cycle_count,
		.cycle_capacity = cycle_capacity,
		.phase = UPDATE_MANAGER_OFF_PHASE,
		.cycles = malloc( sizeof(ArrayIndex) * cycle_count * cycle_capacity ),
		.cycles_length = malloc( sizeof(int) * cycle_count ),
		.current_cycle_index = 0,
		.current_cycle_application_length = 0,
		.clock = 0
	};
}

void update_manager_free(UpdateManager * manager) {
	if( ! manager ) return;
	if( manager->cycles ) free(manager->cycles);
	if( manager->cycles_length ) free(manager->cycles_length);
	(*manager) = update_manager_create_empty();
}

ArrayIndex * update_manager_k_cycle(UpdateManager * manager, int k) {
	return manager->cycles + (
			( manager->current_cycle_index + k ) %
			manager->cycle_count
		) * manager->cycle_capacity;
}

ArrayIndex * update_manager_current_cycle(UpdateManager * manager) {
	return manager->cycles +
		manager->current_cycle_index *
		manager->cycle_capacity; 
}

int update_manager_current_cycle_length(UpdateManager * manager) {
	return manager->cycles_length[ manager->current_cycle_index ];
}

void update_manager_switch_cycle(UpdateManager * manager) {
	manager->cycles_length[ manager->current_cycle_index ] = 0;
	manager->current_cycle_index = 
		manager->current_cycle_index >= manager->cycle_count - 1 ?
			0 : manager->current_cycle_index + 1;
}

/**
* Fonction qui sera la plus utilisée dans le code : c'est la fonction qui permet
* de rajouter une callback dans les listes du manager. Le paramètre delay est
* équivalent au nombre de listes qui seront sautées avant d'effectuer l'ajout.
* Pour executer la mise à jour le plus tôt possible, il suffit de mettre delay
* à 0.
* La valeur maximale acceptée pour delay est UPDATE_REGISTER_CYCLE_COUNT - 3
* (voir plus haut).
*/

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

void update_manager_cycle_add(UpdateManager * manager, ArrayIndex index, int cycle_index) {

	if(cycle_index >= manager->cycle_count) cycle_index = manager->cycle_count - 1;

	ArrayIndex * cycle = update_manager_k_cycle(manager, cycle_index);
	int * length = manager->cycles_length +
		(manager->current_cycle_index + cycle_index ) % manager->cycle_count;

	if( *length >= manager->cycle_capacity ) {
		fprintf(stderr, "ERROR : Update Manager : full update cycle ");
		fprintf(stderr, "(capacity = %d, index = %d)\n",
			manager->cycle_capacity, cycle_index);
		return;
	}

	cycle[*length] = index;
	(*length)++;
}

void update_manager_register_handle(UpdateManager * manager, ArrayIndex index,
									int cycle) {
	if(cycle == 0)
		update_manager_register_handle_now(manager, index);
	else update_manager_cycle_add(manager, index, cycle);
}

void update_manager_register_handle_now(UpdateManager * manager, ArrayIndex index) {
	ArrayIndex * cycle = NULL;
	int length = 0;
	switch( manager->phase ) {
		case UPDATE_MANAGER_OFF_PHASE :
			update_manager_cycle_add(manager, index, 0);
			break;
		case UPDATE_MANAGER_DECLARATION_PHASE :	
			cycle = update_manager_current_cycle(manager);
			length = manager->current_cycle_application_length;

			if( length >= manager->cycle_capacity ) {
				fprintf(stderr, "ERROR : Update Manager : full update cycle ");
				fprintf(stderr, "(capacity = %d, index = %d)\n",
					manager->cycle_capacity, manager->current_cycle_index);
				return;
			}

			cycle[length] = index;
			manager->current_cycle_application_length++;
			break;
		case UPDATE_MANAGER_APPLICATION_PHASE :
			update_manager_cycle_add(manager, index, 1);
			break;
	}
}

/*void updateRegisterAdd(UpdateRegister * update_register,
							 UpdateHandle * handle, int delay) {
	if(delay > UPDATE_REGISTER_CYCLE_COUNT - 3)
		delay = UPDATE_REGISTER_CYCLE_COUNT - 3;

	List * list = updateRegisterGetKthList(update_register, delay + 1);

	listAdd(list, &handle->list_link);
}

void updateRegisterAddToCurrentUpdate(UpdateRegister * update_register,
									  UpdateHandle * handle) {
	if( ! update_register->currently_updated_list )
		updateRegisterAdd(update_register, handle, 0);
	else
		listAdd(update_register->currently_updated_list, &handle->list_link);
}*/

/**
* Fonction pseudo lambda utilisée par la fonction suivante
*/

/*void updateRegisterLocalDeclarationFunction(void * data) {
	UpdateHandle * handle = data;
	updateHandleDeclarationCall(handle);
}

void updateRegisterLocalApplicationFunction(void * data) {
	UpdateHandle * handle = data;
	updateHandleApplicationCall(handle);
}*/

/**
* Fonction la plus importante, qui exécute toutes les handles, et passe à la
* liste suivante.
*/

void update_manager_update(UpdateManager * manager) {
	ArrayIndex * cycle = update_manager_current_cycle(manager);
	int length = update_manager_current_cycle_length(manager);
	manager->current_cycle_application_length = length;

	manager->phase = UPDATE_MANAGER_DECLARATION_PHASE;
	for(int k = 0; k < length; k++)
		update_handle_declaration_call(
			array_get(
				&manager->handles_array,
				cycle[k]
			)
		);

	manager->phase = UPDATE_MANAGER_APPLICATION_PHASE;
	for(int k = 0; k < manager->current_cycle_application_length; k++)
		update_handle_application_call(
			array_get(
				&manager->handles_array,
				cycle[k]
			)
		);

	for(int k = 0; k < manager->current_cycle_application_length; k++)
		update_handle_reset_flags(
			array_get(
				&manager->handles_array,
				cycle[k]
			)
		);

	update_manager_switch_cycle(manager);
	manager->phase = UPDATE_MANAGER_OFF_PHASE;
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

int main(void) {
	UpdateManager manager = update_manager_init(100, 10, 100);

	for(int k = 0; k < 10; k++)
		printf("length %d = %d\n", k, manager.cycles_length[k]);

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

	printf("indexes = %d, %d, %d, %d, %d\n", index_A, index_B, index_C, index_D,
		index_E);
	
	update_manager_register_handle(&manager, index_A, 0);
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

	update_manager_register_handle_now(&manager, index_F);

	for(int k = 3; k < 20; k++) {
		printf("Update %d :\n", k + 1);
		update_manager_update(&manager);
	}

	update_manager_free(&manager);
}
