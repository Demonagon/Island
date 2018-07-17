#ifndef FLUID__H
#define FLUID__H
#include "util_math.h"

#define FLUID_MATRIX_MAX_WIDTH 100
#define FLUID_MATRIX_MAX_HEIGHT 100

/**
* Par Pacôme le 17/07/2018
* Code qui permet de simuler un fluide modélisé par une matrice de vecteurs.
* Chaque vecteur peut être mis à jour en fonction des vecteurs voisins.
* Le système complet peut être paramétré selont une variable de souplesse
* (entre 0 et 1). L'idée est qu'un fluide avec une grande souplesse pourrait
* simuler des systèmes de fluides assez volatiles comme le vent et un fluide
* avec une très basse souplesse des systèmes comme des plaques tectoniques.
* Une autre possibilité d'application sont les courants moyens, avec une
* souplesse moyenne.
*/

typedef char FluidVectorAnglePrecision;
typedef char FluidVectorValuePrecision;

typedef char FluidVectorAngle;
typedef char FluidVectorValue;

typedef struct FluidVector {
	FluidVectorAngle angle;
	FluidVectorValue value;
} FluidVector;

typedef FluidVectorAnglePrecision FVAngleP;
typedef FluidVectorValuePrecision FVValueP;
typedef FluidVectorAngle FVAngle;
typedef FluidVectorValue FVValue;
typedef FluidVector FV;
typedef Complex FVC;

FV fluidVectorCreate(FVAngle angle, FVValue value);
FVC fluidVectorComplexRepresentation(FV vector, FVAngleP a, FVValueP v);
FV fluidVectorConvert(Complex complex, FVAngleP a, FVValueP v);

FVC fluidVectorInfluence(FVC fv_source, FVC fv_actor, double pliability);

FVC fluidVectorUpdate(FVC * neighbours, FVC center, double pliability);

typedef struct FluidMatrix {
	int width; int height;
	FluidVector * * vectors;
	FluidVectorAnglePrecision angle_precision;
	FluidVectorValuePrecision value_precision;
	double pliability;
} FluidMatrix;

FluidMatrix fluidMatrixCreate(int width, int height, double pliability,
							  FluidVectorAnglePrecision angle_precision,
							  FluidVectorValuePrecision value_precision);
void fluidMatrixFree(FluidMatrix * matrix);

/*
* En cas de out of bounds, retourne NULL.
*/
FV * fluidMatrixGetVector(FluidMatrix * matrix, int x, int y);

void fluidMatrixUpdate(FluidMatrix * matrix);

void printFluidVectorAngle(FluidVectorAngle angle, FVAngleP precision);
void printFluidVectorValue(FluidVectorValue value, FVValueP precision);
void printFluidMatrix(FluidMatrix * matrix);


void fluidMatrixMainTest(void);

#endif
