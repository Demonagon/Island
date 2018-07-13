#include <stdio.h>
#include <stdlib.h>
#include "update.h"

/***************************** UPDATE HANDLE ******************************/


void updateHandleInit(UpdateHandle * handle, void * data,
												CallBack function) {
	handle->data = data;
	handle->function = function;
	handle->list_link = listCreate();
	handle->list_link.data = handle;
}

void updateHandleCall(UpdateHandle handle) {
	handle.function(handle.data);
}

/***************************** UPDATE REGISTER ****************************/

void updateRegisterInit(UpdateRegister * update_register) {
	update_register->current_list = 0;
	for(int k = 0; k < UPDATE_REGISTER_CYCLE_COUNT; k++)
		update_register->update_lists[k] = listCreate();
}

List * updateRegisterGetCurrentList(UpdateRegister * update_register) {
	return &update_register->update_lists[update_register->current_list];
}

List * updateRegisterGetKthList(UpdateRegister * update_register, int k) {
	return update_register->update_lists + (
		(update_register->current_list + (k - 1)) % UPDATE_REGISTER_CYCLE_COUNT
	);
}

void updateRegisterSwitch(UpdateRegister * update_register) {
	update_register->current_list ++;
	update_register->current_list %= UPDATE_REGISTER_CYCLE_COUNT;
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

void updateRegisterAdd(UpdateRegister * update_register,
							 UpdateHandle * handle, int delay) {
	if(delay > UPDATE_REGISTER_CYCLE_COUNT - 3)
		delay = UPDATE_REGISTER_CYCLE_COUNT - 3;

	List * list = updateRegisterGetKthList(update_register, delay + 1);

	listAdd(list, &handle->list_link);
}

/**
* Fonction pseudo lambda utilisée par la fonction suivante
*/

void updateRegisterLocalUpdateApplication(void * data) {
	UpdateHandle * handle = data;
	updateHandleCall(*handle);
}

/**
* Fonction la plus importante, qui exécute toutes les handles, et passe à la
* liste suivante.
*/

void updateRegisterUpdate(UpdateRegister * update_register) {
	List * list = updateRegisterGetCurrentList(update_register);

	updateRegisterSwitch(update_register);

	listApplyAll(*list, updateRegisterLocalUpdateApplication);
}

/**************************** TEST ********************************/

void callBackA(void * a) {
	printf("A !\n");
}

void callBackB(void * b) {
	printf("B ! \n");
}

void callBackC(void * c) {
	printf("C ! \n");
}

void updateTest() {
	UpdateRegister reg;

	updateRegisterInit(&reg);

	UpdateHandle A1, A2, A3, B1, B2, C1, C2, C3;

	updateHandleInit(&A1, NULL, callBackA);
	updateHandleInit(&A2, NULL, callBackA);
	updateHandleInit(&A3, NULL, callBackA);
	updateHandleInit(&B1, NULL, callBackB);
	updateHandleInit(&B2, NULL, callBackB);
	updateHandleInit(&C1, NULL, callBackC);
	updateHandleInit(&C2, NULL, callBackC);
	updateHandleInit(&C3, NULL, callBackC);

	printf("tout en même temps\n");
	updateRegisterAdd( &reg, &A1, 0);
	updateRegisterAdd( &reg, &C1, 0);
	updateRegisterAdd( &reg, &B1, 0);
	updateRegisterAdd( &reg, &A2, 1);
	updateRegisterAdd( &reg, &C2, 1);
	updateRegisterAdd( &reg, &B2, 2);
	updateRegisterAdd( &reg, &A3, 2);
	updateRegisterAdd( &reg, &C3, 3);
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterUpdate(&reg);
	printf("\n");

	printf("par salves\n");
	updateRegisterAdd( &reg, &A1, 0);
	updateRegisterAdd( &reg, &C1, 0);
	updateRegisterAdd( &reg, &B1, 0);
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterAdd( &reg, &A2, 0);
	updateRegisterAdd( &reg, &C2, 0);
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterAdd( &reg, &B2, 0);
	updateRegisterAdd( &reg, &A3, 0);
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterAdd( &reg, &C3, 0);
	updateRegisterUpdate(&reg);
	printf("\n");

	printf("par salves mélangées\n");
	updateRegisterAdd( &reg, &C1, 3);
	updateRegisterAdd( &reg, &A1, 0);
	updateRegisterAdd( &reg, &A2, 1);
	updateRegisterAdd( &reg, &C2, 0);
	updateRegisterAdd( &reg, &B1, 0);
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterAdd( &reg, &C3, 0);
	updateRegisterAdd( &reg, &B2, 1);
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterAdd( &reg, &A3, 0);
	updateRegisterUpdate(&reg);
	printf("\n");
	updateRegisterUpdate(&reg);
	printf("\n");

	
}
