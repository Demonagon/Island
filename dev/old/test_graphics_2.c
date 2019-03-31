#include "test_graphics_2.h"
#include "util_math.h"
#include "global.h"
#include <stdlib.h>
#include <stdio.h>

TestGraphicsList TEST_GRAPHICS_2_LIST;

char getObjectCharacter(GameObject * object) {
	switch(object->type) {
		case TEST_OBJECT: return 'O';
		case TREE: return 'T';
		case BIRD: return 'b';
		default : return '?';
	}
}

struct CharacterLocationStruct {
	char * c;
	int x;
	int y;
	int w;
	int h;
	double rw;
	double rh;
};

char objectHasPosition(GameObject * object) {
	return ( object->type == TREE ||
			 object->type == BIRD ||
			 object->type == TEST_OBJECT );
}

Complex objectGetPosition(GameObject * object) {
	if( object->type == TREE )
		return object->data.tree.position;
	if( object->type == BIRD )
		return object->data.bird.position;
	if( object->type == TEST_OBJECT )
		return object->data.test_object.position;
	return complexCreate(-1, -1);
}

char isPositionInCharacter(Complex pos, struct CharacterLocationStruct * s) {
	double nx = ( pos.a / s->rw ) * s->w;
	double ny = ( pos.b / s->rh ) * s->h;
	return ( s->x <= nx && nx < s->x + 1 ) &&
		   ( s->y <= ny && ny < s->y + 1 );
}

void testListApplication(void * data, void * parameter) {
	struct CharacterLocationStruct * s = parameter;

	GameObject * object = data;

	if( ! objectHasPosition(object) ) return;

	Complex pos = objectGetPosition(object);

	if( ! isPositionInCharacter(pos, s) ) return;

	char c = getObjectCharacter(object);

	(*s->c) = (*s->c) == ' ' ? c : c > (*s->c) ? c : (*s->c);
}

char getCharacterAtLocation(TestGraphicsList * list, int x, int y) {
	char value = ' ';

	struct CharacterLocationStruct s;
	s.c = &value;
	s.x = x;
	s.y = y;
	s.w = list->width;
	s.h = list->height;
	s.rw = list->real_width;
	s.rh = list->real_height;
	
	listParameterizedApplyAll(
		list->graphic_list,
		testListApplication,
		&s
	);

	return value;
}

void printList(TestGraphicsList * list) {
	printf("╔");
	for(int x = 0; x < list->width; x++)
		printf("═");
	printf("╗\n");
	for(int y = list->height - 1; y >= 0; y--) {
		printf("║");
		for(int x = 0; x < list->width; x++)
			printf("%c", getCharacterAtLocation(list, x, y));
		printf("║\n");
	}
	printf("╚");
	for(int x = 0; x < list->width; x++)
		printf("═");
	printf("╝\n");

}

char objectIsPrintable(GameObject * object) {
	if( object->type == TREE )
		return object->data.tree.state != TREE_DEAD &&
				object->data.tree.state != TREE_INITIAL;
	return ( object->type == BIRD || object->type == TEST_OBJECT );
}

void testGraphic2UpdateCallback(GameObjectListLink * object) {
	TestGraphics2Object * go = object->object.graphics.graphical_data;
	if( objectIsPrintable(& object->object) )
		listAdd(& go->list->graphic_list, & go->graphical_link);
	else
		listLinkDetach(& go->graphical_link);
}

GraphicalObject testGraphics2Factory(void * data, GameObject * object) {
	GraphicalObject o = (GraphicalObject) {
		.update_call_back = testGraphic2UpdateCallback,
		.graphical_data = malloc( sizeof(TestGraphics2Object) )
	};

	((TestGraphics2Object *) o.graphical_data)->list = data;
	((TestGraphics2Object *) o.graphical_data)->graphical_link = listLinkCreate(object);

	return o;
}

void testGraphics2GlobalInit() {
	TEST_GRAPHICS_2_LIST = (TestGraphicsList) {
		.graphic_list = listCreate(),
		.width = 100,
		.height = 50,
		.real_width = 100,
		.real_height = 100
	};

	globalInit(&TEST_GRAPHICS_2_LIST, testGraphics2Factory);
}
