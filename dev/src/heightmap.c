#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "island.h"
#include "util_math.h"
//On crée la heighmap de base à partir de laquelle on va interpoler plusieurs sous heighmap
int** heightmap_init ( int heightmap_largeur , int heightmap_longeur , int heightmap_hauteur_min , int heightmap_hauteur_max )
{
	int** heightmap;
	int i, j;
	//on créer la heightmap	
	heightmap = malloc(sizeof(*heightmap) * heightmap_largeur);
	for (i = 0; i < heightmap_largeur; i++)
		heightmap[i] = malloc(sizeof(**heightmap) * heightmap_longeur);
		
	if (heightmap == NULL) // Si l'allocation a échoué
	{
		fprintf(stderr, "erreur d'allocation de la heightmap");
		exit(-1); // On arrête le programme
		return 0;
	}

	for (i = 0; i < heightmap_largeur; i++)
	{
		for (j = 0; j < heightmap_longeur; j++)
		{
			if( i == 0 || j == 0 || i == heightmap_largeur-1 || j == heightmap_longeur-1 )
			{
				heightmap[i][j] = heightmap_hauteur_min;
			}
			else
			{
				heightmap[i][j] = rand_int_a_b( heightmap_hauteur_min , heightmap_hauteur_max );
			}
		}
	}
	return heightmap;
}

//fonction du calcul de l'algorithme de diamand carre
double** heightmap_interpolate_diamod_square( int i, int j, double ** heightmap_interpolated , int ** heightmap , int resolution )
{
	int step, x, y, decal_x, decal_y;
	int tmp_moyenne, sum_tmp, id, decalage, n;
	step =  resolution-1;
	decal_x = i*step;
	decal_y = j*step;
	printf("resolution = %d\n",resolution);
	
	//initialisation of the 4 base point
	printf("decal_x = %d	decalx++ = %d	decal_y = %d	decaly++ = %d\n", decal_x , decal_x+step, decal_y , decal_y+step);
	heightmap_interpolated[decal_x][decal_y] = (double)heightmap[i][j];
	heightmap_interpolated[decal_x+step][decal_y] = (double)heightmap[i+1][j];
	heightmap_interpolated[decal_x][decal_y+step] = (double)heightmap[i][j+1];
	heightmap_interpolated[decal_x+step][decal_y+step] = (double)heightmap[i+1][j+1];
	
	printf("MIA Aou\n");
	//the step
	
	//how much we translate x and y to fit the higtmap_interpolated
	
	
	while( step > 1 )
	{
		printf("Algorihme diamand carre step = %d	i = %d	j = %d\n", step , i , j);
		id = step/2;
		printf("	Phase du diamand  step = %d\n", step);
		//begening of the diamond phase
		for(x = id; x < resolution; x = x + step)
		{
			for(y = id; y < resolution; y = y + step)
			{
				tmp_moyenne = ( heightmap_interpolated[decal_x+x-id][decal_y+y-id] + heightmap_interpolated[decal_x+x-id][decal_y+y+id] + heightmap_interpolated[decal_x+x+id][decal_y+y-id] + heightmap_interpolated[decal_x+x+id][decal_y+y+id] ) / 4.0;
				heightmap_interpolated[decal_x+x][decal_y+y] = tmp_moyenne + rand_double_a_b(-id,id);
			}
		}
		
		decalage = 0;
		//begening of the square phase
		printf("	Phase du carre  step = %d\n", step);
		for(x = 0; x < resolution; x = x + id)
		{
			if( decalage == 0 )
				decalage = id;
			else
				decalage = 0;
			for(y = decalage; y < resolution; y = y + step	)
			{
				printf("PROUT 1\n");
				sum_tmp = 0;
				n = 0;
				if( x >= id )
				{
					sum_tmp = sum_tmp + heightmap_interpolated[decal_x+x-id][decal_y+y];
					n++;
				}
				if( x + id < resolution )
				{
					sum_tmp = sum_tmp + heightmap_interpolated[decal_x+x+id][decal_y+y];
					n++;
				}
				if( y >= id )
				{
					sum_tmp = sum_tmp + heightmap_interpolated[decal_x+x][decal_y+y+-id];
					n++;
				}
				if( y + id < resolution )
				{
					sum_tmp = sum_tmp + heightmap_interpolated[decal_x+x][decal_y+y+id];
					n++;
				}
				
				heightmap_interpolated[decal_x+x][decal_y+y] = (sum_tmp / (double)(n)) + rand_double_a_b(-id,id);
				printf("MEGUMIN!!!!!\n");
			}
		}
		step = id;
	}
	printf("	RETURN!\n");
	return heightmap_interpolated;
}

//fontion d'interpolation de la heightmap
//la resoluion determine la precsion de l'interpolation, c'es le nombre de ponts total (points de base inclus) 	résultant apres interpoltion de 2 points succesifs
//par exmple si on interpole 2 points avec une resolution de 5 on crée 3 nouveaux points
//heightmap_largeur et heightmap_longeur reperesenten la largeur et la longeur de la heightmap de base
double** heightmap_interpolate(int** heightmap , int interpolation_type , int resolution , int heightmap_largeur , int heightmap_longeur)
{	
	int i, j;
	//la heightmap interpolé aura comme nouvelle longeur L = L_base + (L_base - 1) * (resolution - 2) de même pour la largeur
	double** heightmap_interpolated;
	int heightmap_interpolated_largeur, heightmap_interpolated_longeur;
	printf("acienne res = %d \n", resolution);
	//resolution doit être impaire
	if (resolution % 2 == 0)
		resolution++;
	if (resolution < 3)
		resolution = 3;
	printf("new res = %d \n", resolution);
	
	//calcul des nouvelles longeurs et largeurs
	heightmap_interpolated_largeur = heightmap_largeur + (heightmap_largeur - 1) * (resolution - 2);
	heightmap_interpolated_longeur = heightmap_longeur + (heightmap_longeur - 1) * (resolution - 2);
	printf("new heightap largeur = %d,longeur = %d \n", heightmap_interpolated_largeur ,heightmap_interpolated_longeur);
	//allocation de l'espace memoire
	heightmap_interpolated = malloc(sizeof(*heightmap_interpolated) * heightmap_interpolated_largeur);
	for (i = 0; i < heightmap_interpolated_largeur; i++)
		heightmap_interpolated[i] = malloc(sizeof(**heightmap_interpolated) * heightmap_interpolated_longeur);
		
	if (heightmap_interpolated == NULL) // Si l'allocation a échoué
	{
		fprintf(stderr, "erreur d'allocation de la heightmap interpolé");
		exit(-1); // On arrête le programme
		return 0;
	}
	
	//on va interpoler entre chaque point de la heightmap de base
	for (i = 0; i < heightmap_largeur-1; i++)
	{
		for (j = 0; j < heightmap_longeur-1; j++)
		{
			heightmap_interpolated = heightmap_interpolate_diamod_square(i , j , heightmap_interpolated , heightmap , resolution );
		}
	}
	printf("PROUT 42\n");
	return heightmap_interpolated;
}



