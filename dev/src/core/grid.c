#include "core/grid.h"
#include <stdio.h>

/*********************************** GRID EVENT *******************************/

GridEvent gridEventCreate(GridEventType type, GridEventData data,
						  Complex position, double range) {
	return (GridEvent) {.type = type, .data = data,
						.position = position, .range = range};
}

/***************************** GRID BEACON ************************************/

GridBeacon gridBeaconCreateEmpty() {
	return (GridBeacon) {
		.data = NULL,
		.position = NULL,
		.event_handler = NULL,
		.list_link = listCreate()
	};
}

// Initialise le chaînon pour contenir l'adresse précise de la balise.
void gridBeaconInit(GridBeacon * beacon, void * data,
					Complex * position, EventHandler event_handler) {
	beacon->data = data;
	beacon->position = position;
	beacon->event_handler = event_handler;
	beacon->list_link = listCreate();
	beacon->list_link.data = beacon;
}

void gridBeaconRemove(GridBeacon * beacon) {
	listLinkDetach( & beacon->list_link );
}

void gridBeaconReceiveEvent(GridBeacon * beacon, GridEvent event) {
	if( beacon->event_handler )
		beacon->event_handler(beacon->data, event);
}

/****************************** EVENT GRID ************************************/

List * eventGridGetList(EventGrid * grid, int x, int y) {
	return & grid->beacon_lists[x * GRID_HEIGHT + y];
}

void eventGridInit(EventGrid * grid, double total_width,
									 double total_height) {
	grid->total_width = total_width;
	grid->total_height = total_height;
	for(int x = 0; x < GRID_WIDTH; x++)
		for(int y = 0; y < GRID_HEIGHT; y++)
			(*eventGridGetList(grid, x, y)) = listCreate();
}

char eventGridIsPointIn(EventGrid * grid, Complex point) {
	return (point.a >= 0 && point.b >= 0 &&
			point.a <= grid->total_width && point.b <= grid->total_height);
}

void eventGridPlaceBeacon(EventGrid * grid, GridBeacon * beacon) {
	int cell_x = (int) (beacon->position->a * GRID_WIDTH /  grid->total_width );
	int cell_y = (int) (beacon->position->b * GRID_HEIGHT / grid->total_height);

	if(cell_x < 0 || cell_x >= GRID_WIDTH ||
	   cell_y < 0 || cell_y >= GRID_HEIGHT) return;

	listAdd(eventGridGetList(grid, cell_x, cell_y), & beacon->list_link);
}

void eventGridBeaconReplacingApplication(void * data, void * parameter) {
	GridBeacon * beacon = data;
	EventGrid * grid = parameter;
	eventGridPlaceBeacon(grid, beacon);
}

void eventGridReplaceAllBeacons(EventGrid * grid) {
	for(int x = 0; x < GRID_WIDTH; x++)
		for(int y = 0; y < GRID_HEIGHT; y++) {
			List * list = eventGridGetList(grid, x, y);
			listParameterizedApplyAll(*list,
				eventGridBeaconReplacingApplication, grid);
		}
}

void eventGridResize(EventGrid * grid, double total_width, double total_height)
{
	grid->total_width = total_width;
	grid->total_height = total_height;
	eventGridReplaceAllBeacons(grid);
}

void eventGridEventCallApplication(void * data, void * parameter) {
	GridBeacon * beacon = data;
	GridEvent * event = parameter;

	if( ! isPointInCircle(*beacon->position, event->position, event->range) )
		return;

	switch( event->type ) {
		case _GRID_CALL_YOURSELF_EVENT:
			printf("I'm %p !\n", data);
			break;
		default :
			gridBeaconReceiveEvent(beacon, *event);
			break;
	}

	return;
}

char eventGridIsCircleInCell(EventGrid * grid, int x, int y,
						Complex center, double radius) {
	double cell_width = grid->total_width / GRID_WIDTH;
	double cell_height = grid->total_height / GRID_HEIGHT;


	// checking if the center is in the cell
	double cell_x = x * cell_width;
	double cell_y = y * cell_height;

	if( center.a >= cell_x && center.a <= cell_x + cell_width &&
		center.b >= cell_y && center.b <= cell_y + cell_height )
		return 1;

	// check if one of the borders is in range of the center
	if( center.a < cell_x && center.a + radius >= cell_x ) return 1;
	if( center.a > cell_x + cell_width && center.a - radius <=
			cell_x + cell_width ) return 1;
	if( center.b < cell_y && center.b + radius >= cell_y ) return 1;
	if( center.b > cell_y + cell_height && center.b - radius <=
			cell_y + cell_height ) return 1;

	// check if one of the corner is in range of the center
	Complex down_left_corner   = 
		complexCreate( (x + 0) * cell_width, (y + 0) * cell_height );
	Complex down_right_corner  = 
		complexCreate( (x + 1) * cell_width, (y + 0) * cell_height );
	Complex upper_left_corner  = 
		complexCreate( (x + 0) * cell_width, (y + 1) * cell_height );
	Complex upper_right_corner = 
		complexCreate( (x + 1) * cell_width, (y + 1) * cell_height );


	if( isPointInCircle( upper_left_corner, center, radius) ) return 1;
	if( isPointInCircle(upper_right_corner, center, radius) ) return 1;
	if( isPointInCircle(  down_left_corner, center, radius) ) return 1;
	if( isPointInCircle( down_right_corner, center, radius) ) return 1;

	return 0;
}

void eventGridBroadcast(EventGrid * grid, GridEvent event)  {
	for(int x = 0; x < GRID_WIDTH; x++)
		for(int y = 0; y < GRID_HEIGHT; y++) {
			List * list = eventGridGetList(grid, x, y);
			if( eventGridIsCircleInCell(
				grid, 
				x, y, 
				event.position, event.range) )
				listParameterizedApplyAll(*list, eventGridEventCallApplication,
				  						  &event);
		}				
}

void eventGridDeclareBeacon(EventGrid * grid, GridBeacon * beacon, double range, int object_type) {
	GridEventData event_data;
	GridDeclarationEventData data = (GridDeclarationEventData) {
		.data = beacon->data,
		.type = object_type
	};
	event_data.declarating_object = data;

	eventGridBroadcast(
		grid,
		gridEventCreate(
			OBJECT_SELF_DECLARATION_EVENT,
			event_data,
			*(beacon->position),
			range
		)
	);			
}

/******************************** TEST ****************************************/

void mainEventGridTest() {
	EventGrid grid;
	Complex pos1 = complexCreate(4, 4), pos2 = complexCreate(6, 6),
					pos3 = complexCreate(90, 90);
	GridBeacon b1, b2, b3;

	eventGridInit(&grid, 100, 100);

	gridBeaconInit(&b1, NULL, &pos1, NULL);
	gridBeaconInit(&b2, NULL, &pos2, NULL);
	gridBeaconInit(&b3, NULL, &pos3, NULL);

	eventGridPlaceBeacon(&grid, &b1);
	eventGridPlaceBeacon(&grid, &b2);
	eventGridPlaceBeacon(&grid, &b3);

	GridEventData data;
	Complex event_pos = complexCreate(50, 50);

	printf("b1 = %p\n", (void*) &b1);
	printf("b2 = %p\n", (void*) &b2);
	printf("b3 = %p\n", (void*) &b3);

	eventGridBroadcast(&grid,
		gridEventCreate(_GRID_CALL_YOURSELF_EVENT, data, event_pos, 60));

	gridBeaconRemove(&b3);

	printf("----------------------------\n");

	eventGridBroadcast(&grid,
		gridEventCreate(_GRID_CALL_YOURSELF_EVENT, data, event_pos, 70));

	gridBeaconRemove(&b1);
	gridBeaconRemove(&b2);
}
