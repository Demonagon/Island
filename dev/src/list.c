#include "list.h"
#include <stddef.h>
#include <stdio.h>

List listCreate() {
	return (List) {.data = NULL, .previous = NULL, .next = NULL};
}

ListLink listLinkCreate(void * data) {
	return (ListLink) {.data = data, .previous = NULL, .next = NULL};
}

void linkConnect(ListLink * a, ListLink * b) {
	if( a ) a->next     = b;
	if( b ) b->previous = a;
}

void listAdd(List * list, ListLink * new_link) {
	if(! new_link || ! list) return;

	// Dans le cas où le chaînon serait déjà dans la liste, pour éviter une 
	// boucle
	listLinkDetach(new_link);

	linkConnect(new_link, list->next);
	linkConnect(list, new_link);
}

void listLinkDetach(ListLink * link) {
	if(! link) return;

	linkConnect(link->previous, link->next);

	link->previous = NULL;
	link->next = NULL;
}

void listClear(List * list) {
	if(list)
		linkConnect(NULL, list->next);
	linkConnect(list, NULL);
}

void listApplyAll(List list, ListApplication application) {
	ListLink * current_link = list.next;

	while(current_link) {
		application(current_link->data);
		current_link = current_link->next;
	}
}

void listParameterizedApplyAll(List list,
		ParameterizedListApplication application, void * parameters) {
	ListLink * current_link = list.next;

	while(current_link) {
		application(current_link->data, parameters);
		current_link = current_link->next;
	}
}

/*************************** TEST *******************************/

void mainListTestPrintApplication(void * p) {
	int * value = p;
	printf("Value = %d\n", *value);
}

void mainListTest() {
	int a = -40, b = +20, c = 123454;
	List list = listCreate();
	ListLink
		l_a = listLinkCreate(&a),
		l_b = listLinkCreate(&b),
		l_c = listLinkCreate(&c);

	listAdd(&list, &l_a);
	listAdd(&list, &l_b);
	listAdd(&list, &l_c);

	listApplyAll(list, mainListTestPrintApplication);
	printf("\n");

	listLinkDetach(&l_b);

	listApplyAll(list, mainListTestPrintApplication);
	printf("\n");

	listLinkDetach(&l_a);

	listApplyAll(list, mainListTestPrintApplication);
	printf("\n");

	listAdd(&list, &l_b);
	listAdd(&list, &l_a);

	listApplyAll(list, mainListTestPrintApplication);
	printf("\n");
}
