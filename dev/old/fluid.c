#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "fluid.h"

FV fluidVectorCreate(FVAngle angle, FVValue value) {
	return (FV) {.angle = angle, .value = value};
}

Complex fluidVectorComplexRepresentation(FV vector, FVAngleP a, FVValueP v) {
	double angle = (vector.angle / (double) a) * MATH_2_PI;
	double norm = (vector.value / (double) v);
	return complexCreatePolar(angle, norm);
}

FV fluidVectorConvert(Complex complex, FVAngleP a, FVValueP v) {
	FVAngle angle = (FVAngle) ( ( complexAngle(complex) / MATH_2_PI ) * a );
	double norm = complexNorm(complex);
	FVAngle value = norm <= 0 ? 0 : norm >= 1 ? v :
				 	(FVValue) ( norm * v );
	return fluidVectorCreate(angle, value);
}

FVC FVCgetNeightbourInfluenceVector(int index) {
	switch(index) {
		case 0 : return complexCreatePolar( 7 * MATH_PI / 4, SQRT_2 ); 
		case 1 : return complexCreate( + 0, - 1 ); 
		case 2 : return complexCreatePolar( 5 * MATH_PI / 4, SQRT_2 ); 
		case 3 : return complexCreate( + 1, + 0 ); 
		case 4 : return complexCreate( - 1, + 0 ); 
		case 6 : return complexCreatePolar( 1 * MATH_PI / 4, SQRT_2 ); 
		case 7 : return complexCreate( + 0, + 1 ); 
		case 8 : return complexCreatePolar( 3 * MATH_PI / 4, SQRT_2 ); 
		default : return complexCreate(0, 0);
	}
}

FVC fluidVectorInfluence(FVC fv_source, FVC fv_actor, double pliability,
						double * influence) {
	/*if( ! complexNorm(fv_actor) || ! complexNorm(fv_source) ) return fv_actor;
	double source_angle = complexAngle(fv_source);
	double actor_angle = complexAngle(fv_actor);
	double influence_factor = sin( source_angle - actor_angle );
	return complexScale( fv_actor, influence_factor );*/

	if( ! complexNorm(fv_actor) || ! complexNorm(fv_source) ) return fv_actor;
	double source_angle = complexAngle(fv_source);
	double actor_angle = complexAngle(fv_actor);
	double delta_angle = source_angle - actor_angle;
	double influence_factor = cos( delta_angle ) / 4 + 0.75;
	*influence = influence_factor;
	return ( MATH_PI / 2 <= delta_angle && delta_angle <= 3 * MATH_PI / 2 ) ?
		complexScale( fv_actor, - influence_factor ) :
		complexScale( fv_actor, + influence_factor );
}

FVC fluidVectorDotInfluence(FVC fv_source, FVC fv_actor, double pliability,
						int neightbour_index) {
	return complexCreate(0, 0);
}

FVC fluidVectorUpdate(FVC * neighbours, FVC center, double pliability, double loss) {
	double total_weight = 1.0 + loss;
	for(int k = 0; k < 8; k++) {
		double weight = 0;
		FVC neighbour = fluidVectorInfluence(center, neighbours[k], pliability,
						&weight);
		total_weight += weight;
		
		center = complexAdd(
					center,
					neighbour
				);
	}
	return complexScale(center, 1 / total_weight);
}

FluidMatrix fluidMatrixCreate(int width, int height, double pliability, double loss,
							  FluidVectorAnglePrecision angle_precision,
							  FluidVectorValuePrecision value_precision) {
	if( width > FLUID_MATRIX_MAX_WIDTH || height > FLUID_MATRIX_MAX_HEIGHT ) {
		fprintf(stderr, "(%s, line %d) => Fluid matrix cannot be initialised",
			__FILE__, __LINE__);
		fprintf(stderr, " with dimensions (width = %d, height = %d).\n",
			width, height);
		fprintf(stderr, "The maximum defined dimensions are (width = %d, ",
			FLUID_MATRIX_MAX_WIDTH);
		fprintf(stderr, "height = %d)\n", FLUID_MATRIX_MAX_HEIGHT);
		exit(-1);
	}
	FluidMatrix matrix;
	matrix.width = width;
	matrix.height = height;
	matrix.angle_precision = angle_precision;
	matrix.value_precision = value_precision;
	matrix.pliability = pliability;
	matrix.loss = loss;
	matrix.vectors = malloc( sizeof(FluidVector *) * width);
	for(int k = 0; k < matrix.width; k++)
		matrix.vectors[k] = malloc( sizeof(FluidVector) * height);
	return matrix;
}

void fluidMatrixFree(FluidMatrix * matrix) {
	for(int k = 0; k < matrix->width; k++)
		free(matrix->vectors[k]);
	free(matrix->vectors);
}

FV * fluidMatrixGetVector(FluidMatrix * matrix, int x, int y) {
	if( x < 0 || y < 0 || x >= matrix->width || y >= matrix->height )
		return NULL;
	return & matrix->vectors[x][y];
}

FVC FVgetNeightbours(FV buffer[FLUID_MATRIX_MAX_WIDTH][FLUID_MATRIX_MAX_HEIGHT],
				  int x, int y, int width, int height, FVAngleP a, FVValueP v) {
	return ( x < 0 || y < 0 || x >= width || y >= height ) ?
		   complexCreate(0, 0) :
		   fluidVectorComplexRepresentation(
				buffer[x][y], a, v
				);
}

void fluidMatrixUpdate(FluidMatrix * matrix) {
	FV b[FLUID_MATRIX_MAX_WIDTH][FLUID_MATRIX_MAX_HEIGHT];
	FVC neighbours[8];
	FVAngleP a = matrix->angle_precision;
	FVValueP v = matrix->value_precision;
	int w = matrix->width;
	int h = matrix->height;

	for(int x = 0; x < matrix->width; x++)
		for(int y = 0; y < matrix->height; y++)
			b[x][y] = matrix->vectors[x][y];

	for(int x = 0; x < matrix->width; x++)
		for(int y = 0; y < matrix->height; y++) {
			FVC center = fluidVectorComplexRepresentation(
				*fluidMatrixGetVector(matrix, x, y), a, v
				);

			neighbours[0] = FVgetNeightbours(b, x - 1, y - 1, w, h, a, v);
			neighbours[1] = FVgetNeightbours(b, x + 0, y - 1, w, h, a, v);
			neighbours[2] = FVgetNeightbours(b, x + 1, y - 1, w, h, a, v);
			neighbours[3] = FVgetNeightbours(b, x - 1, y + 0, w, h, a, v);
			neighbours[4] = FVgetNeightbours(b, x + 1, y + 0, w, h, a, v);
			neighbours[5] = FVgetNeightbours(b, x - 1, y + 1, w, h, a, v);
			neighbours[6] = FVgetNeightbours(b, x + 0, y + 1, w, h, a, v);
			neighbours[7] = FVgetNeightbours(b, x + 1, y + 1, w, h, a, v);

			*fluidMatrixGetVector(matrix, x, y) =
				fluidVectorConvert(
					fluidVectorUpdate(neighbours, center, matrix->pliability,
									  matrix->loss),
					a, v
					);
		}
}

void FVangleInterval(double angle, double lower, double higher, char * output) {
	if( lower < angle && angle <= higher ) printf("%s", output);
}

void printFluidVectorAngle(FluidVectorAngle angle, FVAngleP precision) {
	char * outputs[] = {"^>", "^^", "<^", "<<", "<v", "vv", "v>"};
	double rdangle = ( angle / (double) precision ) * MATH_2_PI;
	double pi_8 = MATH_PI / 8;

	if( rdangle > 15 * pi_8 || rdangle <= pi_8 )
		printf(">>");

	for(int k = 0; k < 7; k++)
		FVangleInterval(rdangle,(1 + 2*k) * pi_8, (3 + 2*k) * pi_8, outputs[k]);
}

void printFluidVectorValue(FluidVectorValue value, FVValueP precision) {
	int value_intensity = (int) ( (value / (double) precision) * 5 );
	switch(value_intensity) {
		case 0 : printf(" "); break;
		case 1 : printf("."); break;
		case 2 : printf(","); break;
		case 3 : printf(";"); break;
		case 4 : printf("!"); break;
		case 5 : printf("$"); break;

		default : printf("X"); break;
	}
}

void printFluidMatrix(FluidMatrix * matrix) {
	printf("Matrix ->\n");
	for(int y = matrix->height - 1; y >= 0; y--) {
		for(int x = 0; x < matrix->width; x++) {
			FluidVector * vector = fluidMatrixGetVector(matrix, x, y);
			if( ! vector->value ) {
				printf("  .. ");
				continue;
			}
			printf(" ");
			printFluidVectorValue(vector->value, matrix->value_precision);
			printFluidVectorAngle(vector->angle, matrix->angle_precision);
			printFluidVectorValue(vector->value, matrix->value_precision);
		}
		printf("\n");
	}
	printf("<-\n");
}

#include "time.h"

void fluidMatrixTest(void) {
	FluidMatrix matrix = fluidMatrixCreate(10, 10, 0.5/8, 0.01, 16, 100);
	srand(time(NULL));

	/*
	for(int x = 0; x < matrix.width; x++)
		for(int y = 0; y < matrix.height; y++)
			*fluidMatrixGetVector(&matrix, x, y) = fluidVectorCreate(
				rand() % matrix.angle_precision,
				rand() % (matrix.value_precision + 1) );
	//*/
	//*
	for(int x = 0; x < matrix.width; x++)
		for(int y = 0; y < matrix.height; y++)
			*fluidMatrixGetVector(&matrix, x, y) = fluidVectorCreate(0, 0);
	//*/
	

	//fluidMatrixGetVector(&matrix, 4, 4)->value = 15;

	printFluidMatrix(&matrix);

	int input;

	for(;;) {
		input = getc(stdin);
		if( input == -1 ) break;
		else while (input != '\n') getc(stdin);

		printf("\n[Update]\n");
		
		fluidMatrixGetVector(&matrix, 0, 0)->angle = 0;
		fluidMatrixGetVector(&matrix, 0, 0)->value = 50;
		fluidMatrixGetVector(&matrix, 9, 0)->angle = 3;
		fluidMatrixGetVector(&matrix, 9, 0)->value = 50;
		fluidMatrixGetVector(&matrix, 9, 9)->angle = 6;
		fluidMatrixGetVector(&matrix, 9, 9)->value = 50;
		fluidMatrixGetVector(&matrix, 0, 9)->angle = 9;
		fluidMatrixGetVector(&matrix, 0, 9)->value = 50;
		fluidMatrixUpdate(&matrix);

		printFluidMatrix(&matrix);
	}

	fluidMatrixFree(&matrix);
}

void fluidVectorTraductionTest(void) {
	FluidVectorAnglePrecision a = 16;
	FluidVectorValuePrecision v =  5;

	FluidVector vector = fluidVectorCreate(3, 3);
	printFluidVectorValue(vector.value, v);
	printFluidVectorAngle(vector.angle, a);
	printFluidVectorValue(vector.value, v);
	printf(" => ");
	Complex traduction = fluidVectorComplexRepresentation(vector, a, v);
	complexPrint(traduction);
	printf(" => ");
	vector = fluidVectorConvert(traduction, a, v);
	printFluidVectorValue(vector.value, v);
	printFluidVectorAngle(vector.angle, a);
	printFluidVectorValue(vector.value, v);
	printf("\n");
}

void fluidMatrixMainTest(void) {
	fluidMatrixTest();
}
