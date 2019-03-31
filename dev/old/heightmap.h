#ifndef HEIGHTMAP__H
#define HEIGHTMAP__H
/*Par Mathieu le 22/07/2018
Fichier contenant les fonction d'initialisation et d'interpolaation de la heightmap
*/

//ennumeration des differentes interpolation utilisés
typedef enum{ INTERPOL_DIAMOND_SQUARE  }InterpolationType;

// function that initialise our base (heightmap_largeur * heightmap_longeur heightmap) with a z coordonate from heightmap_hauteur_min to heightmap_hauteur_max
//this is this base heightmap that we will interpolate later on
int** heightmap_init ( int heightmap_largeur , int heightmap_longeur , int heightmap_hauteur_min , int heightmap_hauteur_max ); 

//funtion that calculate the new interpolated heightmap from the base heightmap
double** heightmap_interpolate(int** heightmap , InterpolationType interpolation_type , int resolution , int heightmap_largeur , int heightmap_longeur);

//fonction that realise the diamond suare algorithm
double** heightmap_interpolate_diamod_square( int i, int j, double ** heightmap_interpolated , int ** heightmap , int resolution );
#endif