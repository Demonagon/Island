#ifndef GL_GRAPHICS__H
#define GL_GRAPHICS__H
#include "graphics.h"
#include "object_list.h"

#include "ml.h"

#if defined(__APPLE__) && !defined(__APPLE_X11)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define PI (3.1415926)

typedef struct GlGraphicsArtist {
	int window_width;
	int window_height;
} GlGraphicsArtist;

GlGraphicsArtist glArtistCreate(int * argc, char ** argv, int width, int height);

GraphicalObject glArtistFactory (void * graphical_data, GameObject * object);

#endif
