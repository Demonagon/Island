#include "gl_graphics.h"
#include <stdio.h>

void glArtistDisplayFunc(void)
{
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );
	
	glPushMatrix();
	glMultMatrixd(mlTbGetTransformation());

	glPopMatrix();
	glutSwapBuffers();
}

void glArtistReshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0, (double)winX/(double)winY, 0.1, 50000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(20.00, +20.00, +20.00, +0.00, +0.00, +0.00, +0.00, +1.00, +0.00);
	

	mlTbInit(width, height);
	glutPostRedisplay();
}

void glArtistKeyboardFunc(unsigned char key, int x, int y)
{
	printf("Key %d pressed\n", (int) key);
    glutPostRedisplay();
}

void glArtistSpecialFunc(int key, int x, int y)
{ 	
	printf("Directionnal key %d pressed\n", (int) key);
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

GlGraphicsArtist glArtistCreate(int * argc, char ** argv, int width, int height)
{
	glutInit (argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	/* window creation */
	glutInitWindowSize (width, height);
	glutInitWindowPosition (0, 0);
	glutCreateWindow (argv [0]);

	/* designation des fonctions liees aux interruptions */
	glutDisplayFunc(glArtistDisplayFunc);
	glutReshapeFunc(glArtistReshapeFunc);
	glutKeyboardFunc(glArtistKeyboardFunc);
	glutSpecialFunc(glArtistSpecialFunc);
	glutMouseFunc(glArtistMouseFunc);
	glutMotionFunc(glArtistMotionFunc);

	return (GlGraphicsArtist) {.window_width = width, .window_height = height};
}

GraphicalObject glArtistFactory (void * graphical_data, GameObject * object) {
	//GlGraphicsArtist * artist = graphical_data;

	//TODO
	return graphicalObjectCreate(
				NULL,
				NULL);
}
