/* CANEVAS POUR UNE INITIATION A OPENGL */
/*======================================*/

/* Compilation sous UNIX : dans le répertoire de travail
 
 1°) Creez un fichier "Makefile" avec le texte ci-dessous (remplacez <tabulation> par la caractère  tabulation). Supprimez -lXi -lXmu suivant les configurations systèmes.
 -----------------------------------------
 XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11 -lXext -lpthread
 GLLIBS = -L/usr/X11R6/lib -lglut -lGL -lGLU
 
 helloGlut : helloGlut.o
 <tabulation>gcc -o helloGlut helloGlut.o $(GLLIBS) $(XLIBS) -lm
 
 2°) Compilez avec la commande 
 -----------------------------
 make helloGlut
 
 3°) Executez avec la commande
 -----------------------------
 ./helloGlut
 
*/




/* Exemple des fonctionnalites du fenetrage GLUT */
/* ============================================= */

// La scene consiste juste en un triangle et le trace des 3 axes du repere.
// Le bouton gauche de la souris donne acces a un menu
// Un message specifique revele l'enchainement des fonctions d'interruption declenchees par GLUT

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(__APPLE__) && !defined(__APPLE_X11)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define PI (3.1415926)


/* Variables globales pour parametrer la scene */
/* ------------------------------------------- */
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
GLuint textures[3];
//matÈriaux chrome
GLfloat Lnoire [4] = {0.0, 0.0, 0.0, 1.0};
GLfloat matAmbiant [4] = {0.25, 0.25 , 0.25 ,0.25}; 
GLfloat matDiffuse [4] = {0.4, 0.4, 0.4, 0.4};
GLfloat matSpecular [4] = {0.774597, 0.774597, 0.774597, 0.774597};
GLfloat matShininess [1] = {76.8};

//lumiËre soit!
GLfloat position [4] = {0.0, 10.0, 10.0, 1.0};
GLfloat Lambiant [4] = {0.2, 0.2, 0.2, 1.0};
GLfloat Lblanche [4] = {1.0, 1.0, 1.0, 1.0};

//tableau des normales (‡ normaliser LOOOLL)
float N [6][3] = {{1, 0}};


// Position de la camera
//float Zcamera=20., Xcamera=5., Ycamera=5.;
float Zcamera=15, Xcamera=0, Ycamera=3;
// et direction visée
float XaxeCamera=0, YaxeCamera=0, ZaxeCamera=-50;


/* Construction de la scene */
/* ------------------------ */

// Un objet simple dans la scene

typedef struct image
{ int larg, haut, rgb; 
	GLubyte *dat;
	GLubyte **pix;
} *Image; 
/*
Si on definit la variable :
   Image terre ;
on chargera l'image de la terre avec l'instruction
   terre=LireImage("terre.ppm") ;
terre->rgb est un booleen qui precise si un pixel est cod√© sur 3 ou 1 octet
les dimensions de l'image seront terre->larg et terre->haut
on accedera a un pixel (i,j) avec terre->pix[i][j]
on accedera a la totalite de la matrice image avec terre->dat
*/

/* lecture d'une image PPM (rgb) ou PGM (niveaux de gris) */
Image LireImage(char *nom)
{   int i, nbOctets;
	GLubyte *P;
	Image I;
	FILE *f;
	char s[MAXLIGNE];
	int maxgrey;
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







void construireObjet(void)
{  glBegin (GL_TRIANGLES);
     glVertex3f (-1., .0, .0);
     glVertex3f (1., .0, .0);
     glVertex3f (0., 1., .0);
   glEnd ();
}



// Repere orthonorme
void repereScene3D(float l)
{  glColor3f(1., 0., 0.) ;
   glBegin (GL_LINES);
     glVertex3f (0., 0., 0.);
     glVertex3f (l, 0., 0.);
     glVertex3f (0., 0., 0.);
     glVertex3f (0., l, 0.);
     glVertex3f (0., 0., 0.);
     glVertex3f (0., 0., l);
   glEnd ();
}

void TracerCube()
{
	typedef float Point3D[3];

	Point3D p1 = {-0.5, 0.5, 0.5};
	
	Point3D p2 = {-0.5, -0.5, 0.5};

	Point3D p3 = {0.5, -0.5, 0.5};

	Point3D p4 = {0.5, 0.5, 0.5};

	Point3D p5 = {0.5, 0.5, -0.5};

	Point3D p6 = {-0.5, 0.5, -0.5};

	Point3D p7 = {0.5, -0.5, -0.5};

	Point3D p8 = {-0.5, -0.5, -0.5};

	glEnable(GL_TEXTURE_2D);
	

	glBegin(GL_QUADS);
	/*
	 * N[0][0] = 1; //x
	N[1][0] = -1; //-x
	N[2][1] = 1; //y
	N[3][1] = -1; //-y
	N[4][2] = 1; //z
	N[5][2] = -1; //-z
	* */
	
	//DEVANT
	
	glNormal3f(N[4][0] , N[4][1] , N[4][2] );
	
	
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p1);

	
	glTexCoord2f(1.0, 0.0); 
	glVertex3fv(p2);
	
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(p3); 
	
	glTexCoord2f(0.0, 1.0);
	glVertex3fv(p4); 

	//DROITE
	glNormal3f(N[0][0] , N[0][1] , N[0][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p4);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(p3);
	
	glTexCoord2f(1.0, 1.0); 
	glVertex3fv(p7);
	
	glTexCoord2f(0.0, 1.0); 
	glVertex3fv(p5);

	//HAUT
	glNormal3f(N[2][0] , N[2][1] , N[2][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p6);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(p1);
	
	glTexCoord2f(2.0, 1.0);
	glVertex3fv(p4);
	
	glTexCoord2f(0.0, 1.0); 
	glVertex3fv(p5);

	//GAUCHE
	glNormal3f(N[1][0] , N[1][1] , N[1][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p6);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(p8);
	
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(p2);
	
	glTexCoord2f(0.0, 1.0); 
	glVertex3fv(p1);

	//DERRIERE
	glNormal3f(N[5][0] , N[5][1] , N[5][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p5);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(p7);
	
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(p8);
	
	glTexCoord2f(0.0, 1.0); 
	glVertex3fv(p6);

	//DESSOUS
	glNormal3f(N[3][0] , N[3][1] , N[3][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p2);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(p8);
	
	glTexCoord2f(1.0, 2.0);
	glVertex3fv(p7);
	
	glTexCoord2f(0.0, 1.0); 
	glVertex3fv(p3);

	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
}

void TracerCubeSol() //permet de rÈaliser la texture du sol sans que cette derniËre soit ÈtirÈe
{
	typedef float Point3D[3];

	Point3D p1 = {-0.5, 0.5, 0.5};
	
	Point3D p2 = {-0.5, -0.5, 0.5};

	Point3D p3 = {0.5, -0.5, 0.5};

	Point3D p4 = {0.5, 0.5, 0.5};

	Point3D p5 = {0.5, 0.5, -0.5};

	Point3D p6 = {-0.5, 0.5, -0.5};

	Point3D p7 = {0.5, -0.5, -0.5};

	Point3D p8 = {-0.5, -0.5, -0.5};

	glEnable(GL_TEXTURE_2D);
	

	glBegin(GL_QUADS);
	/*
	 * N[0][0] = 1; //x
	N[1][0] = -1; //-x
	N[2][1] = 1; //y
	N[3][1] = -1; //-y
	N[4][2] = 1; //z
	N[5][2] = -1; //-z
	* */
	
	//DEVANT
	
	glNormal3f(N[4][0] , N[4][1] , N[4][2] );
	
	
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p1);

	
	glTexCoord2f(7, 0.0); 
	glVertex3fv(p2);
	
	glTexCoord2f(7.0, 7.0);
	glVertex3fv(p3); 
	
	glTexCoord2f(0.0, 7.0);
	glVertex3fv(p4); 

	//DROITE
	glNormal3f(N[0][0] , N[0][1] , N[0][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p4);
	
	glTexCoord2f(7.0, 0.0);
	glVertex3fv(p3);
	
	glTexCoord2f(7.0, 7.0); 
	glVertex3fv(p7);
	
	glTexCoord2f(0.0, 7.0); 
	glVertex3fv(p5);

	//HAUT
	glNormal3f(N[2][0] , N[2][1] , N[2][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p6);
	
	glTexCoord2f(7.0, 0.0);
	glVertex3fv(p1);
	
	glTexCoord2f(7.0, 7.0);
	glVertex3fv(p4);
	
	glTexCoord2f(0.0, 7.0); 
	glVertex3fv(p5);

	//GAUCHE
	glNormal3f(N[1][0] , N[1][1] , N[1][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p6);
	
	glTexCoord2f(7.0, 0.0);
	glVertex3fv(p8);
	
	glTexCoord2f(7.0, 7.0);
	glVertex3fv(p2);
	
	glTexCoord2f(0.0, 7.0); 
	glVertex3fv(p1);

	//DERRIERE
	glNormal3f(N[5][0] , N[5][1] , N[5][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p5);
	
	glTexCoord2f(7.0, 0.0);
	glVertex3fv(p7);
	
	glTexCoord2f(7.0, 7.0);
	glVertex3fv(p8);
	
	glTexCoord2f(0.0, 7.0); 
	glVertex3fv(p6);

	//DESSOUS
	glNormal3f(N[3][0] , N[3][1] , N[3][2] );
	glTexCoord2f(0.0, 0.0); 
	glVertex3fv(p2);
	
	glTexCoord2f(7.0, 0.0);
	glVertex3fv(p8);
	
	glTexCoord2f(7.0, 7.0);
	glVertex3fv(p7);
	
	glTexCoord2f(0.0, 7.0); 
	glVertex3fv(p3);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	
}

void membre3()
{
glPushMatrix();
	glTranslatef(17 - transM3, 0, 0);
	glRotatef( 180 , 0 , 1 , 0 );
	glScalef(0.7 , 0.7 , 0.7 );
	glPushMatrix();
		glTranslatef(5.5, 0, 0);
		glScalef(9, 1, 1);
		TracerCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5, 0, 0.6);
		glScalef(3, 1, 0.2);
		TracerCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5, 0, -0.6);
		glScalef(3, 1, 0.2);
		TracerCube();
	glPopMatrix();
glPopMatrix();	

}

void membre2()
{
glPushMatrix();
	glTranslatef(9.5, 0, 0);
	glRotatef(-rotaM2, 0, 0, 1);	
	glPushMatrix();
		glTranslatef(5.5, 0, 0);
		glScalef(9, 1, 1);
		TracerCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5, 0, 0.6);
		glScalef(3, 1, 0.2);
		TracerCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5, 0, -0.6);
		glScalef(3, 1, 0.2);
		TracerCube();
	glPopMatrix();
	membre3();
glPopMatrix();	

}

void membre1()
{
	glPushMatrix();
		glTranslatef(5.5, 0, 0);
		glScalef(9, 1, 1);
		TracerCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5, 0, 0.6);
		glScalef(3, 1, 0.2);
		TracerCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5, 0, -0.6);
		glScalef(3, 1, 0.2);
		TracerCube();
	glPopMatrix();
	
	membre2();

}

void epaule()
{
	glPushMatrix();
		glTranslatef(0, 0.95, 0);
		glScalef(1, 1.5, 1);
		TracerCube();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	glPushMatrix();
		glRotatef(-rotaGlob, 0,1,0); //la rotation globale sur tout le systËme sauf le socle
		glTranslatef(0.0, 0.1, 0.0);
		glScalef(64, 0.2, 64);
		TracerCubeSol();
	glPopMatrix();

}






void construireScene(void)
{ printf("On passe dans 'construireScene'\n");
	//glColor3fv(couleurAxe);  
// "effacement" de la fenetre et du z-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // initialisation de MODELVIEW
	glEnable(GL_LIGHTING);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	
	glLoadIdentity ();
  // position de la camera
	
  gluLookAt (Xcamera, Ycamera, Zcamera, // position
             Xcamera + XaxeCamera , Ycamera + YaxeCamera, Zcamera + ZaxeCamera ,  // point vise
             0.0, 1.0, 0.0);  // "verticale appareil"
	glLightfv (GL_LIGHT0, GL_AMBIENT, Lambiant);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, Lblanche);
	glLightfv (GL_LIGHT0, GL_SPECULAR, Lblanche);
	glLightfv (GL_LIGHT0, GL_POSITION, position);
             
		glRotatef(rotaGlob, 0,1,0); 
  		
  
  //repereScene3D(5.); // on met un putain de quadrillage

/*	int i , j;	
	for (i  = -20 ; i <=20 ; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i,0,-20);
		glVertex3f(i,0,20);
		glEnd();
	}
	for (j  = -20 ; j <=20 ; j++)
	{
		glBegin(GL_LINES);
		glVertex3f(-20,0,j);
		glVertex3f(20,0,j);
		glEnd();
	}   */

	//on dÈfinit le matÈriau de du bras et socle(Èpaule)
	glMaterialfv(GL_FRONT, GL_EMISSION, Lnoire);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbiant);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS,matShininess);

	glBindTexture(GL_TEXTURE_2D,textures[1]);
	epaule();
	
	glBindTexture(GL_TEXTURE_2D,textures[0]);
glPushMatrix(); // Membre 1
	glTranslatef(0, 1.2, 0);
	glRotatef(rotaM1, 0, 0, 1);
	membre1();
glPopMatrix();	

/*glPushMatrix();
	glTranslatef( 9.5 * cos(M_PI * rotaM1 / 180.0 ), 9.5 * sin(M_PI * rotaM1 / 180.0 ), 0);
	glRotatef(rotaM1 + 270, 0, 0, 1);
    membre();
glPopMatrix(); */
		
  

//printf ("%f" , Zcamera);
  glutSwapBuffers ();
}


/* Fonction liees aux interruptions */
/* -------------------------------- */

void fenetrage(int largeur, int hauteur)
{ printf("On passe dans 'projection'\n");
  glViewport (0, 0, largeur, hauteur);
  // Projection 3D -> 2D liee à la taille de la fenêtre
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (65.0, (float) largeur/(float) hauteur, 1.0, 500.0);
  glMatrixMode (GL_MODELVIEW);
}





void clavier (unsigned char key, int x, int y)
{   switch (key)
	{ 
		case 27 : /* escape : fin */
			exit (0);
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
			switch (mActu)
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
			}
		break;
				
		case GLUT_KEY_DOWN :
			switch (mActu)
			{
				case 1 :
					if(rotaM1 > 0) 
						rotaM1 -= 10;
				break;
				
				case 2 :
					if(rotaM2 < 90) 
						rotaM2 += 10;
				break;
				
				case 3 :
					transM3 += 1;
					if( transM3 > 5.6)
						transM3 = 5.6;
				break;
			}
		break;
						
		case GLUT_KEY_LEFT :
			/*Xcamera = Xcam*cos(M_PI/ 8) + Zcam*sin(M_PI / 8);
			Zcamera = -Xcam*sin(M_PI / 8) + Zcam*cos(M_PI/8);*/
			  rotaGlob += 10;
		break;
		
		case GLUT_KEY_RIGHT :	
			/*Xcamera = Xcam*cos(M_PI / 8) - Zcam*sin(M_PI / 8);
			Zcamera =  Xcam*sin(M_PI / 8) + Zcam*cos(M_PI/8); */
			rotaGlob -=10;
		break;
	}

	//gluLookAt(Xcamera, Ycamera, Zcamera, 0, 0, 0, 0, 1, 0);
	glutPostRedisplay(); //pour demander au systeme un nouvel affichage
}

void souris(int bouton, int state, int x, int y)
{ // state vaut GLUT_DOWN ou GLUT_UP
  printf("click souris\n");
  switch (bouton)
  { case GLUT_LEFT_BUTTON : /* inutile ici, car utilise pour le menu (cf main) */
      break;
    case GLUT_MIDDLE_BUTTON :
      // inserer ici l'action associee au bouton central
      // glutPostRedisplay();  si l'evenement doit modifier l'affichage
      break;
    case GLUT_RIGHT_BUTTON :
     	 TracerCube();
      break;
  }
}

void menu(int value)
{ switch (value)
  { case 0: /* bascule face pleine/aretes que vous devez programmer */
	
		
	
	
	
	printf("COUCOU MA GUEULE!\n") ;
      break ;
    case 2:
      exit(0);
      break ;
   }
}


/* Initialisation et boucle d'evenements */
/* ------------------------------------- */
/*
Si on definit la variable :
   Image terre ;
on chargera l'image de la terre avec l'instruction
   terre=LireImage("terre.ppm") ;
terre->rgb est un booleen qui precise si un pixel est codÈ sur 3 ou 1 octet
les dimensions de l'image seront terre->larg et terre->haut
on accedera a un pixel (i,j) avec terre->pix[i][j]
on accedera a la totalite de la matrice image avec terre->dat
*/
void initScene (void)
{   glClearColor (0.0, 0.0, 0.0, 0.0);
    glCullFace (GL_BACK); // designation des faces "non visibles"
//  glEnable (GL_CULL_FACE); // faces "non visibles" ignorees
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // faces avants pleines (sens trigo des sommets)
    //glPolygonMode (GL_BACK, GL_FILL); // faces arrieres vides
    glEnable (GL_DEPTH_TEST);
    //chargement de l'image
	
	glGenTextures(2, textures);
	
	Image rainbow;
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    rainbow = LireImage("/home/mathieu/Bureau/Project Island/Island/Test/my-little-pony-friendship-is-magic-rainbow-dash-1440x1080-wallpaper.ppm") ;
    glBindTexture(GL_TEXTURE_2D,textures[2]);
    gluBuild2DMipmaps(GL_TEXTURE_2D , 3 , rainbow->larg , rainbow->haut , GL_RGB , GL_UNSIGNED_BYTE , rainbow->dat);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT) ;
	
	
	Image poneys;
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    poneys = LireImage("/home/mathieu/Bureau/Project Island/Island/Test/my-little-pony-friendship-is-magic-all-ponies.ppm") ;
    glBindTexture(GL_TEXTURE_2D,textures[1]);
    gluBuild2DMipmaps(GL_TEXTURE_2D , 3 , poneys->larg , poneys->haut , GL_RGB , GL_UNSIGNED_BYTE , poneys->dat);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT) ;
    
    Image chrome;
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    chrome = LireImage("/home/mathieu/Bureau/Project Island/Island/Test/TextureChrome.ppm") ;
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    gluBuild2DMipmaps(GL_TEXTURE_2D , 3 , chrome->larg , chrome->haut , GL_RGB , GL_UNSIGNED_BYTE , chrome->dat);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT) ;
    
    
    
    restituerImage( chrome );
    
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

int main (int argc, char **argv)
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
        /* Un petit menu au bouton gauche */
	int numSousMenuRemplissage;
    	numSousMenuRemplissage = glutCreateMenu(sousMenuRemplissage);
   	glutAddMenuEntry("ArÍte" , 0);
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
	printf("Test 1\n");
    


	
            
    
    glutAttachMenu(GLUT_LEFT_BUTTON);
    /* boucle d'evenements */
    initScene();
   	printf("Test 2\n");

    glutMainLoop ();
    return 0;
}

