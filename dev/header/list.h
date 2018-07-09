#ifndef LIST__H
#define LIST__H

/**
* Fait le 9 Juillet 2018 par Pacôme
* Code d'une simple liste chaînée. Chaque chaînon possède une information
* sous la forme d'un void *. Pour des liste chaînées dont l'information
* ne doit pas être contenue sous pointeurs, écrire sa propre copie de
* ce code, en remplaçant [void *] par le type désiré. (en gros.)
* Afin de prévenir toute boucle d'information dans la mémoire, un chaînon
* est automatiquement retiré de l'endroit où il est avant d'être rajouté
* quelque part.
*/

typedef struct ListLink {
	void * data;
	struct ListLink * previous;
	struct ListLink * next;
} ListLink;

typedef ListLink List;

typedef void (*ListApplication) (void *);

// S'applique sur le premier argument avec le second comme paramètre
typedef void (*ParameterizedListApplication) (void *, void *);

List listCreate();
ListLink listLinkCreate(void * data);
void linkConnect(ListLink * a, ListLink * b);
void listAdd(List * list, ListLink * new_link);
void listLinkDetach(ListLink * link);
void listClear(List * list);
void listApplyAll(List list, ListApplication application);
void listParameterizedApplyAll(List list, ParameterizedListApplication application,
				  void * parameters);

void mainListTest();

#endif
