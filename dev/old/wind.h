#ifndef WIND__H
#define WIND__H
#include "util_math.h"

/**
* Créé par Pacôme le 22 Juillet 2018 comme une itération supplémentaire
* sur le concept de simulation du vent. Dans cette proposition les vecteurs
* ne sont plus en matrice mais positionnés librement. Ils sont tout de même
* arrangés sur une matrice afin de gérer facilement la proximité des vecteurs.
*/
/*
typedef struct WindVector {
	Complex position;
	Complex intensity;
} WindVector;

WindVector windCreateVector(Complex position, Complex intensity);

#define WIND_CELL_MAX_VECTOR_COUNT 30

typedef struct WindCell {
	WindVector * vectors;
	int size;
}

typedef struct WindMatrix {
	WindVector * * * matrix;
	int width;
	int height;
	double real_width;
	double real_height;
}

WindMatrix windCreateMatrix(int width, int height);

int windMatrixGetPositionCellWidth(double x);
int windMatrixGetPositionCellHeight(double y);
WindCell * windMatrixGetPositionCell(Complex position);

void windMatrixAddVector(WindVector vector);*/

#endif
