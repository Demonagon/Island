#ifndef UPDATE__H
#define UPDATE__H
#include "list.h"

/**
* Dans tout jeu, il y a un moment où l'on veut mettre à jour une liste d'objets
* tout les cycles. Dans notre projet, il est probable que la vaste majorité des
* objets n'aient pas besoin d'être mis à jour à tout les cycles. Par conséquent,
* je pense qu'il est fortement préférable d'adopter la stratégie du registre de
* mise à jour. Le registre conserve une liste des objets à mettre à jour au
* prochain cycle, qu'il assume vide par défaut. Les objets eux-même doivent
* s'inscrire au registre pour être appelés au prochain cycle.
* Ils peuvent également demander d'être mis à jour dans 2 cycles, voir plus,
* le maximum possible étant définit par le #define UPDATE_REGISTER_CYCLE_COUNT.
* Pour l'instant ce système est supporté par un tableau, ce qui veut dire que
* le nombre d'entité mis à jour par cycle est limité à un nombre définit par
* #define UPDATE_REGISTER_ARRAY_SIZE ; si cela devient un problème cela pourra µ
* toujours
* être pris en compte en changeant le tableau par une liste chaînée.
* -> mise à jour du 11 Juillet 2018 ; le remplacement par une liste chaînée est
* en cours.
*/

/**
* Je garde également en tête l'idée de rendre tout cela plus robuste, notamment
* avec une architecture qui permet d'éviter toute ambiguité dans l'application
* des règles de la mise à jour (comme par exemple quand un objet veut faire une
* action et est détruit en même temps), et donc également le calcul en
* parallèle (mais c'est pas tout de suite).
*/

#define UPDATE_REGISTER_CYCLE_COUNT 50

/**
* une instance de UpdateHandle est là pour permettre au registre d'appeler
* la mise à jour de plein d'objets différend sans avoir à changer quoi que
* ce soit au code de ce document. Open / Close principle
*/

typedef void (*CallBack)(void *);

typedef struct {
	void * data;
	CallBack declaration_function;
	CallBack application_function;
	ListLink list_link;
} UpdateHandle;

UpdateHandle updateHandleCreateEmpty();

void updateHandleInit(UpdateHandle * handle, void * data,
					  CallBack declaration_function,
					  CallBack application_function);

void updateHandleDeclarationCall(UpdateHandle * handle);

void updateHandleApplicationCall(UpdateHandle * handle);

void updateHandleRemove(UpdateHandle * handle);

/****************************** UPDATE REGISTER *******************************/

typedef struct UpdateRegister {
	List update_lists[UPDATE_REGISTER_CYCLE_COUNT];
	int current_list;

	long int clock;

	List * currently_updated_list;
} UpdateRegister;

void updateRegisterInit(UpdateRegister * update_register);

List * updateRegisterGetCurrentList(UpdateRegister * update_register);

List * updateRegisterGetKthList(UpdateRegister * update_register, int k);

void updateRegisterSwitch(UpdateRegister * update_register);

/**
* Fonction qui sera la plus utilisée dans le code : c'est la fonction qui permet
* de rajouter une callback dans les listes du manager. Le paramètre delay est
* équivalent au nombre de listes qui seront sautées avant d'effectuer l'ajout.
* Pour executer la mise à jour le plus tôt possible, il suffit de mettre delay
* à 0.
* La valeur maximale acceptée pour delay est UPDATE_REGISTER_CYCLE_COUNT - 3.
* La valeur delay est capée à ce maximum. De cette façon la liste précédente
* ne peut pas être modifiée, ce qui permet une bonne opération des choses.
*/

void updateRegisterAdd(UpdateRegister * update_register,
							 UpdateHandle * handle, int delay);

void updateRegisterAddToCurrentUpdate(UpdateRegister * update_register,
									  UpdateHandle * handle);

/**
* Fonction la plus importante, qui exécute toutes les handles, et passe à la
* liste suivante.
*/

void updateRegisterUpdate(UpdateRegister * update_register);

/************************************* TEST ***********************************/

void updateTest();

#endif
