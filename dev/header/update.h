#ifndef UPDATE__H
#define UPDATE__H

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
* #define UPDATE_REGISTER_ARRAY_SIZE ; si cela devient un problème cela pourra toujours
* être pris en compte en changeant le tableau par une liste chaînée.
*/

/**
* Je garde également en tête l'idée de rendre tout cela plus robuste, notamment
* avec une architecture qui permet d'éviter toute ambiguité dans l'application
* des règles de la mise à jour (comme par exemple quand un objet veut faire une
* action et est détruit en même temps), et donc également le calcul en
* parallèle (mais c'est pas tout de suite).
*/

#define UPDATE_REGISTER_ARRAY_SIZE 100
#define UPDATE_REGISTER_CYCLE_COUNT 5

/**
* une instance de UpdateHandle est là pour permettre au registre d'appeler
* la mise à jour de plein d'objets différend sans avoir à changer quoi que
* ce soit au code de ce document. Open / Close principle
*/

typedef void (*CallBack)(void *);

typedef struct {
	void * data;
	CallBack function;
} UpdateHandle;

UpdateHandle handleCreate(void * data, CallBack function);

void handleCall(UpdateHandle handle);

/**
* Les variables globales qui constituent le registre de mise à jour : un tableau
* qui contient les listes, un tableau qui contient la taille de chaque liste
* et une valeur qui indique à quelle liste on est actuellement.
* NOTE IMPORTANTE : dans la pratique, il sera impossible d'utiliser le délai
* de delay = UPDATE_REGISTER_CYCLE_COUNT - 1, car ce tableau est utilisé lors
* de la mise à jour et ne pourra pas être modifié en même temps qu'il est lu
* (c'est un problème qui pourra être résolu avec une mise en parallèle).
*/

UpdateHandle update_register_arrays
	[UPDATE_REGISTER_ARRAY_SIZE * UPDATE_REGISTER_CYCLE_COUNT];
int update_register_sizes[UPDATE_REGISTER_CYCLE_COUNT];
int update_register_current_array;

void updateRegisterInit();

UpdateHandle * updateRegisterGetCurrentArray();
void updateRegisterClearCurrentArray();

UpdateHandle * updateRegisterGetKthArray(int k);
int updateRegisterGetKthArraySize(int k);
void updateRegisterSetKthArraySize(int k, int size);

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

void updateTest();

#endif
