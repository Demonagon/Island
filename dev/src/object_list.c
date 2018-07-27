#include "object_list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

GameObjectListLink * objectListLinkAllocate(GameObject object) {
	GameObjectListLink * link = malloc( sizeof( GameObjectListLink ) );
	(*link) = objectListLinkCreate(object);
	return link;
}

void objectListLinkDestroy(GameObjectListLink * link) {
	objectListLinkDetach(link);
	free(link);
}

GameObjectList objectListCreate() {
	GameObjectList list;
	list.previous = NULL;
	list.next = NULL;
	return list;
}

GameObjectListLink objectListLinkCreate(GameObject object) {
	return (GameObjectListLink) {
		.object = object,
		.previous = NULL,
		.next = NULL
	};
}

void objectLinkConnect(GameObjectListLink * a, GameObjectListLink * b) {
	if( a ) a->next     = b;
	if( b ) b->previous = a;
}

void objectListAdd(GameObjectList * objectList, GameObjectListLink * new_link) {
	if(! new_link || ! objectList) return;

	// Dans le cas où le chaînon serait déjà dans la objectListe, pour éviter une 
	// boucle
	objectListLinkDetach(new_link);

	objectLinkConnect(new_link, objectList->next);
	objectLinkConnect(objectList, new_link);
}

void objectListLinkDetach(GameObjectListLink * link) {
	if(! link) return;

	objectLinkConnect(link->previous, link->next);

	link->previous = NULL;
	link->next = NULL;
}

void objectListClear(GameObjectList * objectList) {
	if( ! objectList ) return;
	
	GameObjectListLink * current_link = objectList->next;

	while(current_link) {
		GameObjectListLink * next_link = current_link->next;
		objectListLinkDestroy(current_link);
		current_link = next_link;
	}
}

void objectListApplyAll(GameObjectList objectList, GameObjectListApplication application) {
	GameObjectListLink * current_link = objectList.next;

	while(current_link) {
		GameObjectListLink * next_link = current_link->next;
		application(&current_link->object);
		current_link = next_link;
	}
}

void objectListParameterizedApplyAll(GameObjectList objectList,
		ParameterizedGameObjectListApplication application, void * parameters) {
	GameObjectListLink * current_link = objectList.next;

	while(current_link) {
		GameObjectListLink * next_link = current_link->next;
		application(&current_link->object, parameters);
		current_link = next_link;
	}
}

/*************************** TEST *******************************/

void mainGameObjectListTestPrintApplication(GameObject * p) {
	/* TODO place some test message display about the object */
	printf("my number = %d\n", p->type);
}

void mainGameObjectListTest() {
	GameObject t1, t2, t3;
	GameObjectList list = objectListCreate();
	GameObjectListLink
		l_a = objectListLinkCreate(t1),
		l_b = objectListLinkCreate(t2),
		l_c = objectListLinkCreate(t3);

	objectListAdd(&list, &l_a);
	objectListAdd(&list, &l_b);
	objectListAdd(&list, &l_c);

	objectListApplyAll(list, mainGameObjectListTestPrintApplication);
	printf("\n");

	objectListLinkDetach(&l_b);

	objectListApplyAll(list, mainGameObjectListTestPrintApplication);
	printf("\n");

	objectListLinkDetach(&l_a);

	objectListApplyAll(list, mainGameObjectListTestPrintApplication);
	printf("\n");

	objectListAdd(&list, &l_b);
	objectListAdd(&list, &l_a);

	objectListApplyAll(list, mainGameObjectListTestPrintApplication);
	printf("\n");
}
