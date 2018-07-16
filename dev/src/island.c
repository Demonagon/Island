#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "island.h"
#include "ml.h"

#include <math.h>
#if defined(__APPLE__) && !defined(__APPLE_X11)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define PI (3.1415926)





/* Variables globales pour parametrer la scene */
/* ------------------------------------------- */

/* Taille de la fenetre */
int winX = 800;
int winY = 600;

//couleurs objet et axes
float couleurObjet[3] = {1, 1, 1};
float couleurObjetAretes[3] = {1 , 0 , 1};
float couleurAxe[3]= {0 ,1,0};
int rotaM1 = 0;
int rotaM2 = 0;
float transM3 = 0;
int rotaGlob = 0;
int mActu = 1;
int MAXLIGNE = 256;
int mouseLeftDown;

typedef enum{ TEXT_CHROME , TEXT_PONY_MANE6 , TEXT_PONY_RAINBOW , TEXT_SOL }textType;
unsigned int textures[4];

//paramétrisation de la heightmap
int heightmap_largeur = 100; //x
int heightmap_longeur = 100; //y
int heightmap_hauteur_min = 0; //z_min
int heightmap_hauteur_max = 4; //z_max
int** heightmap = NULL;


//matériaux chrome
GLfloat Lnoire [4] = {0.0, 0.0, 0.0, 1.0};
GLfloat matAmbiant [4] = {0.25, 0.25 , 0.25 ,0.25};
GLfloat matDiffuse [4] = {0.4, 0.4, 0.4, 0.4};
GLfloat matSpecular [4] = {0.774597, 0.774597, 0.774597, 0.774597};
GLfloat matShininess [1] = {76.8};

//lumière soit!
GLfloat position [4] = {0.0, 10.0, 10.0, 1.0};
GLfloat Lambiant [4] = {0.2, 0.2, 0.2, 1.0};
GLfloat Lblanche [4] = {1.0, 1.0, 1.0, 1.0};

//tableau des normales (à normaliser LOOOLL)
float N [6][3] = {{1, 0}};


// Position de la camera
//float Zcamera=20., Xcamera=5., Ycamera=5.;
float Zcamera=15, Xcamera=0, Ycamera=3;
// et direction visŽe
float XaxeCamera=0, YaxeCamera=0, ZaxeCamera=-50;


/* Construction de la scene */
/* ------------------------ */

// Un objet simple dans la scene

typedef struct image
{ int larg, haut, rgb; 
	GLubyte *dat;
	GLubyte **pix;
} *Image; 

/* lecture d'une image PPM (rgb) ou PGM (niveaux de gris) */
Image LireImage(char *nom)
{   int i, nbOctets;
	GLubyte *P;
	Image I;
	FILE *f;
	char s[MAXLIGNE];
	/* Ouverture du fichier */
	f = fopen(nom, "r");
	if (f == NULL) exit(0);
	/* confirmation du codage PPM ou PGM */
	fgets(s, MAXLIGNE, f);
	if (s[0] != 'P' || ((s[1] != '6') && (s[1] != '5')))
	{ printf("erreur de format\n") ; 
	  exit(0); 
	}
	I = (Image) malloc(sizeof(struct image));
	if (s[1] == '6') { I->rgb=1; nbOctets=3;}
	else { I->rgb=0; nbOctets=1;}
	/* Lecture larg haut et allocation de l'image */
	do fgets(s, MAXLIGNE, f) ; while (s[0] == '#') ;
	sscanf (s, "%d %d", &I->larg, &I->haut) ;
	P = I->dat = (GLubyte*) malloc(sizeof(GLubyte)*nbOctets*I->larg*I->haut);
	I->pix = (GLubyte**) malloc(sizeof(GLubyte*)*I->haut);
	for (i=0 ; i<I->haut ; i++)
	{ I->pix[i] = P;
	  P = P+nbOctets*I->larg;
	}
	/* Saut des commentaires et lecture de maxgrey */
	do fgets(s, MAXLIGNE, f); while (s[0] == '#') ;
	/* Lecture de la matrice image */
	fread(I->dat,sizeof(GLubyte)*nbOctets*I->larg*I->haut,1,f);
	fclose(f);
	return I;
}


/* restitution de l'espace memoire occupe par une image */
void restituerImage(Image I)
{ free(I->pix);
  free(I->dat);
  free(I);
  I=NULL;
}

//fonction qui construit la scène 3D
void construireScene(void)
{
	int x , y;	
	//glColor3fv(couleurAxe);  
	// "effacement" de la fenetre et du z-buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	//trackball
	glPushMatrix();
	glMultMatrixd(mlTbGetTransformation());
	
	/*
	//gestion des lumires
	glLightfv (GL_LIGHT0, GL_AMBIENT, Lambiant);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, Lblanche);
	glLightfv (GL_LIGHT0, GL_SPECULAR, Lblanche);
	glLightfv (GL_LIGHT0, GL_POSITION, position);

	//on définit le matériau 
	glMaterialfv(GL_FRONT, GL_EMISSION, Lnoire);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbiant);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS,matShininess);
	*/
	
	
	//******************** HEIGHTMAP*******************
	glEnable(GL_TEXTURE_2D);
	glTranslatef(-heightmap_largeur/2 , 0.0 , -heightmap_longeur/2);
	/*
	//pour centrrer l'ile
	
	//texture a appliquer au "sol"
	
	glBindTexture(GL_TEXTURE_2D,textures[TEXT_SOL]);
	
	//1ere verion
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3i( 0 , 0 , 0 );
		
		glTexCoord2f(0.0, heightmap_longeur);
		glVertex3i( 0 , 0 , heightmap_longeur );
		
		glTexCoord2f(heightmap_largeur, heightmap_longeur);
		glVertex3i( heightmap_largeur , 0 , heightmap_longeur);
		
		glTexCoord2f(heightmap_largeur, 0.0);
		glVertex3i( heightmap_largeur , 0 , 0 );
	glEnd();
	*/
	glBindTexture(GL_TEXTURE_2D,textures[TEXT_SOL]);
	for ( x = 0 ; x < heightmap_largeur - 1 ; x++ )
	{
		for ( y = 0 ; y < heightmap_longeur - 1 ; y++ )
		{
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex3i( x , heightmap[x][y] , y );
				
				glTexCoord2f(0.0, 1.0);
				glVertex3i( x , heightmap[x][y+1] , y+1  );
				
				glTexCoord2f(1.0, 1.0);
				glVertex3i( x+1 , heightmap[x+1][y+1] ,  y+1  );
				
				glTexCoord2f(1.0, 0.0);
				glVertex3i( x+1 , heightmap[x+1][y] , y );
			glEnd();
		}
	}

	glPopMatrix();
	glutSwapBuffers ();
}


/* Fonction liees aux interruptions */
/* -------------------------------- */

//fonction qui gere un changement de taille de la fenetre
void fenetrage(int _w, int _h)
{ 
	winX = _w;
	winY = _h;

	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)winX/(double)winY, 0.1, 50000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20.00, +20.00, +20.00, +0.00, +0.00, +0.00, +0.00, +1.00, +0.00);
	

	mlTbInit(winX, winY);
	glutPostRedisplay();
}

//gere les appuits des tches du clavier
void clavier (unsigned char key, int x, int y)
{   switch (key)
	{ 
		case 27 : /* escape : fin */
			exit (0);
		break;
		
		case 'i' : //zoom in
			mlTbZoom(+0.1);
		break;
		
		case 'o' : //zoom out
			mlTbZoom(-0.1);
		break;
		
		case 122 : //z avance camera sur l'axe z
			Zcamera -= 1;
			ZaxeCamera -= 1;
		break;
		
		case 115 : // s recule camera sur l'axe z
			Zcamera += 1;
			ZaxeCamera += 1;
		break;
			
		case 113 : //q recule camera sur l'axe x
			Xcamera -= 1;
			XaxeCamera -= 1;
		break;
		
		case 100 : //d avance sur x
			Xcamera += 1;
			XaxeCamera += 1;
		break;
		
		case 9 :
			mActu++;
			if (mActu == 4)
				mActu = 1;
		break;  
    }
    glutPostRedisplay();
}

void fleches(int key, int x, int y)
{ 	
	switch (key)
	{
		case GLUT_KEY_UP :
			Ycamera = Xcamera + 10;
			
			/*switch (mActu)
			{
				case 1 :
					if(rotaM1 < 90)
						rotaM1 += 10;
				break;
					
				case 2 :
					if(rotaM2 > 0)
						rotaM2 -= 10;
				break;
				
				case 3 :
					transM3 -= 1;
					if( transM3 < 0)
						transM3 = 0;
				break;
			}*/
		break;
				
		case GLUT_KEY_DOWN :
			Ycamera = Xcamera - 10;
		break;
						
		case GLUT_KEY_LEFT :
			Xcamera = Xcamera - 10;
			/*Xcamera = Xcam*cos(M_PI/ 8) + Zcam*sin(M_PI / 8);
			Zcamera = -Xcam*sin(M_PI / 8) + Zcam*cos(M_PI/8);*/
			
			
			//rotaGlob += 10;
		break;
		
		case GLUT_KEY_RIGHT :
			Xcamera = Xcamera + 10;
			/*Xcamera = Xcam*cos(M_PI / 8) - Zcam*sin(M_PI / 8);
			Zcamera =  Xcam*sin(M_PI / 8) + Zcam*cos(M_PI/8); */
			
			
			//rotaGlob -=10;
		break;
	}

	glutPostRedisplay();
}

void souris(int _button, int _state, int _x, int _y)
{ 
	if(_button == GLUT_LEFT_BUTTON)
	{
		mouseLeftDown = (_state == GLUT_DOWN);

		if(mouseLeftDown == 1)
			mlTbClick(_x, _y);
		else
			mlTbRelease(_x, _y);
	}
	else if(_state == GLUT_UP)
	{
		if(_button == 3)
			mlTbZoom(-0.01);
		else if(_button == 4)
			mlTbZoom(0.01);
	}
}

void menu(int value)
{ switch (value)
  { case 0: /* bascule face pleine/aretes que vous devez programmer */
	
      break ;
    case 2:
      exit(0);
      break ;
   }
}

// renvoie un rand entier entre a et b inclus
int rand_a_b(int a, int b)
{
	//pour avoir un entier entre a et b inclus (donc un intervalle de taille b-a+1 ) 
	return rand()%(b-a+1) +a;
}

/* Callback OpenGL de gestion de drag */
void motionGL(int _x, int _y)
{
	if(mouseLeftDown == 1)
		mlTbMotion(_x, _y);

	glutPostRedisplay();
}

void initScene (void)
{
	int i, j;
	
	//initialisation des variables
	mouseLeftDown = 0;
	
	
	
	glClearColor(0.8, 0.8, 0.7, 1.0);
	//glCullFace (GL_BACK); // designation des faces "non visibles"
	//glEnable (GL_CULL_FACE); // faces "non visibles" ignorees
	//glPolygonMode (GL_FRONT_AND_BACK , GL_FILL); // faces avants pleines (sens trigo des sommets)
	//glPolygonMode (GL_BACK, GL_FILL); // faces arrieres vides
	//glEnable (GL_DEPTH_TEST);
	
	
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	
	
	
	//on charge les textures
	glGenTextures(4, textures);
	
	Image chrome;
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	chrome = LireImage("text/TextureChrome.ppm") ;
	glBindTexture(GL_TEXTURE_2D,textures[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D , 3 , chrome->larg , chrome->haut , GL_RGB , GL_UNSIGNED_BYTE , chrome->dat);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT) ;
	restituerImage( chrome );
	
	Image poneys;
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	poneys = LireImage("text/my-little-pony-friendship-is-magic-all-ponies.ppm") ;
	glBindTexture(GL_TEXTURE_2D,textures[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D , 3 , poneys->larg , poneys->haut , GL_RGB , GL_UNSIGNED_BYTE , poneys->dat);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT) ;
	restituerImage( poneys );
	
	Image rainbow;
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	rainbow = LireImage("text/my-little-pony-friendship-is-magic-rainbow-dash-1440x1080-wallpaper.ppm") ;
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	gluBuild2DMipmaps(GL_TEXTURE_2D , 3 , rainbow->larg , rainbow->haut , GL_RGB , GL_UNSIGNED_BYTE , rainbow->dat);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT) ;
	restituerImage( rainbow );
	
	Image triangle_sol;
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	triangle_sol = LireImage("text/texture_sol.ppm") ;
	glBindTexture(GL_TEXTURE_2D,textures[3]);
	gluBuild2DMipmaps(GL_TEXTURE_2D , 3 , triangle_sol->larg , triangle_sol->haut , GL_RGB , GL_UNSIGNED_BYTE , triangle_sol->dat);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT) ;
	restituerImage( triangle_sol );

	//on créer la heightmap	
	heightmap = malloc(sizeof(*heightmap) * heightmap_largeur);	
	for (i = 0; i < heightmap_largeur; i++)
		heightmap[i] = malloc(sizeof(**heightmap) * heightmap_longeur);
    
	if (heightmap == NULL) // Si l'allocation a échoué
	{
		exit(0); // On arrête le programme
	}
	//on met des valeurs dans la heightmap
	srand(time(NULL)); // initialisation de rand

	
	for (i = 0; i < heightmap_largeur; i++)
	{
		for (j = 0; j < heightmap_longeur; j++)
		{
			//on met les bords à hautur min pour avoir de joli bords lololo
			if( i == 0 || j == 0 || i == heightmap_largeur-1 || j == heightmap_longeur-1 )
			{
				heightmap[i][j] = heightmap_hauteur_min;
			}
			else
			{
				heightmap[i][j] = rand_a_b( heightmap_hauteur_min , heightmap_hauteur_max );
			}
			//printf("| %d |", heightmap[i][j]);
		}
			//printf("\n");
	}
	
	
}



void sousMenuRemplissage (int n)
{
	switch (n)
	{
		case 0 :  
			glPolygonMode (GL_FRONT, GL_LINE); // faces avants pleines (sens trigo des sommets)
    			glPolygonMode (GL_BACK, GL_LINE); // faces arrieres vides
			construireScene();
			break; 
		case 1 : 
			glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
			construireScene();
			break;
	}


}
void monSousMenu (int n)
{
	switch (n)
	{
		case 0 :   
			couleurObjet[0] = 1;
			couleurObjet[1] = 0;
			couleurObjet[2] = 1;
			construireScene();
				
			break;
		case 1 :
			couleurObjet[0] = 0;
			couleurObjet[1] = 0;
			couleurObjet[2] = 1;
			construireScene();
			break;
		case 2 :
			couleurObjet[0] = 0.7;
			couleurObjet[1] = 0.7;
			couleurObjet[2] = 0.7;
			construireScene();
			break;
	}		
}

int island (int argc, char **argv)
{   
	//initialiser le tableau des normales
	N[0][0] = 1; //x
	N[1][0] = -1; //-x
	N[2][1] = 1; //y
	N[3][1] = -1; //-y
	N[4][2] = 1; //z
	N[5][2] = -1; //-z
	
	
	/* initialisation de la session GLUT */
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	/* creation d'une fenetre hauteur-largeur*/
	glutInitWindowSize (512,512);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv [0]);
	/* designation des fonctions liees aux interruptions */
	glutDisplayFunc(construireScene);
	glutReshapeFunc(fenetrage);
	glutKeyboardFunc(clavier);
	glutMouseFunc(souris);
	glutSpecialFunc(fleches);
	glutMotionFunc(motionGL);
	/* Un petit menu au bouton gauche */
	/*
	int numSousMenuRemplissage;
    	numSousMenuRemplissage = glutCreateMenu(sousMenuRemplissage);
   	glutAddMenuEntry("Arête" , 0);
	glutAddMenuEntry("OUI!" , 1);	
	int numSousMenu;
    	numSousMenu = glutCreateMenu(monSousMenu);
   	glutAddMenuEntry("Violet" , 0);
	glutAddMenuEntry("Bleu" , 1);
	glutAddMenuEntry("Gris" , 2);
	glutCreateMenu(menu);
	//glutAddMenuEntry("facettes/aretes", 0);
		glutAddSubMenu("Je te remplis?" , numSousMenuRemplissage);
		glutAddSubMenu("choix couleur" , numSousMenu);
		glutAddMenuEntry("quitter", 2);
	
    glutAttachMenu(GLUT_LEFT_BUTTON);
    */
    //initialisation de a scene
    initScene();
    
	/* boucle d'evenements */
    glutMainLoop ();
    
    free(heightmap);
    return 0;
}

