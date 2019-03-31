#ifndef GL_GRAPHICS__H
#define GL_GRAPHICS__H
#include "core/graphics.h"
#include "core/object_list.h"

#include "ml.h"

#if defined(__APPLE__) && !defined(__APPLE_X11)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define PI (3.1415926)
#define PIO2 (1.57079632679)
#define PIO4 (0.78539816339)
#define PIPIO3 (2.09439510239)
#define PIO6 (0.52359877559)

#define GL_MAX_OBJECTS 1000

typedef enum GlGraphicsObjectType {
	GL_OBJECT_NONE,
	GL_OBJECT_CIRCLE,
	GL_OBJECT_SQUARE,
	GL_OBJECT_TRIANGLE,
	GL_OBJECT_ARROW
} GlGraphicsObjectType;

typedef struct GlGraphicsObjectProfile {
	GlGraphicsObjectType type;
	double x, y;
	double size;
	double angle;
	float color[3];
} GlGraphicsObjectProfile;

GlGraphicsObjectProfile glGraphicsProfileCreate(
		GlGraphicsObjectType type,
		double x, double y, double size, double angle,
		float color_r, float color_g, float color_b
);

void glGraphicsRenderCircle(GlGraphicsObjectProfile * profile);
void glGraphicsRenderSquare(GlGraphicsObjectProfile * profile);
void glGraphicsRenderTriangle(GlGraphicsObjectProfile * profile);
void glGraphicsRenderArrow(GlGraphicsObjectProfile * profile);

void glGraphicsRenderProfile(GlGraphicsObjectProfile * profile);

typedef struct GlGraphicsArtist {
	GlGraphicsObjectProfile profile[GL_MAX_OBJECTS];
	int max_index;

	// Les objects existent dans un système de coordonnées, et la fenêtre
	// ne fait qu'observer le monde visible selon les données çi-dessous
	int window_width;
	int window_height;
	int window_x_center;
	int window_y_center;

	unsigned int frame_time;
} GlGraphicsArtist;

GlGraphicsArtist openGLArtist;

/**
Fonction qui permet de faire un appel à la fonction glVertex2d en prenant
compte le fait que la caméra se place à un certain endroit.
**/
//glArtistVertex(double x, double y);

void glArtistInit(int * argc, char ** argv, GlGraphicsArtist * artist, int width, int height);
void glArtistRedraw();

int glArtistAddProfile(GlGraphicsArtist * artist, GlGraphicsObjectProfile p);
void glArtistRemoveProfile(GlGraphicsArtist * artist, int index);
GlGraphicsObjectProfile * glArtistGetProfile(GlGraphicsArtist * artist, int index);

GraphicalObject glArtistFactory (void * graphical_data, GameObject * object);

#endif
