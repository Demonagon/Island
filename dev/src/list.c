#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

List listCreate() {
	return (List) {.data = NULL, .previous = NULL, .next = NULL};
}

ListLink listLinkCreate(void * data) {
	return (ListLink) {.data = data, .previous = NULL, .next = NULL};
}

int indent = 0;
void printIndent() {
	for(int k = 0; k < indent; k++)
		printf("\t");
}

void linkConnect(ListLink * a, ListLink * b) {

	printIndent();
	printf("%p >--< %p\n", a, b);

	if( a && a == b ) {
		void * aa = a;
		fprintf(stderr, "(%s, line %d) => A list link cannot be connected to",
			__FILE__, __LINE__);
		fprintf(stderr, " itself. Link address : %p\n", aa);
		abort();
	}
	if( a ) a->next     = b;
	if( b ) b->previous = a;
}

void listAdd(List * list, ListLink * new_link) {
	if(! new_link || ! list) return;

	printIndent();
	printf("Ajout de %p à liste %p\n", new_link, list);
	indent++;

	// Dans le cas où le chaînon serait déjà dans la liste, pour éviter une 
	// boucle
	listLinkDetach(new_link);

	linkConnect(new_link, list->next);
	linkConnect(list, new_link);
	indent--;

}

void listLinkDetach(ListLink * link) {
	if(! link) return;

	printIndent();
	printf("Détachement de %p\n", link);
	indent++;

	linkConnect(link->previous, link->next);

	link->previous = NULL;
	link->next = NULL;
	indent--;
}

void listLinkUpdateMemoryLocation(ListLink * link) {
	printIndent();
	printf("Mise à jour de %p\n", link);
	indent++;
	linkConnect(link->previous, link);
	linkConnect(link, link->next);
	indent--;
}

void listClear(List * list) {
	ListLink * current_link = list->next;

	printIndent();
	printf("Clearing list %p\n", list);
	indent++;

	while(current_link) {
		ListLink * next_link = current_link->next;
		listLinkDetach(current_link);
		current_link = next_link;
	}
	//if(list)
	//	linkConnect(NULL, list->next);
	linkConnect(list, NULL);
	indent--;
}

void listApplyAll(List list, ListApplication application) {
	ListLink * current_link = list.next;
	
	while(current_link) {
		ListLink * next_link = current_link->next;
		application(current_link->data);
		current_link = next_link;
	}
}

void listParameterizedApplyAll(List list,
		ParameterizedListApplication application, void * parameters) {
	ListLink * current_link = list.next;

	while(current_link) {
		ListLink * next_link = current_link->next;
		application(current_link->data, parameters);
		current_link = next_link;
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
