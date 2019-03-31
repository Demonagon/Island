#include "core/graphics.h"

GraphicalObject graphicalObjectCreate(
				GraphicUpdateCallback call_back,
				void * data) {
	return (GraphicalObject) {
		.update_call_back = call_back,
		.graphical_data = data
	};
}
