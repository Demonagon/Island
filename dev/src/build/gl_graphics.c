#include "build/gl_graphics.h"
#include <stdio.h>
#include <math.h>

GlGraphicsArtist openGLArtist;

GlGraphicsObjectProfile glGraphicsProfileCreate(
		GlGraphicsObjectType type,
		double x, double y, double size, double angle,
		float color_r, float color_g, float color_b
) {
	return (GlGraphicsObjectProfile) {
		.type = type, .x = x, .y = y, .size = size, .angle = angle,
		.color = {color_r, color_g, color_b} };
}

void glGraphicsRenderCircle(GlGraphicsObjectProfile * profile) {
	double angle = profile->angle;

	glBegin(GL_POLYGON);
		glColor3f(profile->color[0], profile->color[1], profile->color[2]);
		for(int k = 0; k < 12; k++) {
			glVertex2d(profile->x + cos(angle) * profile->size,
					   profile->y + sin(angle) * profile->size);
			angle += PIO6;
		}
	glEnd();
}

void glGraphicsRenderSquare(GlGraphicsObjectProfile * profile) {
	double sizeo2 = profile->size/2;
	glBegin(GL_QUADS);
		glColor3f(profile->color[0], profile->color[1], profile->color[2]);
		glVertex2d(profile->x - sizeo2, profile->y - sizeo2);
		glVertex2d(profile->x - sizeo2, profile->y + sizeo2);
		glVertex2d(profile->x + sizeo2, profile->y + sizeo2);
		glVertex2d(profile->x + sizeo2, profile->y - sizeo2);
	glEnd();
}

void glGraphicsRenderTriangle(GlGraphicsObjectProfile * profile) {
	double angle = profile->angle + PIO2;
	glBegin(GL_TRIANGLES);
		glColor3f(profile->color[0], profile->color[1], profile->color[2]);
		for(int k = 0; k < 3; k++) {
			glVertex2d(profile->x + cos(angle) * profile->size,
					   profile->y + sin(angle) * profile->size);
			angle += PIPIO3;
		}
	glEnd();
}

void glGraphicsRenderArrow(GlGraphicsObjectProfile * profile) {
	printf("J'ai pas de flèches pour le moment\n");
}

void glGraphicsRenderProfile(GlGraphicsObjectProfile * profile) {
	if(! profile) return;

	switch(profile->type) {
		case GL_OBJECT_CIRCLE:
			glGraphicsRenderCircle(profile);
			break;
		case GL_OBJECT_SQUARE:
			glGraphicsRenderSquare(profile);
			break;
		case GL_OBJECT_TRIANGLE:
			glGraphicsRenderTriangle(profile);
			break;
		case GL_OBJECT_ARROW:
			glGraphicsRenderArrow(profile);
			break;
		default : break;
	}
}

void glArtistDisplayFunc(void)
{
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );
	
	//glPushMatrix();
	//glMultMatrixd(mlTbGetTransformation());

	for(int k = 0; k <= openGLArtist.max_index; k++)
		glGraphicsRenderProfile(openGLArtist.profile + k);

	
	/*glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-50, -50);
		glVertex2f(-50, +50);
		glVertex2f(+50, +50);
		glVertex2f(+50, -50);
	glEnd();*/

	//glPopMatrix();
	//glutSwapBuffers();
	glFlush();
}

void glArtistTimerFunc(int value)
{
    glutPostRedisplay();
	glutTimerFunc(openGLArtist.frame_time, glArtistTimerFunc, 0);
}

void glArtistReshapeFunc(int width, int height)
{
	openGLArtist.window_width = width;
	openGLArtist.window_height = height;

	glViewport(0, 0, width, height);
	
	glLoadIdentity();
	glScaled(1.0/openGLArtist.window_width, 1.0/openGLArtist.window_height, 1);
	glTranslated(openGLArtist.window_x_center, openGLArtist.window_y_center, 0);
	//glMatrixMode(GL_PROJECTION);
	//gluPerspective(60.0, (double)winX/(double)winY, 0.1, 50000.0);
	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(20.00, +20.00, +20.00, +0.00, +0.00, +0.00, +0.00, +1.00, +0.00);
	//glTranslated(openGLArtist.window_width / 2.0 + openGLArtist.window_x_center,
	//			 openGLArtist.window_height/ 2.0 + openGLArtist.window_y_center,
	//			 0);

	//mlTbInit(width, height);
	//glutPostRedisplay();
}

void glArtistKeyboardFunc(unsigned char key, int x, int y)
{
	printf("Key %d pressed\n", (int) key);
    glutPostRedisplay();
}

void glArtistSpecialFunc(int key, int x, int y)
{ 	
	printf("Directionnal key %d pressed\n", (int) key);
	
	switch(key) {
		case 100 :
			openGLArtist.window_x_center += 20;
			break;
		case 101 :
			openGLArtist.window_y_center -= 20;
			break;
		case 102 :
			openGLArtist.window_x_center -= 20;
			break;
		case 103 :
			openGLArtist.window_y_center += 20;
			break;
		default : return;
	}

	
	glLoadIdentity();
	glScaled(1.0/openGLArtist.window_width, 1.0/openGLArtist.window_height, 1);
	glTranslated(openGLArtist.window_x_center, openGLArtist.window_y_center, 0);

	glutPostRedisplay();
}

void glArtistMouseFunc(int button, int state, int x, int y)
{ 
	printf("Mouse button press %d %d %d %d\n", button, state, x, y);
}

/* Callback OpenGL de gestion de drag */
void glArtistMotionFunc(int x, int y)
{
	//if(mouseLeftDown == 1)
	//	mlTbMotion(x, y);

	glutPostRedisplay();
}

void glArtistInit(int * argc, char ** argv, GlGraphicsArtist * artist, int width, int height)
{
	glutInit (argc, argv);
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
    //          	  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	//glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	/* window creation */
	glutInitWindowSize (width, height);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv [0]);

	/* designation des fonctions liees aux interruptions */
	glutDisplayFunc(glArtistDisplayFunc);
	glutTimerFunc(openGLArtist.frame_time, glArtistTimerFunc, 0);
	glutReshapeFunc(glArtistReshapeFunc);
	glutKeyboardFunc(glArtistKeyboardFunc);
	glutSpecialFunc(glArtistSpecialFunc);
	glutMouseFunc(glArtistMouseFunc);
	glutMotionFunc(glArtistMotionFunc);

	for(int k = 0; k < GL_MAX_OBJECTS; k++)
		artist->profile[k].type = GL_OBJECT_NONE;

	artist->max_index = 0;

	artist->window_width = width;
	artist->window_height = height;
	artist->window_x_center = 0;
	artist->window_y_center = 0;

	int frames = 60;
	double frame_time = 1.0 / frames;
	artist->frame_time = frame_time * 1000;
}

void glArtistRedraw() {
	glutPostRedisplay();
}

int glArtistAddProfile(GlGraphicsArtist * artist, GlGraphicsObjectProfile p) {
	for(int k = 0; k < GL_MAX_OBJECTS; k++)
		if( artist->profile[k].type == GL_OBJECT_NONE ) {
			artist->profile[k] = p;
			if( artist->max_index < k )
				artist->max_index = k;
			return k;
		}

	printf("Error : OpenGL Artist profile overload\n");
	return -1;
}

void glArtistRemoveProfile(GlGraphicsArtist * artist, int index) {
	if(index < 0 || index >= GL_MAX_OBJECTS) return;

	artist->profile[index].type = GL_OBJECT_NONE;
	if(index == artist->max_index) {
		int new_max_index = index - 1;
		for(; new_max_index >= 0; new_max_index--)
			if(artist->profile[new_max_index].type != GL_OBJECT_NONE)
				break;
		artist->max_index = new_max_index;
	}
}

GlGraphicsObjectProfile * glArtistGetProfile(GlGraphicsArtist * artist, int index) {
	if(index < 0 || index >= GL_MAX_OBJECTS) return NULL;

	return artist->profile + index;
}

void GlGraphicUpdateCallback(GameObjectListLink * link) {
	/*Tree * tree = &link->object.data.tree;
	GlGraphicsObjectProfile * profile = link->object.graphics.graphical_data;

	switch(tree->state) {
		case TREE_INITIAL:
			profile->color[0] = 131 /255.0f;
			profile->color[1] = 188 /255.0f;
			profile->color[2] = 24  /255.0f;
			profile->size = 5; 
			break;
		case TREE_CANCELLED:
			glArtistRemoveProfile(&openGLArtist, profile - openGLArtist.profile);
			break;
		case TREE_GROWING:
			profile->color[0] = 77  /255.0f;
			profile->color[1] = 130 /255.0f;
			profile->color[2] = 9   /255.0f;
			profile->size = 15; break;
		case TREE_REPRODUCTION:
			profile->color[0] = 77  /255.0f;
			profile->color[1] = 130 /255.0f;
			profile->color[2] = 9   /255.0f;
			profile->size = 25; break;
		case TREE_MATURE:
			profile->color[0] = 77  /255.0f;
			profile->color[1] = 130 /255.0f;
			profile->color[2] = 9   /255.0f;
			profile->size = 30; break;
		case TREE_DYING:
			profile->color[0] = 130 /255.0f;
			profile->color[1] = 121 /255.0f;
			profile->color[2] = 8   /255.0f;			
			profile->size = 20; break;
		case TREE_DEAD:
			glArtistRemoveProfile(&openGLArtist, profile - openGLArtist.profile);
			break;
	}*/
}

GraphicalObject glArtistFactory (void * graphical_data, GameObject * object) {
	/*GlGraphicsArtist * artist = graphical_data;

	if( ! object || object->type != TREE )*/
		return graphicalObjectCreate(NULL, NULL);

	/*Tree * tree = &object->data.tree;

	int index = glArtistAddProfile(artist, 
		glGraphicsProfileCreate(GL_OBJECT_TRIANGLE,
			tree->position.a * 10, tree->position.b * 10, 0, 0, 1.0f, 0.0f, 0.0f)
		);

	if( index < 0 )
		return graphicalObjectCreate(NULL, NULL);

	return graphicalObjectCreate(GlGraphicUpdateCallback,
				glArtistGetProfile(artist, index)
		);*/
}
