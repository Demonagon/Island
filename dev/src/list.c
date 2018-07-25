#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef LIST_DEBUG_MODE
	#include "util_debug.h"
#endif

List listCreate() {
	return (List) {.data = NULL, .previous = NULL,
				   .next = NULL, .metadata.last_link = NULL};
}

ListLink listLinkCreate(void * data) {
	return (ListLink) {.data = data, .previous = NULL, 
					   .next = NULL, .metadata.list = NULL};
}

void linkConnect(ListLink * a, ListLink * b) {
	void * p;
	if( a && a == b ) {
		p = a;
		fprintf(stderr, "(%s, line %d) => A list link cannot be connected to",
			__FILE__, __LINE__);
		fprintf(stderr, " itself. Link address : %p\n", p);
		abort();
	}
	if( a ) a->next     = b;
	if( b ) b->previous = a;

	#ifdef LIST_DEBUG_MODE
		printIndent();
		p = a;
		printf("%p >--< (", a, p);
		p = b ? b->previous : NULL;
		printf("%p) ", p);
		p = b;
		printf("%p\n", p);
	#endif
}

void listAdd(List * list, ListLink * new_link) {
	if(! new_link || ! list) return;

	#ifdef LIST_DEBUG_MODE
		printIndent();
		printf("Ajout de %p à liste ", new_link);
		printf("%p\n", list);
		indent++;
	#endif

	// Dans le cas où le chaînon serait déjà dans la liste, pour éviter une 
	// boucle
	listLinkDetach(new_link);

	//linkConnect(new_link, list->next);
	//linkConnect(list, new_link);
	if( ! list->metadata.last_link )
		linkConnect(list, new_link);
	else
		linkConnect(list->metadata.last_link, new_link);
	list->metadata.last_link = new_link;
	new_link->metadata.list = list;

	#ifdef LIST_DEBUG_MODE
		indent--;
	#endif

}

void listLinkDetach(ListLink * link) {
	if(! link) return;

	#ifdef LIST_DEBUG_MODE
		printIndent();
		void * p = link;
		printf("Détachement de %p\n", p);
		indent++;
	#endif

	// Dans le cas justifité, on met à jour la liste qui contient le chaînon
	// en mettant à jour son pointeur vers le dernier chaînon.
	if( link->metadata.list && link->metadata.list->metadata.last_link == link )
		link->metadata.list->metadata.last_link = 
			link->previous == link->metadata.list ? NULL : link->previous;

	linkConnect(link->previous, link->next);

	link->previous = NULL;
	link->next = NULL;
	link->metadata.list = NULL;

	#ifdef LIST_DEBUG_MODE
		indent--;
	#endif
}

void listLinkUpdateMemoryLocation(ListLink * link,
								  ListLink * erased_link,
								  void * data) {
	#ifdef LIST_DEBUG_MODE
		printIndent();
		printf("Mise à jour de %p\n", link);
		indent++;
	#endif

	List * old_link =
		link->previous ? link->previous->next :
		link->next ? link->next->previous : NULL;

	link->data = data;


	// Gestion du cas rare où le remplaçant est demandé de se placer juste à
	// coté de son ancienne position, ce qui entraînerait une erreur
	if(link == link->next)
		link->next = erased_link->next;
	if(link == link->previous)
		link->previous = erased_link->previous;

	linkConnect(link->previous, link);
	linkConnect(link, link->next);

	#ifdef LIST_DEBUG_MODE
		indent--;
	#endif

	if( ! link->previous )
		return;

	if( ! link->previous->previous )
		link->metadata.list = link->previous;
	else
		link->metadata.list = link->previous->metadata.list;

	if( ! link->next )
		link->metadata.list->metadata.last_link = link;

	listLinkDetach(old_link);
}

void listClear(List * list) {
	ListLink * current_link = list->next;

	#ifdef LIST_DEBUG_MODE
		printIndent();
		printf("Clearing list %p\n", list);
		indent++;
	#endif

	while(current_link) {
		ListLink * next_link = current_link->next;
		listLinkDetach(current_link);
		current_link = next_link;
	}
	linkConnect(list, NULL);

	#ifdef LIST_DEBUG_MODE
		indent--;
	#endif
}

void listApplyAll(List list, ListApplication application) {
	ListLink * current_link = list.next;
	
	//ListLink * previous_link = &list;

	while(current_link) {
		ListLink * next_link = current_link->next;
		ListLink * next_next_link = next_link ? next_link->next : NULL;
		application(current_link->data);
		/*if(previous_link == next_link) {
			printf("BOUCLE INFINIE : %p (%p) <-> (%p) %p (%p) <-> (%p) %p\n",
			previous_link, previous_link->next, current_link->previous, current_link, current_link->next, next_link->previous, next_link);
			abort();
		}*/
		//previous_link = current_link;

		//Cas particuler où l'avant dernier chaînon se détruit, se voit
		//remplacé par le dernier chaînon, qui n'est donc plus valide
		//mais stocké dans next_link.
		/*if( next_link &&
			! current_link->previous && ! current_link->next &&
			! next_link->previous    && ! next_link->next )
			next_link = next_next_link;*/

		current_link = next_link;
	}
}

void listParameterizedApplyAll(List list,
		ParameterizedListApplication application, void * parameters) {
	ListLink * current_link = list.next;

	while(current_link) {
		ListLink * next_link = current_link->next;
		ListLink * next_next_link = next_link ? next_link->next : NULL;
		application(current_link->data, parameters);

		//Cas particuler où l'avant dernier chaînon se détruit, se voit
		//remplacé par le dernier chaînon, qui n'est donc plus valide
		//mais stocké dans next_link.
		/*if( ! current_link->previous && ! current_link->next &&
			! next_link->previous    && ! next_link->next )
			next_link = next_next_link;*/

		current_link = next_link;
	}
}

void listPrint(List * list) {
	void * p = list;
	ListLink * current_link = list->next;
	printf("L %p", p);
	p = list->metadata.last_link;
	printf("[%p] <-> ", p);

	while(current_link) {
		p = current_link->previous;
		printf("(%p) ", p);
		p = current_link;
		printf("%p", p);
		p = current_link->metadata.list;
		printf("[%p] <-> ", p);
		current_link = current_link->next;
	}
	printf("(/)\n");
}

void listLinkPrintList(ListLink * list) {
	if( ! list ) {
		printf("Not a link\n");
	}
	while(list->previous) list = list->previous;

	listPrint(list);
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
