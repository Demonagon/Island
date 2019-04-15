#ifndef UPDATE__H
#define UPDATE__H
#include "util/array.h"

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

//#define UDPATE_MANAGER_MAX_HANDLE_PER_CYCLE 1000
//#define UPDATE_MANAGER_CYCLE_COUNT 50


/**
* Mise à jour, avril 2019. On refait tout.
* Un principe à changé : afin de permettre à des objects qui n'auraient pas
* été inclus dans la phase déclarative de la mise à jour d'appliquer tout de
* même des modifications à la phase applicative. Pour cela, on considère que
* placer un handler dans le cycle numéro 0 inclu l'objet dans la mise à jour
* aussi tôt que possible. Si nous somme en phase déclarative, l'objet sera inclu
* dans la liste pour la phase applicative.
* Suite à la simplification drastique des structures de données on va inclure
* un ensemble de flags dans les handlers afin de permettre de ne pas mettre à
* jour un même objet deux fois.
*/

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
	int8_t flags;

	// Linked List elements
	ArrayIndex previous_handle;
	ArrayIndex next_handle;
	int cycle_list;
} UpdateHandle;

UpdateHandle update_handle_create_empty();

UpdateHandle update_handle_init(void * data,
						CallBack declaration_function,
						CallBack application_function);

void update_handle_mark_obsolete(UpdateHandle * handle);
int update_handle_is_obsolete(UpdateHandle * handle);

void update_handle_declaration_call(UpdateHandle * handle);

void update_handle_application_call(UpdateHandle * handle);

void update_handle_detach_link(UpdateHandle * handle);

/****************************** UPDATE REGISTER *******************************/

#define UPDATE_MANAGER_LIST_NO_OBJECT -1
#define UPDATE_MANAGER_LIST_INITIAL_OBJECT -2
#define UPDATE_MANAGER_PROPAGATION_LIST -1
#define UPDATE_MANAGER_NO_LIST -2

typedef enum UpdateManagerPhase {
	UPDATE_MANAGER_OFF_PHASE,
	UPDATE_MANAGER_DECLARATION_PHASE,
	UPDATE_MANAGER_APPLICATION_PHASE,
	UPDATE_MANAGER_CLEANUP_PHASE
} UpdateManagerPhase;

typedef struct UpdateManager {
	Array handles_array;

	int cycle_count;

	UpdateManagerPhase phase;

	ArrayIndex * cycle_lists;
	ArrayIndex application_propagation_list;

	int current_cycle_index;

	long int clock;
} UpdateManager;

/** MEMORY ALLOCATION **/

UpdateManager update_manager_create_empty();

UpdateManager update_manager_init(int handle_capacity, int cycle_count, int cycle_capacity);

void update_manager_free(UpdateManager * manager);

ArrayIndex update_manager_allocate_handle(UpdateManager * manager,
						void * data, 
						CallBack declaration_function,
						CallBack application_function);

void update_manager_free_handle(UpdateManager * manager, ArrayIndex index);

/** CYCLE LIST ACCESS **/

ArrayIndex * update_manager_k_cycle_list(UpdateManager * manager, int k);
ArrayIndex * update_manager_current_cycle_list(UpdateManager * manager);

/** CYCLE RAW LIST MANIPULATION **/

// These functions will not operate if the handle is already in a list.
void update_manager_list_add_handle(UpdateManager * manager, ArrayIndex index, int cycle);
void update_manager_list_propagate_handle(UpdateManager * manager, ArrayIndex index);

void update_manager_list_detach_handle(UpdateManager * manager, ArrayIndex index);

void update_manager_cleanup_list(UpdateManager * manager, ArrayIndex index);
void update_manager_cleanup_cycle_list(UpdateManager * manager, ArrayIndex index);
void update_manager_cleanup_propagation_list(UpdateManager * manager);

void update_manager_switch_cycle(UpdateManager * manager);

/** HANDLE REGISTER **/

void update_manager_register_handle(UpdateManager * manager, ArrayIndex index,
									int delay);
void update_manager_register_handle_now(UpdateManager * manager, ArrayIndex index);

/** UPDATE **/

void update_manager_update(UpdateManager * manager);

/************************************* TEST ***********************************/

int main(void);

#endif
