#ifndef GRID__H
#define GRID__H
#include "util_math.h"
#include "list.h"

/**
* Par Pacôme, le 9 Juillet 2018
* Ce système découpe une partie une large zone rectangulaire en une grille,
* et gère les évènements localisés sur cette grille.
* Chaque objet localisé sur la zone qui souhaite être informé des évènements
* qui s'y déroulent possède une balise, qui est placée dans la case de la grille
* qui contient la position de l'objet. Lorsqu'un évènement est transmis à une
* position avec une certaine portée, le système transmet l'évènement uniquement
* aux balises contenues dans les cellules à la portée de l'évènement, coupant
* ainsi la complexité de la transmission de l'évènement, sous l'hypothèse que
* les objets sont répartis équitablement sur la carte.
* Ce système n'utilise aucune allocation dynamique ni aucun tableau, ni de
* variables globales.
*/

#define GRID_WIDTH  10
#define GRID_HEIGHT 10

/******************************** GRID EVENT **********************************/

/**
* Comme ce projet est développé en C, une union et une énumération sont utilisés
* pour définir ce qu'est un évènement. Lorsqu'un nouvel évènement est rajouté
* au projet il faut l'ajouter ici dans l'union, avec un nom dans l'énumération.
* Notes sur la nomenclature : _GRID_***_EVENT est réservé pour les évènements
* techniques principalements utilisés pour le débug. Tout évènement classique
* doit être appellé GRID_***_EVENT.
*/

/** DESCRIPTION DES TYPES D'ÉVÈNEMENTS
 _GRID_CALL_YOURSELF_EVENT :
	Évènement de débug, qui provoque tout beacon à portée d'écrire un message
	dans le console. Ce morceau de code surplante l'appel à la fonction qui
	gère normalement les évènements (event.event_handler, voir GridBeacon).
*/

typedef enum GridEventType {
	_GRID_CALL_YOURSELF_EVENT,

	/* TODO */
	/** CUSTOM EVENTS HERE **/
	TREE_SPAWNING_COLISION_CHECK_EVENT
	/** ** ** *** ** ** ** **/
} GridEventType;

typedef union GridEventData {
	/* TODO */
	/** CUSTOM DATA HERE **/
	void * spawning_tree; // -> TREE_SPAWNING_COLISION_CHECK_EVENT
	/** ** ** * ** ** ** **/
} GridEventData;

typedef struct GridEvent {
	GridEventType type;
	GridEventData data;
	Complex position;
	double range;
} GridEvent;

GridEvent gridEventCreate(GridEventType type, GridEventData data,
						  Complex position, double range);

/***************************** GRID BEACON ************************************/

/**
* Chaque objet créé pour recevoir un évènement le fait en étant composé d'une
* balise qui se place elle même dans la grille. La position de la balise dans
* la grille doit être mise à jour à chaque déplacement de l'objet.
* Chaque balise possède un pointeur sur fonction qui permet aux objets de
* récupérer les évènements de façon personnalisée.
* La position de chaque balise est un pointeur vers la position de l'objet
* qui lui correspond, afin d'éviter les étapes de mise à jour.
*/

typedef void (*EventHandler)(void *, GridEvent);

typedef struct GridBeacon {
	void * data;
	Complex * position;
	EventHandler event_handler;
	ListLink list_link;
} GridBeacon;

GridBeacon gridBeaconCreateEmpty();

// Initialise le chaînon pour contenir l'adresse précise de la balise.
void gridBeaconInit(GridBeacon * beacon, void * data,
					Complex * position, EventHandler event_handler);

void gridBeaconRemove(GridBeacon * beacon);

void gridBeaconReceiveEvent(GridBeacon * beacon, GridEvent event);

void gridBeaconUpdateMemoryLocation(GridBeacon * beacon,
									GridBeacon * erased_beacon,
									void * data,
									Complex * position);

/****************************** EVENT GRID ************************************/

typedef struct EventGrid {
	List beacon_lists[GRID_WIDTH * GRID_HEIGHT];
	double total_width;
	double total_height;
} EventGrid;

List * eventGridGetList(EventGrid * grid, int x, int y);

void eventGridInit(EventGrid * grid, double total_width,
									 double total_height);

void eventGridPlaceBeacon(EventGrid * grid, GridBeacon * beacon);
void eventGridReplaceAllBeacons(EventGrid * grid);
void eventGridResize(EventGrid * grid, double total_width, double total_height);

void eventGridEventCallApplication(void * data, void * parameter);

char eventGridIsCircleInCell(EventGrid * grid, int x, int y, Complex c, double radius);

void eventGridBroadcast(EventGrid * grid, GridEvent event);

/********************************* TEST ***************************************/

void mainEventGridTest();

#endif
