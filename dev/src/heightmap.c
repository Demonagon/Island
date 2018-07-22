#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "island.h"
#include "util_math.h"
//On crée la heighmap de base à partir de laquelle on va interpoler plusieurs sous heighmap
int** heightmap_init ( int** heightmap , int heightmap_largeur , int heightmap_longeur , int heightmap_hauteur_min , int heightmap_hauteur_max )
{
	int i, j;
	//on créer la heightmap	
	heightmap = malloc(sizeof(*heightmap) * heightmap_largeur);
	for (i = 0; i < heightmap_largeur; i++)
		heightmap[i] = malloc(sizeof(**heightmap) * heightmap_longeur);
		
	if (heightmap == NULL) // Si l'allocation a échoué
	{
		exit(0); // On arrête le programme
		return 0;
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
	//printf("\nINiT FIN\n");
	return heightmap;
}

/*
//fontion d'interpolation de la hrightmap
void heightmap_interpolate()
*/
