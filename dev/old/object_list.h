#ifndef GAMEOBJECTLIST__H
#define GAMEOBJECTLIST__H

#include "core/object.h"

/**
* Fait le 9 Juillet 2018 par Pacôme
* Code d'une simple liste chaînée. Chaque chaînon possède une information
* sous la forme d'un GameObject. Pour des liste chaînées dont l'information
* ne doit pas être contenue sous pointeurs, écrire sa propre copie de
* ce code, en remplaçant GameObject par le type désiré. (en gros.)
* Afin de prévenir toute boucle d'information dans la mémoire, un chaînon
* est automatiquement retiré de l'endroit où il est avant d'être rajouté
* quelque part.
*/

typedef struct GameObjectListLink {
	GameObject object;
	struct GameObjectListLink * previous;
	struct GameObjectListLink * next;
} GameObjectListLink;

typedef GameObjectListLink GameObjectList;

typedef void (*GameObjectListApplication) (GameObject *);

// S'applique sur le premier argument avec le second comme paramètre
typedef void (*ParameterizedGameObjectListApplication) (GameObject *, void *);

GameObjectListLink * objectListLinkAllocate(GameObject data);
void objectListLinkDestroy(GameObjectListLink * link);
GameObjectList objectListCreate();
GameObjectListLink objectListLinkCreate(GameObject data);
void objectLinkConnect(GameObjectListLink * a, GameObjectListLink * b);
void objectListAdd(GameObjectList * list, GameObjectListLink * new_link);
void objectListLinkDetach(GameObjectListLink * link);
void objectListClear(GameObjectList * list);
void objectListApplyAll(GameObjectList list, GameObjectListApplication application);
void objectListParameterizedApplyAll(
				GameObjectList list,
				ParameterizedGameObjectListApplication application,
				void * parameters);

void mainGameObjectListTest();

/*----------------------------------------------------------------------------*/

/**
* Le code qui suit va servir à intialiser des objets correctement dans la liste.
* Le protocole est le suivant : lorsque l'on veut ajouter un objet, on
* joint une copie de l'objet physique et une fonction.
* L'objet physique est placé dans la liste, puis la fonction appellée sur
* le chaînon. Le but de la fonction est de permettre d'initialiser toute
* sorte de pointeur ; par exemple, initialiser un pointeur sur le chaînon
* lui même, ce qui est nécessaire pour tout objet qui souhaite pouvoir
* s'autodétruire.
*/

#endif
