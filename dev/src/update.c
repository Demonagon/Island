#include <stdio.h>
#include <stdlib.h>
#include "update.h"

UpdateHandle update_register_arrays
	[UPDATE_REGISTER_ARRAY_SIZE * UPDATE_REGISTER_CYCLE_COUNT];
int update_register_sizes[UPDATE_REGISTER_CYCLE_COUNT];
int update_register_current_array;

/***************************** UPDATE HANDLE ******************************/


UpdateHandle updateHandleInit(UpdateHandle * handle, void * data,
												CallBack function) {
	handle->data = data;
	handle->function = function;
	handle->list_link = listCreate();
	handle->list_link.data = beacon;
}

void updateHandleCall(UpdateHandle handle) {
	handle.function(handle.data);
}

/***************************** UPDATE REGISTER ****************************/

UpdateRegister updateRegisterCreate() {
	return (UpdateRegister) {
		.update_lists = malloc( sizeof(List) * UPDATE_REGISTER_CYCLE_COUNT ),
		.current_list = 0 };
}

List * updateRegisterGetCurrentList() {
	return 
}

List * updateRegisterGetKthList(int k);

void updateRegisterSwitch();

/**
* Fonction qui sera la plus utilisée dans le code : c'est la fonction qui permet
* de rajouter une callback dans les listes du manager. Le paramètre delay est
* équivalent au nombre de listes qui seront sautées avant d'effectuer l'ajout.
* Pour executer la mise à jour le plus tôt possible, il suffit de mettre delay
* à 0.
* La valeur maximale acceptée pour delay est UPDATE_REGISTER_CYCLE_COUNT - 2
* (voir plus haut).
*/

void updateAdd(UpdateHandle handle, int delay);

/**
* Fonction la plus importante, qui exécute toutes les handles, et passe à la
* liste suivante.
*/

void updateHandles();

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
